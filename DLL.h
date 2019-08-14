#pragma once

#include "LinkedList.h"
namespace LL {

	template<typename Data>
	class DLL
	{

		/*
		template<class node, typename _Data, _Data Radix>
		friend class LLComputeTraits;*/
		//友元函数声明

		template<typename Type, unsigned long Radix>
		/*INLINED*/friend std::ostream& MY_LIBRARY out(
			std::ostream& out, const Type& b
		)noexcept;
		template<typename Type>
		INLINED friend void MY_LIBRARY SinglePrint(
			const Type& that,
			std::ostream& out,
			bool ShowComma,
			unsigned MinLength
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

		MEMORY_CACHE(20);
	public:
		constexpr INLINED std::iterator<std::bidirectional_iterator_tag, DLL> begin() noexcept {
			return std::iterator<std::bidirectional_iterator_tag, DLL>(this);
		}
		constexpr INLINED std::iterator<std::bidirectional_iterator_tag, DLL> end() noexcept {
			return std::iterator<std::bidirectional_iterator_tag, DLL>(nullptr);
		}
		constexpr INLINED std::iterator<std::bidirectional_iterator_tag, const DLL> begin()const noexcept {
			return std::iterator<std::bidirectional_iterator_tag, const DLL>(this);
		}
		constexpr INLINED std::iterator<std::bidirectional_iterator_tag, const DLL> end()const noexcept {
			return std::iterator<std::bidirectional_iterator_tag, const DLL>(nullptr);
		}
		constexpr INLINED std::iterator<std::bidirectional_iterator_tag, const DLL> cbegin()const noexcept {
			return std::iterator<std::bidirectional_iterator_tag, const DLL>(this);
		}
		constexpr INLINED std::iterator<std::bidirectional_iterator_tag, const DLL> cend()const noexcept {
			return std::iterator<std::bidirectional_iterator_tag, const DLL>(nullptr);
		}
		Data data;
		DLL* next = nullptr;
		DLL* last = nullptr;
		//深拷贝与浅拷贝由参数DeepCopy指定
		explicit INLINED MY_LIBRARY DLL(
			const DLL& that,
			bool DeepCopy
		) noexcept :data(that.data) {
			while (this->next != nullptr)
			{
				this->cut();
			}
			if (DeepCopy)
			{
				this->last = nullptr;
				DLL* OprtPtr = this;
				const DLL* PreOprtPtr = that.next;
				while (PreOprtPtr != nullptr)
				{
					OprtPtr->insert(PreOprtPtr->data);
					OprtPtr = OprtPtr->next;
					PreOprtPtr = PreOprtPtr->next;
				}
			}
			else
			{
				this->next = that.next;
				this->last = that.last;
				if (this->next != nullptr)
				{
					this->next->last = this;
				}
			}
			return;
		}
	public:
		//按data，next，last输入
		constexpr explicit INLINED MY_LIBRARY DLL(
			Data HeadData = Data(false),
			DLL* NextPtr = nullptr,
			DLL* LastPtr = nullptr
		)noexcept :
			data(HeadData),
			next(NextPtr),
			last(LastPtr) {}
		//浅拷贝
		INLINED MY_LIBRARY DLL(
			const DLL& that
		) noexcept :data(that.data),last(that.last) {
			while (this->next != nullptr)
			{
				this->cut();
			}
			this->next = that.next;
			if (this->next != nullptr)
			{
				//此步骤非常重要
				this->next->last = this;
			}
		}
		//覆盖赋值
		//浅拷贝
		//将清除被赋值对象原有内容
		INLINED void MY_LIBRARY operator=(
			const DLL& that
			) noexcept {
			if (&that == this)
			{
				return;
			}
			this->destruct();
			this->data = that.data;
			this->next = that.next;
			this->last = that.last;
			if (this->next != nullptr)
			{
				this->next->last = this;
			}
			return;
		}
		INLINED bool MY_LIBRARY operator==(
			const DLL& that
			)const noexcept {
			const DLL* OprtPtr = this, * PreOprtPtr = &that;
			while (true)
			{
				if (OprtPtr == nullptr && PreOprtPtr == nullptr)
				{
					return true;
				}
				else if (OprtPtr == nullptr || PreOprtPtr == nullptr)
				{
					return false;
				}
				if (OprtPtr->data == PreOprtPtr->data)
				{
					OprtPtr = OprtPtr->next;
					PreOprtPtr = PreOprtPtr->next;
					continue;
				}
				return false;
			}
		}
		INLINED bool MY_LIBRARY operator!=(const DLL& that)const noexcept {
			return !(*this == that);
		}
		INLINED bool MY_LIBRARY operator<(const DLL& that)const noexcept {
			if (this->data > 0 && that.data == 0)
				return false;
			if (this->data == 0 && that.data > 0)
				return true;
			DLL* OprtPtr = this->GetEnd(), * PreOprtPtr = that.GetEnd();
			if (this->data == 0 && that.data == 0)
			{
				if (this->RawLength() < that.RawLength())
				{
					return false;
				}
				if (this->RawLength() > that.RawLength())
				{
					return true;
				}
				DLL* temp = OprtPtr;
				OprtPtr = PreOprtPtr;
				PreOprtPtr = temp;
			}
			else
			{
				if (this->RawLength() < that.RawLength())
				{
					return true;
				}
				if (this->RawLength() > that.RawLength())
				{
					return false;
				}
				//*this 与 that 均为正
			}
			//若OprtPtr对应链表小于PreOprtPtr对应链表
			//	返回真
			while (true)
			{
				if (OprtPtr->data == PreOprtPtr->data)
				{
					OprtPtr = OprtPtr->last;
					PreOprtPtr = PreOprtPtr->last;
				}
				else return (OprtPtr->data < PreOprtPtr->data);
				//相等
				if (OprtPtr->last == nullptr && PreOprtPtr->last == nullptr)
				{
					return false;
				}
			}
		}
		INLINED bool MY_LIBRARY operator<=(const DLL& that)const noexcept {
			if (this->data > 0 && that.data == 0)
				return false;
			if (this->data == 0 && that.data > 0)
				return true;
			DLL* OprtPtr = this->GetEnd(), * PreOprtPtr = that.GetEnd();
			if (this->data == 0 && that.data == 0)
			{
				if (this->RawLength() < that.RawLength())
				{
					return false;
				}
				if (this->RawLength() > that.RawLength())
				{
					return true;
				}
				DLL* temp = OprtPtr;
				OprtPtr = PreOprtPtr;
				PreOprtPtr = temp;
			}
			else
			{
				if (this->RawLength() < that.RawLength())
				{
					return true;
				}
				if (this->RawLength() > that.RawLength())
				{
					return false;
				}
				//*this 与 that 均为正
			}
			//若OprtPtr对应链表小于PreOprtPtr对应链表
			//	返回真
			while (true)
			{
				if (OprtPtr->data == PreOprtPtr->data)
				{
					OprtPtr = OprtPtr->last;
					PreOprtPtr = PreOprtPtr->last;
				}
				else return (OprtPtr->data < PreOprtPtr->data);
				//相等
				if (OprtPtr->last == nullptr && PreOprtPtr->last == nullptr)
				{
					return true;
				}
			}
		}
		INLINED bool MY_LIBRARY operator>(const DLL& that)const noexcept {
			return (that < *this);
		}
		INLINED bool MY_LIBRARY operator>=(const DLL& that)const noexcept {
			return (that <= *this);
		}
		INLINED void MY_LIBRARY destruct() noexcept {
			while (this->next != nullptr)
			{
				this->cut();
			}
			this->data = Data(0);
			return;
		}
		//在当前位置后插入新的一节
		INLINED void MY_LIBRARY insert(
			Data New = Data(false)
		) noexcept {
			DLL* temp = this->next;
			this->next = DBG_NEW DLL(New, temp, this);
			if (temp != nullptr)
			{
				temp->last = this->next;
			}
			return;
		}
		//删除当前位置后的一位
		INLINED void MY_LIBRARY cut() noexcept(DEBUG_FLAG) {
			MY_ASSERT(this->next != nullptr, "No next node.");
			{
				DLL* temp = this->next->next;
				delete this->next;
				this->next = temp;
				if (temp != nullptr)
				{
					temp->last = this;
				}
			}
			return;
		}
		//剪去空白高位
		//返回指向非空最高位的指针
		INLINED DLL* MY_LIBRARY Simplify() noexcept {
			LL_SIMPLIFY(DLL);
		}
	protected:
		//刷新last指针
		//保证指向正确
		//返回末尾指针
		INLINED DLL* MY_LIBRARY Fresh() noexcept {
			DLL* OprtPtr = this;
			while (OprtPtr->next != nullptr) {
				if (OprtPtr->next->last != OprtPtr)
				{
					assert(false);
				}
				OprtPtr->next->last = OprtPtr;
				OprtPtr = OprtPtr->next;
			}
			this->last = nullptr;
			return OprtPtr;
		}
		INLINED DLL* MY_LIBRARY GetEnd()const noexcept {
			if (this->next == nullptr)
			{
				return const_cast<DLL*>(this);
			}
			DLL* OprtPtr = this->next;
			while (OprtPtr->next != nullptr) {
				OprtPtr = OprtPtr->next;
			}
			return OprtPtr;
		}
	public:
		//Head included
		INLINED size_t MY_LIBRARY RawLength()const noexcept {
			LL_LENGTH(DLL);
		}//覆盖赋值
		/*INLINED*/void MY_LIBRARY SetValue(
			long num, const Data* data
		) noexcept {
			auto OprtPtr = this->begin();//操作当前对象
			OprtPtr->data = data;
			long count = 1;
			while (true)
			{
				if (count >= num)
				{
					break;
				}
				if (OprtPtr->next == nullptr)
				{
					OprtPtr->insert();
				}
				else
				{
					break;
				}
				++OprtPtr;
				OprtPtr->data = &data[count - 1];
				count++;
			}
			return;
		}
		//覆盖赋值
		//因为不知名的原因，对SBLinkedList禁用
		/*INLINED*/void __cdecl SetValue(
			long num, Data data, ...
		) noexcept {
			auto OprtPtr = this->begin();//操作当前对象
			OprtPtr->data = data;
			long count = 1;
			while (true)
			{
				if (count >= num)
				{
					break;
				}
				if (OprtPtr->next == nullptr)
				{
					OprtPtr->insert();
				}
				else
				{
					break;
				}
				OprtPtr = OprtPtr->next;
				OprtPtr->data = ((&data)[count - 1]);
				count++;
			}
			return;
		}
		/*
		//重载
		INLINED void MY_LIBRARY operator/=(
			unsigned long that
			) noexcept {
			if (that == 0)
			{
				MY_ASSERT(false, "Denominator can't be 0.(from operator%=(const DLL&))");
				return;
			}
			if (this->next == nullptr)
			{
				return *this;
			}
			if constexpr (Radix == 1)
			{
				return *this;
			}
			assert(Radix != 0);
			if (that % Radix == 0) {
				return ((*this >>= 1) /= (that / Radix));
			}
			else
			{
				DLL* OprtPtr = this;
				while (true)
				{
					if (OprtPtr->next != nullptr)
					{
						OprtPtr = OprtPtr->next;
					}
					else {
						break;
					}
				}
				Data Upper = 0;
				while (true)
				{
					unsigned long temp = OprtPtr->data + Upper;
					OprtPtr->data += Upper;
					Upper = temp % that;
					OprtPtr->data = temp / that;
					if (OprtPtr != this->next
						&&
						OprtPtr->last != this)
					{
						OprtPtr = OprtPtr->last;
					}
					else break;
				}
				return *this;
			}
		}
		INLINED void MY_LIBRARY operator%=(unsigned long that) noexcept {
			if (that == 0)
			{
				MY_ASSERT(false, "Denominator can't be 0.(from operator%=(const DLL&))");
				return;
			}
			if (this->next == nullptr)
			{
				return *this;
			}
			if constexpr (Radix == 1)
			{
				return *this;
			}
			if (Radix % that == 0) {
				this->next->destruct();
				this->next->data %= that;
				return *this;
			}
			DLL* OprtPtr = this;
			while (true)
			{
				if (OprtPtr->next != nullptr)
				{
					OprtPtr = OprtPtr->next;
				}
				else {
					OprtPtr->data %= that;
					break;
				}
				OprtPtr->data %= that;
			}
			unsigned long Upper = OprtPtr->data;
			while (true)
			{
				if (OprtPtr != this->next && OprtPtr->last != this
					)
				{
					OprtPtr = OprtPtr->last;
				}
				else break;
				OprtPtr->data = (OprtPtr->data + Upper * Radix) % that;
				Upper = OprtPtr->data;
			}
			OprtPtr->destruct();
			return *this;
		}
		INLINED void MY_LIBRARY operator%=(const DLL& that) noexcept(DEBUG_FLAG) {
			if (that == 0)
			{
				MY_ASSERT(false, "Denominator can't be 0.(from operator%=(const DLL&))");
				return;
			}
			bool ThisSign = (_Data(this->data) > 0);
			this->data = Data(1);
			size_t ThatLength = that.RawLength();
			long long d = (static_cast<long long>(this->RawLength()) - ThatLength);
			if (d < 0LL)
			{
				return;
			}
			size_t bit = static_cast<size_t>(d);
			DLL That(that, true);
			That.data = Data(1);
			That <<= bit;
			while (true)
			{
				//size_t ThisLength = this->RawLength();
				if (this->RawLength() < ThatLength)
				{
					That.destruct();
					break;
				}
				DLL* OprtPtr = this->GetEnd(), * PreOprtPtr = That.GetEnd();
				if (*this >= That)
				{
					if ((OprtPtr->data > PreOprtPtr->data) && ((OprtPtr->data) / (PreOprtPtr->data + Data(1))) >= Data(2))
					{
						DLL temp(That, true);
						temp *= _Data((OprtPtr->data) / (PreOprtPtr->data + Data(1)));
						*this -= temp;
						temp.destruct();
					}
					else
					{
						*this -= That;
					}
				}
				else {
					bit = static_cast<size_t>(static_cast<long long>(this->RawLength()) - that.RawLength());
					if (bit == 0)
					{
						That.destruct();
						this->data = Data(ThisSign);
						return;
					}
					else
					{
						bit--;
						That >>= 1U;
					}
				}
			}
		}
		INLINED void MY_LIBRARY operator/=(const DLL& that) noexcept(DEBUG_FLAG) {
			if (that == 0)
			{
				MY_ASSERT(false, "Denominator can't be 0.(from operator%=(const DLL&))");
				return;
			}
			size_t l1 = this->RawLength(), l2 = that.RawLength();
			if (l1 < l2)
			{
				*this = 0;
				return;
			}
			unsigned int bit = static_cast<unsigned int>(static_cast<long long>(l1) - l2);
			DLL That(that, true);
			That.data = Data(1);
			That <<= bit;
			Data ThisSign = this->data;
			this->data = Data(1);
			DLL Res(Data(true));
			while (true)
			{
				if (*this > That)
				{
					DLL* _a = this->GetEnd(), * _b = that.GetEnd();
					if ((_a->data > _b->data) && (_a->data) / (_b->data + Data(1)) >= Data(2))
					{
						Data Ratio((_a->data) / (_b->data + Data(1)) - Data(1));
						DLL temp(That * _Data(Ratio), false);
						*this -= temp;
						temp = _Data(Ratio);
						Res += temp;
						temp.destruct();
					}
					else
					{
						That.data = Data(0);
						*this += That;
						That.data = Data(1);
						DLL one(true, 1);//此处可优化
						Res += one;
						one.destruct();
					}
				}
				else if (*this == That)
				{
					//本段代码无用
					//为提高效率略去
					// *this -= that;

					DLL one(true, 1);//此处可优化
					Res += one;
					one.destruct();
					Res <<= bit;
					*this = Res;
					this->data = ThisSign;
					That.destruct();
					return;
				}
				else
				{
					if (bit > 0)
					{
						Res <<= 1U;
						That >>= 1U;
						bit--;
					}
					else
					{
						*this = Res;
						this->data = ThisSign;
						That.destruct();
						return;
					}
				}
			}
		}*/
		INLINED friend std::ostream& MY_LIBRARY operator<<(
			std::ostream& out, const DLL& that
			) noexcept {
			return LL::out<DLL>(out, that);
		}
		INLINED void MY_LIBRARY SinglePrint(
			std::ostream& out = std::cout,
			bool Comma = true
		)const noexcept {
			return LL::SinglePrint<DLL>(*this, out);
		}
		//二进制输出到控制台窗口
		INLINED std::ostream& MY_LIBRARY Print(
			std::ostream& out = std::cout
		)const noexcept {
			return LL::_Print(*this, out);
		}
	};

}



template<typename Data>
class std::iterator<std::bidirectional_iterator_tag, LL::DLL<Data>>
{
	using in=LL::DLL<Data>;
public:
	static constexpr auto MY_LIBRARY getRadix()noexcept { return decltype(ptr->data)::getRadix(); }
	static constexpr in* MY_LIBRARY NEXT(in& i)noexcept { if (i.next == nullptr)i.insert(); return i.next; }
	constexpr MY_LIBRARY iterator(in* _ptr)noexcept :ptr(_ptr) {}

	constexpr bool MY_LIBRARY operator==(const in* _ptr)const noexcept { return this->ptr == _ptr; }
	constexpr bool MY_LIBRARY operator==(const iterator _ptr)const noexcept { return this->ptr == _ptr.ptr; }
	constexpr bool MY_LIBRARY operator!=(const in* _ptr)const noexcept { return this->ptr != _ptr; }
	constexpr bool MY_LIBRARY operator!=(const iterator _ptr)const noexcept { return this->ptr != _ptr.ptr; }

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
		for (size_t i = 0; i < sz; i++)
		{
			if (it.ptr != nullptr)
			{
				it.ptr = it.ptr->next;
			}
			else break;
		}
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
	constexpr auto MY_LIBRARY operator*()const noexcept {
		if (this->ptr != nullptr) {
			return this->ptr->data;
		}
		else {
			assert((NullData == 0));
			return static_cast<Data>(NullData = 0);
		}
	}


	constexpr void insert(iterator& it, Data d) {
		it.ptr->insert(d);
	}

private:
	in* ptr;
	friend class std::iterator<std::bidirectional_iterator_tag, LL::DLL<Data>>;
	friend struct std::iterator<std::bidirectional_iterator_tag, const LL::DLL<Data>>;
	static inline Data NullData = 0;
};

template<typename Data>
struct std::iterator<std::bidirectional_iterator_tag, const LL::DLL<Data>>
{
	using in=const LL::DLL<Data>;
public:
	static constexpr auto MY_LIBRARY getRadix()noexcept { return decltype(ptr->data)::getRadix(); }
	static constexpr in* MY_LIBRARY NEXT(in& i)noexcept { if (i.next == nullptr)i.insert(); return i.next; }
	constexpr MY_LIBRARY iterator(const in* _ptr)noexcept :ptr(_ptr) {}

	constexpr bool MY_LIBRARY operator==(const in* _ptr)const noexcept { return this->ptr == _ptr; }
	constexpr bool MY_LIBRARY operator==(const iterator _ptr)const noexcept { return this->ptr == _ptr.ptr; }
	constexpr bool MY_LIBRARY operator!=(const in* _ptr)const noexcept { return this->ptr != _ptr; }
	constexpr bool MY_LIBRARY operator!=(const iterator _ptr)const noexcept { return this->ptr != _ptr.ptr; }

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
		for (size_t i = 0; i < sz; i++)
		{
			if (it.ptr != nullptr)
			{
				it.ptr = it.ptr->next;
			}
			else break;
		}
		return it;
	}

	constexpr auto& MY_LIBRARY operator*()noexcept {
		if (this->ptr != nullptr) {
			return this->ptr->data;
		}
		else {
			assert((std::iterator<std::bidirectional_iterator_tag, LL::DLL<Data>>::NullData == 0));
			return static_cast<const Data&>(std::iterator<std::bidirectional_iterator_tag, LL::DLL<Data>>::NullData = 0);
		}
	}
	constexpr auto MY_LIBRARY operator*()const noexcept {
		if (this->ptr != nullptr) {
			return this->ptr->data;
		}
		else {
			assert((std::iterator<std::bidirectional_iterator_tag, LL::DLL<Data>>::NullData == 0));
			return static_cast<const Data&>(std::iterator<std::bidirectional_iterator_tag, LL::DLL<Data>>::NullData = 0);
		}
	}

private:
	in* ptr;
};