#pragma once

#include "LinkedList.h"
namespace LL {

	template<typename _Data, _Data Radix>
	class DLL
	{
		using Data=LargeInteger::Num<_Data, Radix>;

		template<class node, typename Data>
		friend class LLComputeTraits;
		//��Ԫ��������

		template<typename Type, unsigned long Radix>
		/*INLINED*/friend std::ostream& MY_LIBRARY out(
			std::ostream& out, const Type& b
		)noexcept;
		template<typename Type>
		INLINED friend void MY_LIBRARY SinglePrint(
			const Type& that,
			std::ostream& out,
			bool ShowComma,
			unsigned MinLength,
			unsigned base
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
		friend class Q;

		MEMORY_CACHE(20);
	public:
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
			LongCmpt::MultiplyTo<DLL*, Data, LLComputeTraits<DLL, Data>>(times, this->next);
		}
		//����
		INLINED DLL MY_LIBRARY operator*(Data times)const noexcept {
			DLL Res(*this, true);
			Res *= times;
			return Res;
		}
		//����
		void MY_LIBRARY operator*=(const DLL& b) noexcept {
			if (b.next == nullptr) { return; }
			this->data = ((b.data > 0) ? (this->data) : (!this->data));
			DLL This(*this, true);
			this->destruct();
			this->insert();
			for (DLL* OprtPtr = b.next; OprtPtr != nullptr; OprtPtr = OprtPtr->next)
			{
				DLL temp(This * OprtPtr->data);
				LongCmpt::AppositionComputeTo<LongCmpt::StdCmptTraits<Data>::Add, DLL*, Data, LLComputeTraits<DLL, Data>>(temp.next, this->next);
				temp.destruct();
				This <<= 1;
			}
			This.destruct();
		}
		//����
		INLINED DLL MY_LIBRARY operator*(const DLL& b)const noexcept {
			DLL Res(*this, true);
			Res *= b;
			return Res;
		}
		//������
		INLINED bool MY_LIBRARY IsPositive()const noexcept {
			return (this->data > 0);
		}
		//���ظ�
		INLINED void MY_LIBRARY SetToContradict()noexcept {
			this->data = Data((this->data == 0) ? 1 : 0);
		}
		//����DLL����Ӻ�
		INLINED DLL MY_LIBRARY operator+(
			const DLL& b//������
			)  const noexcept {
			DLL Result(*this, true);//�洢���
			Result += b;
			return Result;
		}
		//����DLL����+=
		INLINED void MY_LIBRARY operator+=(const DLL& that)noexcept {
			if (that.next == nullptr)
			{
				return;
			}
			if (this->next == nullptr)
			{
				*this = DLL(that, true);
				return;
			}
			if ((this->data > 0 && that.data > 0) || (this->data == 0 && that.data == 0))
			{
				LongCmpt::AppositionComputeTo<LongCmpt::StdCmptTraits<Data>::Add, DLL*, Data, LLComputeTraits<DLL, Data>>(that.next, this->next);
			}
			else {
				LongCmpt::Compare Cmpr = LongCmpt::CompareTo<DLL*, Data, LLComputeTraits<DLL, Data>>(this->next, that.next);
				if (Cmpr == LongCmpt::Compare::Equal)
				{
					this->destruct();
				}
				if (Cmpr == LongCmpt::Compare::Larger)
				{
					LongCmpt::AppositionComputeTo<LongCmpt::StdCmptTraits<Data>::SubtractFrom, DLL*, Data, LLComputeTraits<DLL, Data>>(that.next, this->next);
				}
				else
				{
					DLL temp(that, true);
					LongCmpt::AppositionComputeTo<LongCmpt::StdCmptTraits<Data>::SubtractFrom, DLL*, Data, LLComputeTraits<DLL, Data>>(this->next, temp.next);
					*this = temp;
				}
				this->Simplify();
			}
		}
		//����DLL�������
		INLINED DLL MY_LIBRARY operator-(
			const DLL& b//������
			)const noexcept {
			return (*this + (-b));
		}
		//����
		INLINED void MY_LIBRARY operator-=(const DLL& b)noexcept {
			DLL _b = -b;
			*this += _b;
			_b.destruct();
		}
		//����
		INLINED void MY_LIBRARY operator-=(DLL& b)noexcept {
			Data Orig = b.data;
			b.data = Data(!b.data);
			*this += b;
			b.data = Orig;
		}
		//����DLL������
		INLINED DLL MY_LIBRARY operator-(
			)const noexcept {
			DLL res(*this, true);
			res.data = Data(!res.data);
			return res;
		}
		void MY_LIBRARY operator++() {
			DLL temp(true, 1);
			*this += temp;
			temp.destruct();
		}
		INLINED _stdcall ~DLL() {
			this->next = nullptr;
			this->last = nullptr;
		}
#ifdef _DEBUG
	public:
#else
	protected:
#endif // _DEBUG
		LargeInteger::Num<_Data, Radix> data;
		DLL* next = nullptr;
		DLL* last = nullptr;
		//�����ǳ�����ɲ���DeepCopyָ��
		explicit INLINED MY_LIBRARY DLL(
			const DLL& that,
			bool DeepCopy
		) noexcept :data(that.data) {
			this->destruct();
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
		//��data��next��last����
		constexpr explicit INLINED MY_LIBRARY DLL(
			Data HeadData = Data(false),
			DLL* NextPtr = nullptr,
			DLL* LastPtr = nullptr
		)noexcept :
			data(HeadData),
			next(NextPtr),
			last(LastPtr) {}
		//ǳ����
		INLINED MY_LIBRARY DLL(
			const DLL& that
		) noexcept :data(that.data) {
			this->destruct();
			this->next = that.next;
			this->last = that.last;
			if (this->next != nullptr)
			{
				//�˲���ǳ���Ҫ
				this->next->last = this;
			}
		}
		//���Ǹ�ֵ
		//ǳ����
		//���������ֵ����ԭ������
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
			int that
			)const noexcept {
			if (this->next == nullptr && that == 0)
			{
				return true;
			}
			else if (that == 0)
			{
				return false;
			}
			if (
				(this->data > 0 && that >= 0)
				||
				(this->data == 0 && that <= 0)
				) {
				that = std::abs(that);
			}
			else return false;
			DLL* OprtPtr = this->next;
			while (OprtPtr != nullptr)
			{
				if constexpr (Radix == 0) {
					if (OprtPtr->data != ((Data)that))
						return false;
				}
				else if (OprtPtr->data != (that % Radix))
					return false;
				else
				{
					if constexpr (Radix == 0)
						that >> (Array::BitsPerByte * sizeof(Data));
					else that /= Radix;
					if (that == 0)
					{
						if (OprtPtr->next == nullptr || OprtPtr->next->data == 0)
						{
							return true;
						}
						else return false;
					}
					OprtPtr = OprtPtr->next;
				}
			}
			return false;
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
				//*this �� that ��Ϊ��
			}
			//��OprtPtr��Ӧ����С��PreOprtPtr��Ӧ����
			//	������
			while (true)
			{
				if (OprtPtr->data == PreOprtPtr->data)
				{
					OprtPtr = OprtPtr->last;
					PreOprtPtr = PreOprtPtr->last;
				}
				else return (OprtPtr->data < PreOprtPtr->data);
				//���
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
				//*this �� that ��Ϊ��
			}
			//��OprtPtr��Ӧ����С��PreOprtPtr��Ӧ����
			//	������
			while (true)
			{
				if (OprtPtr->data == PreOprtPtr->data)
				{
					OprtPtr = OprtPtr->last;
					PreOprtPtr = PreOprtPtr->last;
				}
				else return (OprtPtr->data < PreOprtPtr->data);
				//���
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
			return;
		}
		//�ڵ�ǰλ�ú�����µ�һ��
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
		//ɾ����ǰλ�ú��һλ
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
		//��ȥ�հ׸�λ
		//����ָ��ǿ����λ��ָ��
		INLINED DLL* MY_LIBRARY Simplify() noexcept {
			LL_SIMPLIFY(DLL);
		}
	protected:
		//ˢ��lastָ��
		//��ָ֤����ȷ
		//����ĩβָ��
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
		}
		explicit INLINED MY_LIBRARY DLL(
			bool positive,
			unsigned long value
		) noexcept :
			data(static_cast<Data>(positive)) {
			static_assert(Radix != 1, "Not supported constructor for the radix.");
			{
				this->destruct();
				DLL* OprtPtr = this;//������ǰ����
				while (true)
				{
					if (value == 0)
					{
						break;
					}
					if (OprtPtr->next == nullptr)
					{
						if constexpr (Radix == 0)
						{
							OprtPtr->insert((Data)value);
							value >>= (Array::BitsPerByte * sizeof(Data));
						}
						else
						{
							OprtPtr->insert(
								(Data)(value % Radix)
							);
							value /= Radix;
						}
						OprtPtr = OprtPtr->next;
					}
				}
			}
		}
		//ǳ����
		//���Ǹ�ֵ
		INLINED DLL& MY_LIBRARY operator=(
			long value
			) noexcept {
			this->destruct();
			*this = DLL(((value >= 0) ? 1 : 0), std::abs(value));
			return *this;
		}
		//λ������
		//��������λ
		//���������ƶ��Ƕ�����
		//����ʱ��Ĭ��ֵ����
		INLINED DLL& operator<<=(
			size_t bits
			) noexcept {
			for (size_t i = 0; i < bits; i++)
			{
				this->insert();
			}
			return *this;
		}
		//λ������
		//��������λ
		//���������ƶ��Ƕ�����
		//����ʱ��һλ����
		INLINED DLL& operator>>=(
			size_t bits
			) noexcept {
			for (size_t i = 0; i < bits; i++)
			{
				this->cut();
			}
			return *this;
		}
		//����
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
					if (((OprtPtr->data) / (PreOprtPtr->data + Data(1))) >= Data(2))
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
						DLL one(true, 1);//�˴����Ż�
						Res += one;
						one.destruct();
					}
				}
				else if (*this == That)
				{
					//���δ�������
					//Ϊ���Ч����ȥ
					//*this -= that;

					DLL one(true, 1);//�˴����Ż�
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
		}
		INLINED friend std::ostream& MY_LIBRARY operator<<=(
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
		//���������������̨����
		INLINED std::ostream& MY_LIBRARY Print(
			std::ostream& out = std::cout
		)const noexcept {
			return LL::_Print<DLL, Radix>(*this, out);
		}
	};

}