#pragma once

#include "LinkedList.h"

namespace LL {
	template <typename _Data, _Data Radix>class OLL;
	template<typename in, bool insert>class OLLIterator;

	//Data为数据类型，勿将其置为指针
	template <
		typename _Data, _Data Radix
	>
		//单向（oneway）链表（linked list）（基类）
		//Notice:
		//The head of the linked list should be in the stack or heap,
		//however, the other sections should be in the heap.
		class OLL
	{
		using Data=LargeInteger::Num<_Data, Radix>;

		template<class node, typename _Data, _Data Radix>
		friend class LLComputeTraits;

		friend class OLLIterator<OLL,true>;
		friend class OLLIterator<OLL,false>;

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

		MEMORY_CACHE(20);
	public:
		static constexpr INLINED auto getRadix() noexcept {
			return Radix;
		}
		constexpr INLINED OLL* begin()const noexcept {
			return this;
		}
		constexpr INLINED OLL* end()const noexcept {
			return nullptr;
		}

		//重载
		INLINED void MY_LIBRARY operator*=(Data times) noexcept {
			if (times == Data(0))
			{
				this->destruct();
				return;
			}
			else if (times == Data(1))
			{
				return;
			}
			LongCmpt::MultiplyTo<OLLIterator<OLL,true>, Data, LLComputeTraits<OLL, _Data, Radix>>(times, this->next);
		}
		//重载
		/*INLINED*/OLL MY_LIBRARY operator*(Data times)const noexcept {
			OLL Res(*this, true);
			Res *= times;
			return Res;
		}
		//重载
		/*INLINED*/void MY_LIBRARY operator*=(const OLL& b) noexcept {
			if (b.next == nullptr) { return; }
			this->data = ((b.data > 0) ? (this->data) : (!this->data));
			OLL This(*this, true);
			this->destruct();
			this->insert();
			for (const OLL* OprtPtr = b.next; OprtPtr != nullptr; OprtPtr = OprtPtr->next) {
				OLL temp(This * OprtPtr->data, false);
				LongCmpt::AppositionComputeTo<LongCmpt::StdCmptTraits<Data>::Add, OLLIterator<OLL, false>,OLLIterator<OLL, true>, Data, LLComputeTraits<OLL, _Data, Radix>>(temp.next, this->next);
				temp.destruct();
				This <<= 1;
			}
			This.destruct();
		}
		//重载
		/*INLINED*/OLL MY_LIBRARY operator*(const OLL& b)const noexcept {
			OLL Res(*this, true);
			Res *= b;
			return Res;
		}
		//重载
		INLINED void MY_LIBRARY operator-=(const OLL& b) noexcept {
			*this += (-b);
		}
		//重载OLL链表负号
		INLINED OLL MY_LIBRARY operator-(
			)const noexcept {
			OLL res(*this);
			res.data = !res.data;
			return res;
		}
		//重载OLL链表+=
		INLINED void MY_LIBRARY operator+=(const OLL& that) noexcept {
			if (that.next == nullptr)
			{
				return;
			}
			if (this->next == nullptr)
			{
				*this = OLL(that, true);
				return;
			}
			if ((this->data > 0 && that.data > 0) || (this->data == 0 && that.data == 0))
			{
				LongCmpt::AppositionComputeTo<LongCmpt::StdCmptTraits<Data>::Add, OLLIterator<OLL, false>, OLLIterator<OLL, true>, Data, LLComputeTraits<OLL, _Data, Radix>>(that.next, this->next);
			}
			else {
				LongCmpt::Compare Cmpr = LongCmpt::CompareTo<OLLIterator<OLL, false>,OLLIterator<OLL, true>, Data, LLComputeTraits<OLL, _Data, Radix>>(this->next, that.next);
				if (Cmpr == LongCmpt::Compare::Equal)
				{
					this->destruct();
				}
				if (Cmpr == LongCmpt::Compare::Larger)
				{
					LongCmpt::AppositionComputeTo<LongCmpt::StdCmptTraits<Data>::SubtractFrom, OLLIterator<OLL, false>, OLLIterator<OLL, true>, Data, LLComputeTraits<OLL, _Data, Radix>>(that.next, this->next);
				}
				else
				{
					OLL temp(that, true);
					LongCmpt::AppositionComputeTo<LongCmpt::StdCmptTraits<Data>::SubtractFrom, OLLIterator<OLL, false>, OLLIterator<OLL, true>, Data, LLComputeTraits<OLL, _Data, Radix>>(this->next, temp.next);
					*this = temp;
				}
			}
			this->Simplify();
		}
		//重载OLL链表加号
		INLINED OLL MY_LIBRARY operator+(
			const OLL& b//操作数
			) const noexcept {
			OLL Result(*this, true);//存储结果
			Result += b;
			return Result;
		}
		//重载OLL链表减号
		/*INLINED*/OLL MY_LIBRARY operator-(
			const OLL& b//操作数
			)const noexcept {
			return (*this + (-b));
		}
		void MY_LIBRARY operator++() {
			OLL temp(true, 1);
			*this += temp;
			temp.destruct();
		}


		INLINED void MY_LIBRARY operator+=(const Data& that)noexcept {
			OLL temp(that, nullptr);
			if (that != Data(0) && this->next == nullptr)
			{
				this->data = Data(1);
				this->insert(that);
				return;
			}
			if (this->data == 0)
			{
				LongCmpt::AppositionComputeTo<LongCmpt::StdCmptTraits<Data>::SubtractFrom, OLLIterator<OLL, false>, Data, LLComputeTraits<OLL, _Data, Radix>>(&temp, this->next);
			}
			else
			{

				LongCmpt::AppositionComputeTo<LongCmpt::StdCmptTraits<Data>::Add, OLLIterator<OLL, false>, Data, LLComputeTraits<OLL, _Data, Radix>>(&temp, this->next);
			}
		}
		INLINED void MY_LIBRARY operator-=(const Data& that)noexcept {
			OLL temp(that, nullptr);
			if (that != Data(0) && this->next == nullptr)
			{
				this->data = Data(0);
				this->insert(that);
				return;
			}
			if (this->data > 0)
			{
				LongCmpt::AppositionComputeTo<LongCmpt::StdCmptTraits<Data>::SubtractFrom, OLLIterator<OLL, false>, Data, LLComputeTraits<OLL, _Data, Radix>>(&temp, this->next);
			}
			else
			{

				LongCmpt::AppositionComputeTo<LongCmpt::StdCmptTraits<Data>::Add, OLLIterator<OLL, false>, Data, LLComputeTraits<OLL, _Data, Radix>>(&temp, this->next);
			}
		}
		INLINED void MY_LIBRARY operator+(const Data& that)const noexcept {
			OLL res(*this, true);
			return (res += that);
		}
		INLINED void MY_LIBRARY operator-(const Data& that)const noexcept {
			OLL res(*this, true);
			return (res -= that);
		}

		//重载正
		INLINED bool MY_LIBRARY IsPositive()const noexcept {
			return (this->data > 0);
		}
		//重载负
		INLINED void MY_LIBRARY SetToContradict()noexcept {
			this->data = Data((this->data == 0) ? 1 : 0);
		}
		INLINED MY_LIBRARY ~OLL() {
			this->next = nullptr;
		}
	protected:

		//指向下一节的指针
		OLL* next = nullptr;
		//数据
		LargeInteger::Num<_Data, Radix> data;
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
		INLINED Data MY_LIBRARY GetMax()const noexcept {
			return Radix - (Data)1;
		}
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
		INLINED MY_LIBRARY OLL(
			bool positive,
			Data value
		) : data(Data(positive)) {
			this->insert(value);
		}
		INLINED MY_LIBRARY OLL(
			bool positive,
			unsigned long value
		) : data(Data(positive)) {
			*this = value;
			this->data = Data(positive);
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
		/*INLINED*/void MY_LIBRARY SetValue(
			long num, const Data* data
		) noexcept {
			OLL* OprtPtr = this;//操作当前对象
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
				OprtPtr->data = &data[count - 1];
				count++;
			}
			return;
		}
		//覆盖赋值
		//因为不知名的原因，对SBOLL禁用
		/*INLINED*/void __cdecl SetValue(
			long num, Data data, ...
		) noexcept {
			OLL* OprtPtr = this;//操作当前对象
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
		//位移运算
		//按独立进制而非二进制
		//左移时用默认值补齐
		/*INLINED*/OLL& operator<<=(
			unsigned int bits) noexcept {
			for (unsigned int i = 0; i < bits; i++)
			{
				this->insert();
			}
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
		//位移运算
		//按独立进制而非二进制
		//右移时第一位销毁
		/*INLINED*/OLL& operator>>=(unsigned int bits) noexcept {
			for (unsigned int i = 0; i < bits; i++)
			{
				this->cut();
			}
			return *this;
		}
		bool MY_LIBRARY operator<(const OLL& that)const noexcept {
			if (this->data == 0 && that.data > 0)
			{
				return true;
			}
			if (this->data > 0 && that.data == 0)
			{
				return false;
			}
			if (this->data > 0 && that.data > 0)
			{
				if (LongCmpt::CompareTo<OLLIterator<OLL, false>, Data, LLComputeTraits<OLL, _Data, Radix>>(this->next, that.next) == LongCmpt::Compare::Smaller)
					return true;
				else return false;
			}
			else
			{
				if (LongCmpt::CompareTo<OLLIterator<OLL, false>, Data, LLComputeTraits<OLL, _Data, Radix>>(that.next, this->next) == LongCmpt::Compare::Smaller)
					return true;
				else return false;
			}
		}
		bool MY_LIBRARY operator>(const OLL& that)const noexcept {
			if (this->data == 0 && that.data > 0)
			{
				return false;
			}
			if (this->data > 0 && that.data == 0)
			{
				return true;
			}
			if (this->data > 0 && that.data > 0)
			{
				if (LongCmpt::CompareTo<OLLIterator<OLL, false>, Data, LLComputeTraits<OLL, _Data, Radix>>(this->next, that.next) == LongCmpt::Larger)
					return true;
				else return false;
			}
			else
			{
				if (LongCmpt::CompareTo<OLLIterator<OLL, false>, Data, LLComputeTraits<OLL, _Data, Radix>>(that.next, this->next) == LongCmpt::Larger)
					return true;
				else return false;
			}
		}
		bool MY_LIBRARY operator<=(const OLL& that)const noexcept {
			return !(*this > that);
		}
		bool MY_LIBRARY operator>=(const OLL& that)const noexcept {
			return !(*this < that);
		}
		void MY_LIBRARY operator%=(const OLL& that)noexcept {
			if (this->next != nullptr && that.next != nullptr)
			{
				LongCmpt::DivideInto<OLLIterator<OLL, false>, Data, LLComputeTraits<OLL, _Data, Radix>>(that.next, this->next);
			}
			else return;
			this->Simplify();
		}
		void MY_LIBRARY operator/=(const OLL& that)noexcept {
			if (this->next != nullptr && that.next != nullptr)
			{
				OLL Res(true);
				LongCmpt::DivideInto<OLL, OLLIterator<OLL, false>, Data, LLComputeTraits<OLL, _Data, Radix>>(Res, that.next, this->next);
				this->destruct();
				this->next = Res.next;
			}
			else return;
			this->Simplify();
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
		//覆盖赋值
		/*INLINED*/OLL& MY_LIBRARY operator=(
			long value
			) noexcept {
			if constexpr (Radix == _Data(1))
			{
				this->data = (Data)value;
			}
			else
			{
				OLL* OprtPtr = this;//操作当前对象
				if (value >= 0)
				{
					OprtPtr->data = Data(true);
				}
				else
				{
					OprtPtr->data = Data(false);
					value = -value;
				}
				while (true)
				{
					if (value == 0)
					{
						return *this;
					}
					if constexpr (Radix == 0) {
						OprtPtr->insert(static_cast<Data>(value));
						if (sizeof(Data) >= sizeof(value))
						{
							return *this;
						}
						value >>= (LargeInteger::BitsPerByte * sizeof(Data));
					}
					else {
						OprtPtr->insert((Data)(value % Radix));
						value = value / Radix;
					}
					OprtPtr = OprtPtr->next;
				}
			}
			return *this;
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
		//获取存储的值
		//可能溢出
		/*INLINED*/long long MY_LIBRARY GetValue()const noexcept {
			bool sign = (this->data > 0) ? true : false;
			long long value = 0;
			long n = 0;
			const OLL* OprtPtr = this;
			if (OprtPtr == nullptr)
			{
				return 0;
			}
			while (true)
			{
				if (OprtPtr->next != nullptr)
				{
					OprtPtr = OprtPtr->next;
					value += ((unsigned long long)(OprtPtr->data())) * Power((unsigned long long)Radix, n);
					n++;
				}
				else
				{
					break;
				}
			}
			return (sign) ? value : (-value);
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
		//二进制输出到控制台窗口
		/*INLINED*/std::ostream& MY_LIBRARY Print(
			std::ostream& out = std::cout
		) const noexcept {
			return LL::_Print<OLL, Radix>(*this, out);
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



	template<typename in,bool insert>
	class OLLIterator
	{
	public:
		static constexpr auto MY_LIBRARY getRadix()noexcept { return in::getRadix(); }
		static constexpr in* MY_LIBRARY NEXT(in& i)noexcept { if (i.next == nullptr)i.insert(); return i.next; }
		constexpr MY_LIBRARY OLLIterator(in* _ptr)noexcept :ptr(_ptr) {}
		constexpr in*& MY_LIBRARY operator()()noexcept { return this->ptr; }
		constexpr bool MY_LIBRARY operator==(in* that)const noexcept { return this->ptr == that; }

		MY_LIBRARY ~OLLIterator()noexcept = default;

		constexpr OLLIterator& MY_LIBRARY operator++()noexcept {
			if (this->ptr != nullptr)
			{
				if constexpr (insert)
				{
					if (this->ptr->next == nullptr)
					{
						this->ptr->insert();
					}
				}
				this->ptr = ptr->next;
			}
			return *this;
		}
		
		constexpr OLLIterator MY_LIBRARY operator+(size_t sz)const noexcept {
			OLLIterator it(this->ptr);
			for (size_t i = 0; i < sz; i++)
			{
				if (it.ptr != nullptr)
				{
					++it;
				}
				else
				{
					break;
				}
			}
			return it;
		}

		constexpr LargeInteger::Num<decltype(in::getRadix()), in::getRadix()>& MY_LIBRARY operator*()noexcept {
			if (this->ptr != nullptr){
				return this->ptr->data;
			}
			else {
				assert((LLComputeTraits<in, decltype(in::getRadix()), in::getRadix()>::NullData == 0));
				return (LLComputeTraits<in, decltype(in::getRadix()), in::getRadix()>::NullData = 0);
			}
		}

		constexpr in*& operator->() noexcept{
			return this->ptr;
		}

	private:
		in* ptr;
	};
}