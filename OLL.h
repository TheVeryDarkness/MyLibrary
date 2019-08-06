#pragma once

#include "LinkedList.h"

namespace LL {
	template <typename _Data, _Data Radix>class OLL;
	template<typename in, bool insert>class OLLIterator;

	//DataΪ�������ͣ�������Ϊָ��
	template <
		typename _Data, _Data Radix
	>
		//����oneway������linked list�������ࣩ
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

		//����
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
		//����
		/*INLINED*/OLL MY_LIBRARY operator*(Data times)const noexcept {
			OLL Res(*this, true);
			Res *= times;
			return Res;
		}
		//����
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
		//����
		/*INLINED*/OLL MY_LIBRARY operator*(const OLL& b)const noexcept {
			OLL Res(*this, true);
			Res *= b;
			return Res;
		}
		//����
		INLINED void MY_LIBRARY operator-=(const OLL& b) noexcept {
			*this += (-b);
		}
		//����OLL������
		INLINED OLL MY_LIBRARY operator-(
			)const noexcept {
			OLL res(*this);
			res.data = !res.data;
			return res;
		}
		//����OLL����+=
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
		//����OLL����Ӻ�
		INLINED OLL MY_LIBRARY operator+(
			const OLL& b//������
			) const noexcept {
			OLL Result(*this, true);//�洢���
			Result += b;
			return Result;
		}
		//����OLL�������
		/*INLINED*/OLL MY_LIBRARY operator-(
			const OLL& b//������
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

		//������
		INLINED bool MY_LIBRARY IsPositive()const noexcept {
			return (this->data > 0);
		}
		//���ظ�
		INLINED void MY_LIBRARY SetToContradict()noexcept {
			this->data = Data((this->data == 0) ? 1 : 0);
		}
		INLINED MY_LIBRARY ~OLL() {
			this->next = nullptr;
		}
	protected:

		//ָ����һ�ڵ�ָ��
		OLL* next = nullptr;
		//����
		LargeInteger::Num<_Data, Radix> data;
		//���ƹ��캯��
		//Ĭ��Ϊ���
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
		//���ƹ��캯��
		//Ĭ��Ϊǳ����
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
		//����ʼ������ͷ�Ĺ��캯��
		constexpr explicit INLINED MY_LIBRARY OLL(
			Data HeadData,
			OLL* NextPtr = nullptr
		) noexcept :data(HeadData), next(NextPtr) {}
		//�ͷ�����ͷ���Ӧ���ڵ�ָ��
		INLINED void MY_LIBRARY destruct() noexcept {
			while (this->next != nullptr)
			{
				this->cut();
			}
			return;
		}
		//����������ֵ
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
		//���������Сֵ
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
		//�������ƽ��ֵ
		//�����Ȳ��ó���LONG_MAX
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
		//����
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
		//���Ǹ�ֵ
		/*INLINED*/void MY_LIBRARY SetValue(
			long num, const Data* data
		) noexcept {
			OLL* OprtPtr = this;//������ǰ����
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
		//���Ǹ�ֵ
		//��Ϊ��֪����ԭ�򣬶�SBOLL����
		/*INLINED*/void __cdecl SetValue(
			long num, Data data, ...
		) noexcept {
			OLL* OprtPtr = this;//������ǰ����
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
		//���Ǹ�ֵ
		//ǳ����
		//���������ֵ����ԭ������
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
		//λ������
		//���������ƶ��Ƕ�����
		//����ʱ��Ĭ��ֵ����
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
		//λ������
		//���������ƶ��Ƕ�����
		//����ʱ��һλ����
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
		//������ͷ������������ͷ����ʼ������֮�������
		//ʹ��new����������
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
		//�ڵ�ǰλ�ú�����µ�һ��
		INLINED void insert(Data New = Data(false)) noexcept {
			OLL* temp = this->next;
			this->next = DBG_NEW OLL(New, temp);
			return;
		}
		//ɾ����ǰλ�ú��һλ
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
		//���Ǹ�ֵ
		/*INLINED*/OLL& MY_LIBRARY operator=(
			long value
			) noexcept {
			if constexpr (Radix == _Data(1))
			{
				this->data = (Data)value;
			}
			else
			{
				OLL* OprtPtr = this;//������ǰ����
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
		//�ж�that�����Ƿ�Ϊ�����ĩβ
		//��ʹ��ȱʡ��������ʾ��ȡ��ǰ�����Ƿ�Ϊ�����ĩβ
		INLINED bool MY_LIBRARY IsEnding(
			const OLL* that = nullptr
		)const noexcept {
			return ((that == nullptr)
				?
				(this->next == nullptr)
				:
				(that->next == nullptr));
		}
		//��ȡ�洢��ֵ
		//�������
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
		//���������������̨����
		/*INLINED*/std::ostream& MY_LIBRARY Print(
			std::ostream& out = std::cout
		) const noexcept {
			return LL::_Print<OLL, Radix>(*this, out);
		}
	protected:
		//��ȡthat���ڴ洢������
		//��ʹ��ȱʡ��������ʾ��ȡ��ǰ���ڴ洢������
		INLINED const Data& MY_LIBRARY GetThisData(
			const OLL* that = nullptr
		)const noexcept {
			return ((that == nullptr) ?
				this->data
				:
				that->data);
		}
		//��ȡthat���ڵ���һ����
		//��ʹ��ȱʡ��������ʾ��ȡ��ǰ���ڵ���һ����
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