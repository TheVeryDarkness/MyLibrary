#pragma once

#ifndef _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
#endif // !_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING

#include "LinkedList.h"

#pragma warning(disable : 4996)

#ifndef _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
#endif // !_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING

namespace LL {
	template <typename Data>class OLL;

	//Data为数据类型，勿将其置为指针
	template <typename Data>
		//单向（oneway）链表（linked list）（基类）
		//Notice:
		//The head of the linked list should be in the stack or heap,
		//however, the other sections should be in the heap.
		class OLL
	{

		template<class node, typename _Data, _Data Radix>
		friend class LLComputeTraits;

		template<typename Type>
		INLINED friend std::ostream& MY_LIBRARY out(
			std::ostream& out, const Type& b
		)noexcept;
		template<typename Type, typename BaseType>
		INLINED friend void MY_LIBRARY SinglePrint(
			const Type& that,
			std::ostream& out,
			bool ShowComma,
			unsigned MinLength,
			BaseType base
		)noexcept;
		template<typename Type, auto Radix>
		/*INLINED*/friend std::ostream& MY_LIBRARY _Print(
			const Type& that,
			std::ostream& out
		)noexcept;
		template<
			typename Data,
			typename SubData,
			typename Type,
			typename SubType>
			friend SubType GetSubList(
				const SubData& (MY_LIBRARY* GetFunction)(const Data&),
				const Type& that
			)noexcept; 
		template<class inNode, class outNode, auto inRadix, auto outRadix, bool Destroy>
		friend outNode MY_LIBRARY Transform(inNode& in)noexcept;
		friend class Q;


		friend struct std::iterator<std::output_iterator_tag, OLL>;
		friend struct std::iterator<std::output_iterator_tag, const OLL>;

		MEMORY_CACHE(20);
	public:
		template<bool ins = false>
		constexpr INLINED std:: iterator<std::output_iterator_tag, const OLL> begin()const noexcept {
			return std::iterator<std::output_iterator_tag, const OLL>(this);
		}
		constexpr INLINED std:: iterator<std::output_iterator_tag, OLL> begin() noexcept {
			return std::iterator<std::output_iterator_tag, OLL>(this);
		}
		constexpr INLINED std::iterator<std::output_iterator_tag,OLL> end()const noexcept {
			return std::iterator<std::output_iterator_tag, OLL>(nullptr);
		}
		constexpr INLINED std::iterator<std::output_iterator_tag,const OLL> end() noexcept {
			return std::iterator<std::output_iterator_tag, const OLL>(nullptr);
		}
	protected:

		//指向下一节的指针
		OLL* next = nullptr;
		//数据
		Data data;
		//复制构造函数
		//默认为深拷贝
		INLINED MY_LIBRARY OLL(
			const OLL& that,
			bool DeepCopy
		) noexcept :data(that.data) {
			this->data = that.data;
			this->destruct();
			if (DeepCopy)
			{
				OLL* OprtPtr = this;
				const OLL* PreOprtPtr = &that;
				while (true)
				{
					if (PreOprtPtr->next != nullptr)
					{
						OprtPtr->insert(PreOprtPtr->next->data);
						OprtPtr = OprtPtr->next;
						PreOprtPtr = PreOprtPtr->next;
					}
					else
					{
						OprtPtr->next = nullptr;
						break;
					}
				}
			}
			else
			{
				this->next = that.next;
			}
			return;
		}
	public:
		INLINED size_t MY_LIBRARY RawLength()const noexcept {
			LL_LENGTH(OLL);
		}
		//复制构造函数
		//默认为浅拷贝
		INLINED MY_LIBRARY OLL(
			const OLL& that
		) noexcept :data(that.data) {
			this->destruct();
			this->next = that.next;
			return;
		}
		//仅初始化链表头的构造函数
		constexpr explicit INLINED MY_LIBRARY OLL(
			Data HeadData,
			OLL* NextPtr = nullptr
		) noexcept :data(HeadData), next(NextPtr) {}
		//释放链表头后对应链节的指针
		INLINED void MY_LIBRARY destruct() noexcept {
			while (this->next != nullptr)
			{
				this->cut();
			}
			return;
		}
		//链表版求最大值
		/*INLINED*/Data MY_LIBRARY MaxInAllNodes() const noexcept {
			const OLL* OprtPtr = this;
			Data MaxNumber = OprtPtr->data;
			do
			{
				MaxNumber =
					(MaxNumber > OprtPtr->data) ?
					(MaxNumber) : (OprtPtr->data);
				if (OprtPtr->next != nullptr)
				{
					OprtPtr = OprtPtr->next;
				}
				else
				{
					break;
				}
			} while (true);
			OprtPtr = nullptr;
			return MaxNumber;
		}
		//链表版求最小值
		/*INLINED*/Data MY_LIBRARY MinInAllNodes()const noexcept {
			const OLL* OprtPtr = this;
			Data MinNumber = OprtPtr->data;
			do
			{
				MinNumber =
					(MinNumber < OprtPtr->data) ?
					(MinNumber) : (OprtPtr->data);
				if (OprtPtr->next != nullptr)
				{
					OprtPtr = OprtPtr->next;
				}
				else
				{
					break;
				}
			} while (true);
			OprtPtr = nullptr;
			return MinNumber;
		}
		//链表版求平均值
		//链表长度不得超过LONG_MAX
		/*INLINED*/Data MY_LIBRARY Average()const noexcept
		{
			const OLL* OprtPtr = this;
			double average = 0;
			unsigned long Account = 1;
			while (OprtPtr->next != nullptr)
			{
				Account++;
				OprtPtr = OprtPtr->next;
			}
			OprtPtr = this;
			do {
				average += (OprtPtr->data / Account);
				if (OprtPtr->next != nullptr)
				{
					OprtPtr = OprtPtr->next;
				}
				else
				{
					break;
				}
			} while (true);
			OprtPtr = nullptr;
			return (long)average;
		}
		//交换
		INLINED void MY_LIBRARY swap(
			OLL& that
		) noexcept {
			Data TempData = that.data;
			that.data = this->data;
			this->data = TempData;
			OLL* TempPtr = that.next;
			that.next = this->next;
			this->next = TempPtr;
			return;
		}
		//覆盖赋值
		//浅拷贝
		//将清除被赋值对象原有内容
		INLINED OLL& MY_LIBRARY operator=(
			const OLL& that
			) noexcept {
			if (&that == this)
			{
				return *this;
			}
			this->destruct();
			this->data = that.data;
			this->next = that.next;
			return *this;
		}
		/*INLINED*/bool MY_LIBRARY operator==(
			const OLL& that
			)const noexcept {
			const OLL* OprtPtr = this, * PreOprtPtr = &that;
			while (true)
			{
				if (OprtPtr == nullptr
					&&
					PreOprtPtr == nullptr)
				{
					return true;
				}
				else if (OprtPtr == nullptr
					||
					PreOprtPtr == nullptr)
				{
					return false;
				}
				if (OprtPtr->data != PreOprtPtr->data)
				{
					return false;
				}
				OprtPtr = OprtPtr->next;
				PreOprtPtr = PreOprtPtr->next;
			}
		}
		//从链表头（不包括链表头）开始，倒置之后的链节
		//使用new创建新链表
		/*INLINED*/OLL MY_LIBRARY invert(const OLL& b) const noexcept {
			OLL Result(b.data);
			const OLL* OprtPtr = &b;
			while (OprtPtr->next != nullptr)
			{
				Result.insert(OprtPtr->next->data);
				OprtPtr = (OprtPtr->next);
			}
			return Result;
		}
		//在当前位置后插入新的一节
		INLINED void insert(Data New = Data(false)) noexcept {
			OLL* temp = this->next;
			this->next = DBG_NEW OLL(New, temp);
			return;
		}
		//删除当前位置后的一位
		INLINED void MY_LIBRARY cut() noexcept(DEBUG_FLAG) {
			MY_ASSERT(this->next != nullptr, "No next node.");
			{
				OLL* temp = this->next->next;
				delete this->next;
				this->next = temp;
			}
			return;
		}
		INLINED OLL* MY_LIBRARY Simplify() noexcept {
			LL_SIMPLIFY(OLL);
		}
		//判断that链节是否为链表的末尾
		//若使用缺省参数，表示获取当前链节是否为链表的末尾
		INLINED bool MY_LIBRARY IsEnding(
			const OLL* that = nullptr
		)const noexcept {
			return ((that == nullptr)
				?
				(this->next == nullptr)
				:
				(that->next == nullptr));
		}
		INLINED friend std::ostream& MY_LIBRARY operator<<(
			std::ostream& out, const OLL& that) noexcept {
			return LL::out<OLL>(out, that);
		}
		INLINED friend std::ostream& MY_LIBRARY operator>>(
			std::istream& in, OLL& that) noexcept {
			
		}

		void MY_LIBRARY SinglePrint(
			std::ostream& out = std::cout,
			bool Comma = true
		)const noexcept {
			return LL::SinglePrint<OLL>(*this, out);
		}
	protected:
		//获取that链节存储的数据
		//若使用缺省参数，表示获取当前链节存储的数据
		INLINED const Data& MY_LIBRARY GetThisData(
			const OLL* that = nullptr
		)const noexcept {
			return ((that == nullptr) ?
				this->data
				:
				that->data);
		}
		//获取that链节的下一链节
		//若使用缺省参数，表示获取当前链节的下一链节
		INLINED const OLL* MY_LIBRARY GetNext(
			const OLL* that = nullptr
		)const noexcept {
			return ((that == nullptr) ?
				this->next
				:
				that->next);
		}
	};
}

template<typename Data>
class std::iterator<std::output_iterator_tag, LL::OLL<Data>>
{
	using in=LL::OLL<Data>;
public:
	static constexpr auto MY_LIBRARY getRadix()noexcept { return decltype(ptr->data)::getRadix(); }
	static constexpr in* MY_LIBRARY NEXT(in& i)noexcept { if (i.next == nullptr)i.insert(); return i.next; }
	constexpr MY_LIBRARY iterator(in* _ptr)noexcept :ptr(_ptr) {}

	constexpr bool MY_LIBRARY operator==(const in* _ptr)const noexcept { return this->ptr == _ptr; }
	constexpr bool MY_LIBRARY operator==(const iterator _ptr)const noexcept { return this->ptr == _ptr.ptr; }

	MY_LIBRARY ~iterator()noexcept = default;

	constexpr iterator& MY_LIBRARY operator++()noexcept {
		if (this->ptr != nullptr)
		{
			if (this->ptr->next == nullptr)
			{
				this->ptr->insert();
			}
			this->ptr = ptr->next;
		}
		return *this;
	}

	constexpr iterator& MY_LIBRARY operator+=(size_t sz)noexcept {
		for (size_t i = 0; i < sz && this->ptr != nullptr; i++)
		{
			++(*this);
		}
		return *this;
	}

	constexpr iterator MY_LIBRARY operator+(size_t sz)const noexcept {
		iterator it(*this);
		it += sz;
		return it;
	}

	constexpr auto& MY_LIBRARY operator*()noexcept {
		if (this->ptr != nullptr) {
			return this->ptr->data;
		}
		else {
			assert((NullData == 0));
			return (NullData = 0);
		}
	}

private:
	in* ptr;
	friend struct std::iterator<std::output_iterator_tag, const LL::OLL<Data>>;
	static inline Data NullData = 0;
};

template<typename Data>
struct std::iterator<std::output_iterator_tag, const LL::OLL<Data>>
{
	using in=const LL::OLL<Data>;
public:
	static constexpr auto MY_LIBRARY getRadix()noexcept { return decltype(ptr->data)::getRadix(); }
	static constexpr in* MY_LIBRARY NEXT(in& i)noexcept { if (i.next == nullptr)i.insert(); return i.next; }
	constexpr MY_LIBRARY iterator(const in* _ptr)noexcept :ptr(_ptr) {}

	constexpr bool MY_LIBRARY operator==(const in* _ptr)const noexcept { return this->ptr == _ptr; }
	constexpr bool MY_LIBRARY operator==(const iterator _ptr)const noexcept { return this->ptr == _ptr.ptr; }

	MY_LIBRARY ~iterator()noexcept = default;

	constexpr iterator& MY_LIBRARY operator++()noexcept {
		if (this->ptr != nullptr)
		{
			this->ptr = ptr->next;
		}
		return *this;
	}

	constexpr iterator& MY_LIBRARY operator+=(size_t sz)noexcept {
		for (size_t i = 0; i < sz && this->ptr != nullptr; i++)
		{
			++(*this);
		}
		return *this;
	}

	constexpr iterator MY_LIBRARY operator+(size_t sz)const noexcept {
		iterator it(*this);
		it += sz;
		return it;
	}

	constexpr auto& MY_LIBRARY operator*()noexcept {
		if (this->ptr != nullptr) {
			return this->ptr->data;
		}
		else {
			assert((std::iterator<std::output_iterator_tag, LL::OLL<Data>>::NullData == 0));
			return static_cast<const Data&>(std::iterator<std::output_iterator_tag, LL::OLL<Data>>::NullData = 0);
		}
	}

private:
	in* ptr;
};