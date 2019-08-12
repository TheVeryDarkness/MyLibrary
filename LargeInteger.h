#pragma once

#include "Shared.h"
#include "Statistics.h"
#include "ComputeTemplate.h"
#include <cassert>

namespace LargeInteger {

	template<typename Type, typename BaseType>
	//简单输出到控制台窗口
	//需要用户补换行
	INLINED void MY_LIBRARY SinglePrint(
		const Type& that,
		std::ostream& out = std::cout,
		bool ShowComma = true,
		unsigned MinLength = 0,
		BaseType base = 10
	) noexcept {
		if (that.next != nullptr)
		{
			SinglePrint(*that.next, out, ShowComma, MinLength, base);
			out << ((ShowComma) ? "," : "");
			char* c = DBG_NEW char[MinLength + 1ULL]();
			assert(base < BaseType(INT_MAX));
			std::to_chars_result rs = std::to_chars(c, &(c[MinLength]), that.data(), base);

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

	template<typename LL, auto radix>
	class LargeUnsigned:protected LL
	{
	protected:
		using radix_t=decltype(radix);
		using Data=Num<radix_t, radix>;
	public:
		template<typename val>
		explicit MY_LIBRARY LargeUnsigned(val Val)noexcept {
			LargeInteger::LongCmpt<StdCmptTraits<Num<radix_t, radix>>>::LayerIterator<StdCmptTraits<Num<radix_t, radix>>::Devide> it;
			for (auto& i = this->begin();; ++i)
			{
				i = Val % radix;
				if ((Val /= radix) == static_cast<val>(0)) {
					break;
				}
			}
			assert(Val == static_cast<val>(0));
			return;
		}
		static constexpr LargeUnsigned MY_LIBRARY Copy(const LargeUnsigned& that)noexcept {
			LargeUnsigned This(that);
			for (auto& i : that) {
				auto& j = This.begin();
				*j = i;
			}
			return This;
		}

		constexpr void MY_LIBRARY destruct()noexcept {
			this->destruct();
		}

		//二进制输出到控制台窗口
		/*INLINED*/friend std::ostream& MY_LIBRARY operator<<(
			std::ostream& out,
			const LargeUnsigned& l
		) noexcept {
			return _Print<LargeUnsigned, radix>(l, out);
		}
		template<typename Type, auto Radix = 0>
		//二进制输出到控制台窗口
		//不再自动换行
		static /*INLINED*/std::ostream & MY_LIBRARY _Print(
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
				LargeInteger::SinglePrint(*(that.next), out, false, 2 * sizeof(Radix), 16);
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
				LargeInteger::SinglePrint(*(that.next), out, temp, Length, OutBase);
			}

			out << std::setbase(10);
			return out;
		}
		INLINED void MY_LIBRARY Print(std::ostream& o = std::cout) const noexcept{
			_Print<LL, radix>(static_cast<LL>(*this), o);
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
			LargeInteger::LongCmpt<typename LargeInteger::StdCmptTraits<typename radix_t>>::MultiplyTo(times, this->next);
		}
		//重载
		/*INLINED*/LargeUnsigned MY_LIBRARY operator*(Data times)const noexcept {
			LargeUnsigned Res(*this, true);
			Res *= times;
			return Res;
		}
		//重载
		/*INLINED*/void MY_LIBRARY operator*=(const LargeUnsigned& b) noexcept {
			LargeUnsigned This(b);
			this->~LargeUnsigned();
			for (auto& OprtPtr = b.begin(); OprtPtr != nullptr; ++OprtPtr) {
				LargeInteger::LongCmpt<typename LargeInteger::StdCmptTraits<Data>>::LineIterator<typename LargeInteger::StdCmptTraits<Data>::Multiply, decltype(this->begin()), Data> temp(*OprtPtr, This.begin());
				LargeInteger::LongCmpt<typename LargeInteger::StdCmptTraits<Data>>::AddTo(temp, this->begin());
				This <<= 1;
			}
			
		}
		//重载
		/*INLINED*/LargeUnsigned MY_LIBRARY operator*(const LargeUnsigned& b)const noexcept {
			LargeUnsigned Res(*this, true);
			Res *= b;
			return Res;
		}
		//重载
		INLINED void MY_LIBRARY operator-=(const LargeUnsigned& b) noexcept {
			*this += (-b);
		}
		//重载LinkedList链表负号
		INLINED LargeUnsigned MY_LIBRARY operator-(
			)const noexcept {
			LargeUnsigned res(*this);
			res.data = !res.data;
			return res;
		}
		//重载LinkedList链表+=
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
				LargeInteger::LongCmpt::AppositionComputeTo<typename LargeInteger::LongCmpt::StdCmptTraits<Data>::Add, LinkedList*, Data, LargeInteger::StdCmptTraits<radix_t>>(that.next, this->next);
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
		//重载LinkedList链表加号
		INLINED LargeUnsigned MY_LIBRARY operator+(
			const LargeUnsigned& b//操作数
			)  const noexcept {
			LargeUnsigned Result(*this, true);//存储结果
			Result += b;
			return Result;
		}
		//重载LinkedList链表减号
		/*INLINED*/LargeUnsigned MY_LIBRARY operator-(
			const LargeUnsigned& b//操作数
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



		//覆盖赋值
		/*INLINED*/void MY_LIBRARY SetValue(
			long num, const Data* data
		) noexcept {
			LinkedList* OprtPtr = this;//操作当前对象
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
		//因为不知名的原因，对SBLinkedList禁用
		/*INLINED*/void __cdecl SetValue(
			long num, Data data, ...
		) noexcept {
			LinkedList* OprtPtr = this;//操作当前对象
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

		//位移运算
		//按独立进制而非二进制
		//左移时用默认值补齐
		/*INLINED*/LargeUnsigned& operator<<=(
			unsigned int bits) noexcept {
			for (unsigned int i = 0; i < bits; i++)
			{
				this->insert();
			}
			return *this;
		}
		bool MY_LIBRARY operator==(const LargeUnsigned& that)const noexcept {
			return LL::operator==(that);
		}
		bool MY_LIBRARY operator!=(const LargeUnsigned& that)const noexcept {
			return LL::operator!=(that);
		}
		bool MY_LIBRARY operator==(const Data& that)const noexcept {
			LargeUnsigned T(Data);
			return LL::operator==(T);
		}
		bool MY_LIBRARY operator!=(const Data& that)const noexcept {
			LargeUnsigned T(Data);
			return LL::operator!=(T);
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
				if (LargeInteger::LongCmpt::CompareTo<LinkedList*, Data, LLComputeTraits<LinkedList, radix_t, Radix>>(this->next, that.next) == LargeInteger::Compare::Smaller)
					return true;
				else return false;
			}
			else
			{
				if (LargeInteger::LongCmpt::CompareTo<LinkedList*, Data, LLComputeTraits<LinkedList, radix_t, Radix>>(that.next, this->next) == LargeInteger::Compare::Smaller)
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
		bool MY_LIBRARY operator<(const Data& that)const noexcept {
			LargeUnsigned T(that);
			return (*this < T);
		}
		bool MY_LIBRARY operator>(const Data& that)const noexcept {
			LargeUnsigned T(that);
			return (*this > T);
		}
		bool MY_LIBRARY operator<=(const Data& that)const noexcept {
			LargeUnsigned T(that);
			return (*this <= T);
		}
		bool MY_LIBRARY operator>=(const Data& that)const noexcept {
			LargeUnsigned T(that);
			return (*this >= T);
		}
		void MY_LIBRARY operator%=(const LargeUnsigned& that)noexcept {
			if (this->next != nullptr && that.next != nullptr)
			{
				LargeInteger::LongCmpt<StdCmptTraits<Data>>::DivideInto(that.next, this->next);
			}
			else return;
			this->Simplify();
		}
		void MY_LIBRARY operator/=(const LargeUnsigned& that)noexcept {
			if (this->next != nullptr && that.next != nullptr)
			{
				LargeUnsigned Res(true);
				LargeInteger::LongCmpt<StdCmptTraits<Data>>::DivideInto(Res, that.next, this->next);
				this->destruct();
				this->next = Res.next;
			}
			else return;
			this->Simplify();
		}

		//位移运算
		//按独立进制而非二进制
		//右移时第一位销毁
		/*INLINED*/LargeUnsigned& operator>>=(unsigned int bits) noexcept {
			for (unsigned int i = 0; i < bits; i++)
			{
				this->cut();
			}
			return *this;
		}


		//覆盖赋值
		/*INLINED*/LargeUnsigned& MY_LIBRARY operator=(
			long value
			) noexcept {
			if constexpr (Radix == radix_t(1))
			{
				this->data = (Data)value;
			}
			else
			{
				LinkedList* OprtPtr = this;//操作当前对象
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


		//获取存储的值
		//可能溢出
		/*INLINED*/long long MY_LIBRARY GetValue()const noexcept {
			long long value = 0;
			long n = 0;
			auto OprtPtr = this->begin();
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
	class LargeSigned:protected LargeUnsigned<LL, radix>
	{
		friend class Q;
	public:
		template<typename val> explicit MY_LIBRARY LargeSigned(val Val)noexcept
			:PosSign(Val > 0), LargeUnsigned<LL, radix>(ABS(val)) {}
		template<typename val> explicit MY_LIBRARY LargeSigned(bool Pos, val Val)noexcept
			:PosSign(Pos), LargeUnsigned<LL, radix>(Val) {
			assert(Val >= 0);
		}
		explicit MY_LIBRARY LargeSigned(bool sign, LargeUnsigned<LL,radix> uns)noexcept
			:PosSign(sign), LargeUnsigned<LL, radix>(uns) {
			assert(Val >= 0);
		}

		static constexpr LargeSigned MY_LIBRARY Copy(const LargeSigned& that)noexcept {
			LargeSigned This(that.PosSign, LargeUnsigned<LL, radix>::Copy(that));
			return This;
		}

		constexpr void MY_LIBRARY destruct()noexcept {
			this->destruct();
		}

		bool MY_LIBRARY operator==(const LargeSigned& that)const noexcept {
			return (LargeUnsigned<LL, radix>::operator==(that) && (this->PosSign == that.PosSign));
		}
		bool MY_LIBRARY operator!=(const LargeSigned& that)const noexcept {
			return (LargeUnsigned<LL, radix>::operator!=(that) || (this->PosSign != that.PosSign));
		}
		bool MY_LIBRARY operator>(const LargeSigned& that)const noexcept {
			return (LargeUnsigned<LL, radix>::operator>(that));
		}
		bool MY_LIBRARY operator<(const LargeSigned& that)const noexcept {
			return (LargeUnsigned<LL, radix>::operator<(that));
		}
		bool MY_LIBRARY operator>=(const LargeSigned& that)const noexcept {
			return (LargeUnsigned<LL, radix>::operator>=(that));
		}
		bool MY_LIBRARY operator<=(const LargeSigned& that)const noexcept {
			return (LargeUnsigned<LL, radix>::operator<=(that));
		}
		bool MY_LIBRARY operator==(const Data& that)const noexcept {
			return (LargeUnsigned<LL, radix>::operator==(that) && (this->PosSign == (that > 0)));
		}
		bool MY_LIBRARY operator!=(const Data& that)const noexcept {
			return (LargeUnsigned<LL, radix>::operator!=(that) || (this->PosSign != (that > 0)));
		}
		bool MY_LIBRARY operator>(const Data& that)const noexcept {
			return (LargeUnsigned<LL, radix>::operator>(that));
		}
		bool MY_LIBRARY operator<(const Data& that)const noexcept {
			return (LargeUnsigned<LL, radix>::operator<(that));
		}
		bool MY_LIBRARY operator>=(const Data& that)const noexcept {
			return (LargeUnsigned<LL, radix>::operator>=(that));
		}
		bool MY_LIBRARY operator<=(const Data& that)const noexcept {
			return (LargeUnsigned<LL, radix>::operator<=(that));
		}

		template<typename Val >
		LargeSigned& MY_LIBRARY operator=(Val val) noexcept {
			LargeUnsigned<LL, radix>::operator=(val);
			return *this;
		}
		LargeSigned& MY_LIBRARY operator+=(const LargeSigned& that) noexcept {
			LargeUnsigned<LL, radix>::operator+=(that);
			return *this;
		}
		LargeSigned MY_LIBRARY operator+(const LargeSigned& that) const noexcept {
			LargeSigned temp = Copy(*this);
			temp += thst;
			return temp;
		}
		LargeSigned& MY_LIBRARY operator-=(const LargeSigned& that) noexcept {
			LargeUnsigned<LL, radix>::operator-=(that);
			return *this;
		}
		LargeSigned MY_LIBRARY operator-(const LargeSigned& that) const noexcept {
			LargeSigned temp = Copy(*this);
			temp -= thst;
			return temp;
		}
		LargeSigned& MY_LIBRARY operator*=(const LargeSigned& that) noexcept {
			LargeUnsigned<LL, radix>::operator*=(that);
			return *this;
		}
		LargeSigned MY_LIBRARY operator*(const LargeSigned& that) const noexcept {
			LargeSigned temp = Copy(*this);
			temp *= that;
			return temp;
		}
		LargeSigned& MY_LIBRARY operator*=(const Data& that) noexcept {
			LargeUnsigned<LL, radix>::operator*=(that);
			return *this;
		}
		LargeSigned MY_LIBRARY operator*(const Data& that) const noexcept {
			LargeSigned temp = Copy(*this);
			temp *= that;
			return temp;
		}
		LargeSigned& MY_LIBRARY operator%=(const LargeSigned& that) noexcept {
			LargeUnsigned<LL, radix>::operator%=(that);
			return *this;
		}
		LargeSigned MY_LIBRARY operator%(const LargeSigned& that) const noexcept {
			LargeSigned temp = Copy(*this);
			temp %= that;
			return temp;
		}
		LargeSigned& MY_LIBRARY operator/=(const LargeSigned& that) noexcept {
			LargeUnsigned<LL, radix>::operator/=(that);
			return *this;
		}
		LargeSigned MY_LIBRARY operator/(const LargeSigned& that) const noexcept {
			LargeSigned temp = Copy(*this);
			temp /= that;
			return temp;
		}
		void MY_LIBRARY SetToContradict() noexcept {
			this->PosSign = !this->PosSign;
		}

		//获取存储的值
		//可能溢出
		/*INLINED*/long long MY_LIBRARY GetValue()const noexcept {
			return LargeUnsigned<LL, radix>::GetValue();
		}

		//二进制输出到控制台窗口
		/*INLINED*/friend std::ostream& MY_LIBRARY operator<<(
			std::ostream& out,
			const LargeSigned& l
			) noexcept {
			return _Print<LargeSigned, radix>(l, out);
		}

		INLINED std::ostream& MY_LIBRARY Print(std::ostream& o = std::cout) const noexcept {
			return _Print<LL, radix>(static_cast<LL>(*this), o);
		}

		~LargeSigned()
		{
		}

	private:
		bool PosSign;
	};

}