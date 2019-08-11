#pragma once

#include "Shared.h"
#include "Statistics.h"
#include "ComputeTemplate.h"
#include <cassert>

namespace LargeInteger {


	template<typename LL, auto radix>
	class LargeUnsigned
	{
	private:
		LL LinkedList;
		using radix_t=decltype(radix);
		using Data=Num<radix_t, radix>;
	public:
		template<typename val>
		MY_LIBRARY LargeUnsigned(val Val)noexcept {
			LargeInteger::LongCmpt<StdCmptTraits<Num<radix_t, radix>>>::LayerIterator<StdCmptTraits<Num<radix_t, radix>>::Devide> it;
			for (auto& i = LinkedList.begin();; ++i)
			{
				i = Val % radix;
				if ((Val /= radix) == static_cast<val>(0)) {
					break;
				}
			}
			assert(Val == static_cast<val>(0));
			return;
		}

		MY_LIBRARY LargeUnsigned(const LargeUnsigned& that,bool DeepCopy)noexcept {
			if constexpr(DeepCopy)
			{
				for (auto& i : that) {
					auto& j = this->LinkedList.begin();
					*j = *i;
				}
				return;
			}
			else
			{
				this->LinkedList = that.LinkedList;
			}
		}

		//���������������̨����
		/*INLINED*/friend std::ostream& MY_LIBRARY operator<<(
			std::ostream& out,
			const LargeUnsigned& l
		) noexcept {
			return _Print<LargeUnsigned, radix>(l, out);
		}
		template<typename Type, auto Radix = 0>
		//���������������̨����
		//�����Զ�����
		/*INLINED*/std::ostream & MY_LIBRARY _Print(
			const Type & that,
			std::ostream & out = std::cout
		) noexcept {
			if (that.next == nullptr)
			{
				out << "0";
				return out;
			}
			if (!that.data)
			{
				out << "-";
			}
			if (Radix == static_cast<decltype(Radix)>(0))
			{
				out << "0x"
					<< std::setbase(16);
				SinglePrint<Type>(*(that.next), out, false, 2 * sizeof(Radix), 16);
			}
			else
			{
				bool temp = false;
				unsigned int Length = 0;
				Length = GetPowerTimes(Radix, 10);
				decltype(Radix) OutBase = 0;
				if (Length == 0)
				{
					Length = GetPowerTimes(Radix, 16);
					if (Length == 0)
					{
						Length = GetPowerTimes(Radix, 8);
						if (Length == 0)
						{
							Length = GetPowerTimes(Radix, 2);
							if (Length == 0)
							{
								Length = 1;
								OutBase = Radix;
								out << "(Base:"
									<< Radix
									<< ")";
								temp = true;
							}
							else
							{
								OutBase = 2;
								out << "0b" << std::setbase(2);
							}
						}
						else
						{
							OutBase = 8;
							out << "0" << std::setbase(8);
						}
					}
					else {
						OutBase = 16;
						out << "0x"
							<< std::setbase(16);
					}
				}
				else
				{
					OutBase = 10;
				}
				SinglePrint<Type>(*(that.next), out, temp, Length, OutBase);
			}

			out << std::setbase(10);
			return out;
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
			LargeInteger::LongCmpt<LLComputeTraits<LinkedList, radix_t, Radix>>::MultiplyTo<LinkedList*, Data>(times, this->next);
		}
		//����
		/*INLINED*/LL MY_LIBRARY operator*(Data times)const noexcept {
			LL Res(*this, true);
			Res *= times;
			return Res;
		}
		//����
		/*INLINED*/void MY_LIBRARY operator*=(const LargeUnsigned& b) noexcept {
			LargeUnsigned This(*this, true);
			this->~LargeUnsigned();
			for (auto& OprtPtr = b.LinkedList.begin(); OprtPtr != nullptr; ++OprtPtr) {
				LargeInteger::LongCmpt<typename LargeInteger::StdCmptTraits<Data>>::LineIterator<typename LargeInteger::StdCmptTraits<Data>::Multiply, decltype(this->LinkedList.begin()), Data> temp(*OprtPtr, This.LinkedList.begin());
				LargeInteger::LongCmpt<typename LargeInteger::StdCmptTraits<Data>>::AppositionComputeTo<typename LargeInteger::StdCmptTraits<Data>::Add, decltype(this->LinkedList.begin()), Data>(temp, this->LinkedList.begin());
				This <<= 1;
			}
		}
		//����
		/*INLINED*/LargeUnsigned MY_LIBRARY operator*(const LargeUnsigned& b)const noexcept {
			LargeUnsigned Res(*this, true);
			Res *= b;
			return Res;
		}
		//����
		INLINED void MY_LIBRARY operator-=(const LargeUnsigned& b) noexcept {
			*this += (-b);
		}
		//����LinkedList������
		INLINED LargeUnsigned MY_LIBRARY operator-(
			)const noexcept {
			LargeUnsigned res(*this);
			res.data = !res.data;
			return res;
		}
		//����LinkedList����+=
		INLINED void MY_LIBRARY operator+=(const LargeUnsigned& that) noexcept {
			if (that.next == nullptr)
			{
				return;
			}
			if (this->next == nullptr)
			{
				*this = LargeUnsigned(that, true);
				return;
			}
			if ((this->data > 0 && that.data > 0) || (this->data == 0 && that.data == 0))
			{
				LargeInteger::LongCmpt::AppositionComputeTo<typename LargeInteger::LongCmpt::StdCmptTraits<Data>::Add, LinkedList*, Data, LLComputeTraits<LinkedList, radix_t, Radix>>(that.next, this->next);
			}
			else {
				LargeInteger::Compare Cmpr = LargeInteger::LongCmpt::CompareTo<LinkedList*, Data, LLComputeTraits<LinkedList, radix_t, Radix>>(this->next, that.next);
				if (Cmpr == LargeInteger::Compare::Equal)
				{
					this->destruct();
				}
				if (Cmpr == LargeInteger::Compare::Larger)
				{
					LargeInteger::LongCmpt::AppositionComputeTo<typename LargeInteger::StdCmptTraits<Data>::SubtractFrom, LinkedList*, Data, LLComputeTraits<LinkedList, radix_t, Radix>>(that.next, this->next);
				}
				else
				{
					LinkedList temp(that, true);
					LargeInteger::LongCmpt::AppositionComputeTo<typename LargeInteger::StdCmptTraits<Data>::SubtractFrom, LinkedList*, Data, LLComputeTraits<LinkedList, radix_t, Radix>>(this->next, temp.next);
					*this = temp;
				}
			}
			this->Simplify();
		}
		//����LinkedList����Ӻ�
		INLINED LargeUnsigned MY_LIBRARY operator+(
			const LargeUnsigned& b//������
			)  const noexcept {
			LargeUnsigned Result(*this, true);//�洢���
			Result += b;
			return Result;
		}
		//����LinkedList�������
		/*INLINED*/LargeUnsigned MY_LIBRARY operator-(
			const LargeUnsigned& b//������
			)const noexcept {
			return (*this + (-b));
		}
		void MY_LIBRARY operator++() {
			LinkedList temp(true, 1);
			*this += temp;
			temp.destruct();
		}


		INLINED void MY_LIBRARY operator+=(const Data& that)noexcept {
			LinkedList temp(that, nullptr);
			if (that != Data(0) && this->next == nullptr)
			{
				this->data = Data(1);
				this->insert(that);
				return;
			}
			if (this->data == 0)
			{
				LargeInteger::LongCmpt::AppositionComputeTo<typename StdCmptTraits<Data>::SubtractFrom, LinkedList*, Data, LLComputeTraits<LinkedList, radix_t, Radix>>(&temp, this->next);
			}
			else
			{

				LargeInteger::LongCmpt::AppositionComputeTo<typename StdCmptTraits<Data>::Add, LinkedList*, Data, LLComputeTraits<LinkedList, radix_t, Radix>>(&temp, this->next);
			}
		}
		INLINED void MY_LIBRARY operator-=(const Data& that)noexcept {
			LinkedList temp(that, nullptr);
			if (that != Data(0) && this->next == nullptr)
			{
				this->data = Data(0);
				this->insert(that);
				return;
			}
			if (this->data > 0)
			{
				LargeInteger::LongCmpt::AppositionComputeTo<typename LargeInteger::StdCmptTraits<Data>::SubtractFrom, LinkedList*, Data, LLComputeTraits<LinkedList, radix_t, Radix>>(&temp, this->next);
			}
			else
			{

				LargeInteger::LongCmpt::AppositionComputeTo<typename LargeInteger::StdCmptTraits<Data>::Add, LinkedList*, Data, LLComputeTraits<LinkedList, radix_t, Radix>>(&temp, this->next);
			}
		}
		INLINED void MY_LIBRARY operator+(const Data& that)const noexcept {
			LinkedList res(*this, true);
			return (res += that);
		}
		INLINED void MY_LIBRARY operator-(const Data& that)const noexcept {
			LinkedList res(*this, true);
			return (res -= that);
		}



		//���Ǹ�ֵ
		/*INLINED*/void MY_LIBRARY SetValue(
			long num, const Data* data
		) noexcept {
			LinkedList* OprtPtr = this;//������ǰ����
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
		//��Ϊ��֪����ԭ�򣬶�SBLinkedList����
		/*INLINED*/void __cdecl SetValue(
			long num, Data data, ...
		) noexcept {
			LinkedList* OprtPtr = this;//������ǰ����
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

		//λ������
		//���������ƶ��Ƕ�����
		//����ʱ��Ĭ��ֵ����
		/*INLINED*/LargeUnsigned& operator<<=(
			unsigned int bits) noexcept {
			for (unsigned int i = 0; i < bits; i++)
			{
				this->insert();
			}
			return *this;
		}

		bool MY_LIBRARY operator<(const LargeUnsigned& that)const noexcept {
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
				if (LargeInteger::LongCmpt::CompareTo<LinkedList*, Data, LLComputeTraits<LinkedList, radix_t, Radix>>(this->next, that.next) == LongCmpt::Compare::Smaller)
					return true;
				else return false;
			}
			else
			{
				if (LargeInteger::LongCmpt::CompareTo<LinkedList*, Data, LLComputeTraits<LinkedList, radix_t, Radix>>(that.next, this->next) == LongCmpt::Compare::Smaller)
					return true;
				else return false;
			}
		}
		bool MY_LIBRARY operator>(const LargeUnsigned& that)const noexcept {
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
				if (LargeInteger::LongCmpt::CompareTo<LinkedList*, Data, LLComputeTraits<LinkedList, radix_t, Radix>>(this->next, that.next) == LongCmpt::Larger)
					return true;
				else return false;
			}
			else
			{
				if (LargeInteger::LongCmpt::CompareTo<LinkedList*, Data, LLComputeTraits<LinkedList, radix_t, Radix>>(that.next, this->next) == LongCmpt::Larger)
					return true;
				else return false;
			}
		}
		bool MY_LIBRARY operator<=(const LargeUnsigned& that)const noexcept {
			return !(*this > that);
		}
		bool MY_LIBRARY operator>=(const LargeUnsigned& that)const noexcept {
			return !(*this < that);
		}
		void MY_LIBRARY operator%=(const LargeUnsigned& that)noexcept {
			if (this->next != nullptr && that.next != nullptr)
			{
				LargeInteger::LongCmpt::DivideInto<LinkedList*, Data, LLComputeTraits<LinkedList, radix_t, Radix>>(that.next, this->next);
			}
			else return;
			this->Simplify();
		}
		void MY_LIBRARY operator/=(const LargeUnsigned& that)noexcept {
			if (this->next != nullptr && that.next != nullptr)
			{
				LargeUnsigned Res(true);
				LargeInteger::LongCmpt::DivideInto<LinkedList, LinkedList*, Data, LLComputeTraits<LinkedList, radix_t, Radix>>(Res, that.next, this->next);
				this->destruct();
				this->next = Res.next;
			}
			else return;
			this->Simplify();
		}

		//λ������
		//���������ƶ��Ƕ�����
		//����ʱ��һλ����
		/*INLINED*/LargeUnsigned& operator>>=(unsigned int bits) noexcept {
			for (unsigned int i = 0; i < bits; i++)
			{
				this->cut();
			}
			return *this;
		}


		//���Ǹ�ֵ
		/*INLINED*/LargeUnsigned& MY_LIBRARY operator=(
			long value
			) noexcept {
			if constexpr (Radix == radix_t(1))
			{
				this->data = (Data)value;
			}
			else
			{
				LinkedList* OprtPtr = this;//������ǰ����
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


		//��ȡ�洢��ֵ
		//�������
		/*INLINED*/long long MY_LIBRARY GetValue()const noexcept {
			long long value = 0;
			long n = 0;
			auto OprtPtr = this->LinkedList.begin();
			if (OprtPtr == nullptr)
			{
				return 0;
			}
			while (true)
			{
				if (OprtPtr + 1 != nullptr) {
					++OprtPtr;
					value += ((unsigned long long)((*OprtPtr)())) * Power(radix, n);
					n++;
				}
				else
				{
					break;
				}
			}
			return (value);
		}

		~LargeUnsigned(){
		}
	};
	template<typename LL, auto radix>
	class LargeSigned:public LargeUnsigned<LL, radix>
	{
	public:
		template<typename val>LargeSigned(val Val)noexcept
			:PosSign(Val > 0), LargeUnsigned<LL, radix>(ABS(val)) {}

		template<typename val>LargeSigned(bool Pos, val Val)noexcept
			:PosSign(Pos), LargeUnsigned<LL, radix>(Val) {
			static_assert(Val >= 0);
		}

		~LargeSigned()
		{
		}

	private:
		bool PosSign;
	};

}