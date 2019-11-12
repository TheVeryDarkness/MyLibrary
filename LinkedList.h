#pragma once

#include "Shared.h"
#include "VisualStudioDebug.h"
#include "Statistics.h"
#include "MemoryCache.h"
#include <limits>
#include <climits>
#include <type_traits>
#include <cassert>
#include <iostream>
#include <iomanip>

//#include <charconv>

//根据规范（当然是我自己胡诌的规范），
//Simplify()操作应在加减乘除操作完成后返回时主动调用
//以避免冗余
//#pragma pack(push)
//#pragma pack(2)
//#pragma pack(pop)
#define LL_LENGTH(type) const type* OprtPtr=this;size_t s=0;while(OprtPtr!=nullptr){OprtPtr=OprtPtr->next;s++;}return s;
#define LL_SIMPLIFY(type) {type* Flag = this;type* OprtPtr = this;while (true){if (OprtPtr->data!=Data(0U)){Flag = OprtPtr;}if (OprtPtr->next == nullptr){break;}OprtPtr = OprtPtr->next;}while (Flag->next != nullptr){Flag->cut();}return Flag;}

//Use an array to cache memory
#define MEMORY_CACHE(MEMORY_CACHE_SIZE) \
static INLINED MemoryCache<MEMORY_CACHE_SIZE> Buffer = {};\
static void* MY_LIB operator new(size_t size)noexcept {return Buffer.pop(size);}\
static void MY_LIB operator delete(void* _ptr)noexcept {return Buffer.push(_ptr);}

namespace Darkness {
	inline namespace LL {
		template<typename Type>
		//输出流
		INLINED std::ostream &MY_LIB out(
			std::ostream &out, const Type &b
		) noexcept {
			if (b.next != nullptr) {
				out << *(b.next) << "," << b.data;
			}
			else {
				out << b.data;
			}
			return out;
		}
		template<typename Type>
		//简单输出到控制台窗口
		//需要用户补换行
		[[deprecated]] INLINED void MY_LIB SinglePrint(
			const Type &that,
			std::ostream &out = std::cout,
			bool ShowComma = true
		) noexcept {
			if (that.next != nullptr) {
				SinglePrint(*that.next, out, ShowComma);
				out << ((ShowComma) ? ',' : ' ');
			}
			out << that.data;
			return;
		}
		//回调接口
		//Data为源链表链节中数据源的类型
		//SubData为取出的数据的类型
		//Type为源链表类型
		//SubType为子链表
		template<
			typename __Data,
			typename SubData,
			typename Type,
			typename SubType
		>
			SubType GetSubList(
				const SubData &(MY_LIB *GetFunction)(const __Data &),
				const Type &that
			) noexcept {
			const Type *OprtPtr = &that;
			SubType Result(GetFunction(that.data));
			SubType *SubOprtPtr = &Result;
			while (true) {
				if (OprtPtr->next != nullptr) {
					SubOprtPtr->next = DBG_NEW SubType(GetFunction(OprtPtr->next->data));
				}
				else {
					break;
				}
				OprtPtr = OprtPtr->next;
			}
			return Result;
		}


		template<class inNode, class outNode, auto inRadix, auto outRadix, bool Destroy = true>
		outNode MY_LIB Transform(inNode &in)noexcept {
			static_assert(Math::GetPowerTimes(outRadix, inRadix) != 0);
			constexpr auto times = Math::GetPowerTimes(outRadix, inRadix);
			if constexpr (Destroy) {
				outNode out(0);
				while (true) {
					decltype(out.data) temp = 0;
					for (typename std::remove_const<decltype(times)>::type i = 0; i < times; i++) {
						temp += static_cast<decltype(temp)>(in._pop()) *Math::Power(inRadix, i);
						if (in.next == nullptr)
							break;
					}
					if (in.next == nullptr) {
						out.data = temp;
						out = out.invert(out);
						return out;
					}
					else out.insert(temp);
				}
			}
			else {
				static_assert(Destroy);
			}
		}


		template<typename T>const T &to_const(T &ref)noexcept {
			return static_cast<const T &>(ref);
		}
	}
}