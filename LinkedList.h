#pragma once

#include "VisualStudioDebug.h"


#include "CustomizedRadixNumber.h"
#include "Statistics.h"
#include "MemoryCache.h"
#include <limits>
#include <climits>
#include <type_traits>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <charconv>

//根据规范（当然是我自己胡诌的规范），
//Simplify()操作应在加减乘除操作完成后返回时主动调用
//以避免冗余
//#pragma pack(push)
//#pragma pack(2)
//#pragma pack(pop)
#define LL_LENGTH(type) const type* OprtPtr=this;size_t s=0;while(OprtPtr!=nullptr){OprtPtr=OprtPtr->next;s++;}return s;
#define LL_SIMPLIFY(type) {type* Flag = this;type* OprtPtr = this;while (true){if (OprtPtr->data!=Data(0U)){Flag = OprtPtr;}if (OprtPtr->next == nullptr){break;}OprtPtr = OprtPtr->next;}while (Flag->next != nullptr){Flag->cut();}return Flag;}

//Use a std::array to cache memory
#define MEMORY_CACHE(MEMORY_CACHE_SIZE) \
static INLINED MemorryCache<MEMORY_CACHE_SIZE> Buffer = {};\
static void* MY_LIBRARY operator new(size_t size)noexcept {return Buffer.pop(size);}\
static void MY_LIBRARY operator delete(void* _ptr, size_t size)noexcept {return Buffer.push(_ptr);}

//#define MEMORY_CACHE(X)

namespace LL {


	//定义

	template<typename Type>
	//输出流
	/*INLINED*/std::ostream& MY_LIBRARY out(
		std::ostream& out, const Type& b
	) noexcept {
		if (b.next != nullptr)
		{
			out << *(b.next) << "," << b.data;
		}
		else
		{
			out << b.data;
		}
		return out;
	}
	template<typename Type, typename BaseType>
	//简单输出到控制台窗口
	//需要用户补换行
	INLINED void MY_LIBRARY SinglePrint(
		const Type& that,
		std::ostream& out = std::cout,
		bool ShowComma = true,
		unsigned MinLength = 0
	) noexcept {
		if (that.next != nullptr)
		{
			SinglePrint(*that.next, out, ShowComma, MinLength);
			out << ((ShowComma) ? "," : "");
			char* c = DBG_NEW char[MinLength + 1ULL]();
			assert(base < BaseType(INT_MAX));
			std::to_chars_result rs = std::to_chars(c, &(c[MinLength]), that.data());

			std::string str = c;
			delete[] c;
			if (str.length() < MinLength)
			{
				std::string nStr;
				for (size_t i = MinLength - str.length(); i > 0; i--)
				{
					nStr.push_back('0');
				}
				nStr += str;
				out << nStr;
			}
			else out << str;
		}
		else
		{
			out << that.data;
		}
		return;
	}
	//回调接口
	//Data为源链表链节中数据源的类型
	//SubData为取出的数据的类型
	//Type为源链表类型
	//SubType为子链表
	template<
		typename Data,
		typename SubData,
		typename Type,
		typename SubType
	>
		SubType GetSubList(
			const SubData& (MY_LIBRARY* GetFunction)(const Data&),
			const Type& that
		) noexcept {
		const Type* OprtPtr = &that;
		SubType Result(GetFunction(that.data));
		SubType* SubOprtPtr = &Result;
		while (true)
		{
			if (OprtPtr->next != nullptr)
			{
				SubOprtPtr->next = DBG_NEW SubType(GetFunction(OprtPtr->next->data));
			}
			else
			{
				break;
			}
			OprtPtr = OprtPtr->next;
		}
		return Result;
	}


	template<class inNode, class outNode, auto inRadix, auto outRadix, bool Destroy = true>
	outNode MY_LIBRARY Transform(inNode & in)noexcept {
		static_assert(GetPowerTimes(outRadix, inRadix) != 0);
		constexpr auto times = GetPowerTimes(outRadix, inRadix);
		if constexpr (Destroy)
		{
			outNode out(in.data);
			while (true)
			{
				decltype(out.data) temp = 0;
				for (typename std::remove_const<decltype(times)>::type i = 0; i < times; i++)
				{
					if (in.next != nullptr)
					{
						temp += static_cast<decltype(temp)>(in.next->data) * Power(inRadix, i);
						in.cut();
					}
					else break;
				}
				out.insert(temp);
				if (in.next == nullptr)
				{
					out = out.invert(out);
					return out;
				}
			}
		}
		else
		{
			static_assert(Destroy);
		}
	}

	/*
	template<class _node, typename _Data, _Data Radix>
	class LLComputeTraits :public LargeInteger::StdCmptTraits<LargeInteger::Num<_Data, Radix>>
	{
		using node=std::remove_const<_node>::type;
	public:
		using Data=LargeInteger::Num<_Data, Radix>;
		MY_LIBRARY LLComputeTraits() = delete;
		MY_LIBRARY ~LLComputeTraits() = delete;
		static INLINED Data NullData = Data(0);
		constexpr static INLINED node* NullIterator = nullptr;

		using Multiply=typename Data::Multiply;

		static constexpr Data& MY_LIBRARY GetData(node* ptr) {
			MY_ASSERT(NullData == Data(0), "Unexpected write has occured!");
			return ((ptr == nullptr) ? (NullData = Data(0)) : (ptr->data));
		}
		static constexpr Data MY_LIBRARY GetData(const node* ptr) {
			MY_ASSERT(NullData == Data(0), "Unexpected write has occured!");
			return ((ptr == nullptr) ? (NullData = Data(0)) : (ptr->data));
		}

		static constexpr node* MY_LIBRARY GetNext(node* ptr) {
			return ((ptr == nullptr) ? nullptr : (ptr->next));
		}

		static constexpr void MY_LIBRARY assign(node* ptr, unsigned sz = 1) {
			*ptr <<= sz;
		}
		static constexpr void MY_LIBRARY InsertAfter(node*& ptr, Data data = Data(0)) {
			ptr->insert(data);
		}
	};
	*/
}
