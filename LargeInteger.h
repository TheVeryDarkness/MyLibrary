#pragma once

#include "Shared.h"
#include "Statistics.h"
#include "ComputeTemplate.h"
#include <iostream>
#include <cassert>

namespace LargeInteger {
	template<typename LL, auto radix> class LargeUnsigned;
	template<typename LL, auto radix> class LargeSigned;
	template<auto radix>
	class LLCmptTraits :public LargeInteger::StdCmptTraits<LargeInteger::Num<decltype(radix), radix>> {
	public:
		using Data=LargeInteger::Num<decltype(radix), radix>;
		MY_LIBRARY LLCmptTraits() = delete;
		MY_LIBRARY ~LLCmptTraits() = delete;

		using Multiply=typename Data::Multiply;
	};

	template<typename Iter, typename BaseType>
	//简单输出到控制台窗口
	//需要用户补换行
	INLINED void MY_LIBRARY SinglePrint(
		const Iter& that,
		std::ostream& out = std::cout,
		bool ShowComma = true,
		unsigned MinLength = 0,
		BaseType base = 10
	) noexcept {
		if (that + 1 != nullptr) {
			SinglePrint(that + 1, out, ShowComma, MinLength, base);
			out << ((ShowComma) ? "," : "");
			char* c = DBG_NEW char[MinLength + 1ULL]();
			assert(base < BaseType(INT_MAX));
			std::to_chars_result rs = std::to_chars(c, &(c[MinLength]), (*that)(), base);

			std::string str = c;
			delete[] c;
			if (str.length() < MinLength) {
				std::string nStr;
				for (size_t index = MinLength - str.length(); index > 0; index--) {
					nStr.push_back('0');
				}
				nStr += str;
				out << nStr;
			}
			else out << str;
		}
		else {
			out << *that;
		}
		return;
	}

	template<typename LL, auto radix>
	class LargeUnsigned :protected LL {
	protected:
		using radix_t=decltype(radix);
		using Data=Num<radix_t, radix>;
	public:
		static constexpr radix_t getRadix()noexcept { return radix; }
		constexpr INLINED auto begin() noexcept {
			return this->LL::begin();
		}
		constexpr INLINED auto begin()const noexcept {
			return this->LL::begin();
		}
		constexpr INLINED auto cbegin()const noexcept {
			return this->LL::cbegin();
		}
		constexpr INLINED auto end() noexcept {
			return this->LL::end();
		}
		constexpr INLINED auto end() const noexcept {
			return this->LL::end();
		}
		constexpr INLINED auto cend() const noexcept {
			return this->LL::cend();
		}
		template<typename val>
		explicit MY_LIBRARY LargeUnsigned(val Val)noexcept :LL(0) {
			static_assert(std::is_integral_v<val>);
			static_assert(radix != radix_t(1));
			static_assert(!std::is_same_v<val, bool>, "Never use bool type");
			typename LongCmpt<StdCmptTraits<val>>::template LayerIterator<StdCmptTraits<val>::template Divide<radix>, val> it(Val);
			for (auto index = this->begin(); !!it; ) {
				*index = Data(*it);
				++it;
				if (!!it) {
					++index;
				}
			}
			return;
		}
		static constexpr LargeUnsigned MY_LIBRARY Copy(const LargeUnsigned& that)noexcept {
			LargeUnsigned This(0);
			auto j = This.begin();
			for (auto index = that.begin(); index != that.end(); ++index) {
				*j = *index;
				if (index + 1 != that.end()) {
					++j;
				}
			}
			return This;
		}

		constexpr void MY_LIBRARY destruct()noexcept {
			this->LL::destruct();
		}

		//二进制输出到控制台窗口
		/*INLINED*/friend std::ostream& MY_LIBRARY operator<<(
			std::ostream& out,
			const LargeUnsigned& l
			) noexcept {
			return _Print<LargeUnsigned, radix>(l, out);
		}
		template<typename Iter, auto Radix = 0>
		//二进制输出到控制台窗口
		//不再自动换行
		static /*INLINED*/std::ostream & MY_LIBRARY _Print(
			const Iter & that,
			std::ostream & out = std::cout
		) noexcept {
			if (that == 0) {
				out << "0";
				return out;
			}
			if (Radix == static_cast<decltype(Radix)>(0)) {
				out << "0x"
					<< std::setbase(16);
				LargeInteger::SinglePrint(that, out, false, 2 * sizeof(Radix), 16);
			}
			else {
				bool temp = false;
				unsigned int Length = 0;
				Length = GetPowerTimes(Radix, 10);
				decltype(Radix) OutBase = 0;
				if (Length == 0) {
					Length = GetPowerTimes(Radix, 16);
					if (Length == 0) {
						Length = GetPowerTimes(Radix, 8);
						if (Length == 0) {
							Length = GetPowerTimes(Radix, 2);
							if (Length == 0) {
								Length = 1;
								OutBase = Radix;
								out << "(Base:"
									<< Radix
									<< ")";
								temp = true;
							}
							else {
								OutBase = 2;
								out << "0b" << std::setbase(2);
							}
						}
						else {
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
				else {
					OutBase = 10;
				}
				LargeInteger::SinglePrint(that, out, temp, Length, OutBase);
			}

			out << std::setbase(10);
			return out;
		}
		INLINED std::ostream& MY_LIBRARY Print(std::ostream& o = std::cout) const noexcept {
			return _Print<LL, radix>(static_cast<LL>(*this), o);
		}
		template<typename Int>
		INLINED void MY_LIBRARY operator*=(const Int& that) noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeUnsigned temp(that);
			*this *= temp;
			temp.destruct();
		}
		template<typename Int>
		/*INLINED*/LargeUnsigned MY_LIBRARY operator*(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeUnsigned temp(that);
			*this *= temp;
			temp.destruct();
		}
		constexpr void MY_LIBRARY Swap(LargeUnsigned& that)noexcept {
			{
				LL* temp = that.next;
				that.next = temp;
				this->next = temp;
			}
			{
				Data temp = that.data;
				that.data = this->data;
				this->data = that.data;
			}
		}
		//重载
		/*INLINED*/void MY_LIBRARY operator*=(const LargeUnsigned& b) noexcept {
			LargeUnsigned This(*this);
			this->next = nullptr;
			this->data = Data(radix_t(0));
			for (auto OprtPtr = b.begin(); OprtPtr != nullptr; ++OprtPtr) {
				typename LargeInteger::LongCmpt<typename LargeInteger::LLCmptTraits<radix>>::template LineIterator<typename LargeInteger::LLCmptTraits<radix>::Multiply, decltype(This.cbegin()), Data> temp(*OprtPtr, This.cbegin());
				LargeInteger::LongCmpt<typename LargeInteger::LLCmptTraits<radix>>::AddTo(temp, this->begin());
				This <<= 1;
			}
			This.release();
		}
		//重载
		/*INLINED*/LargeUnsigned MY_LIBRARY operator*(const LargeUnsigned& b)const noexcept {
			LargeUnsigned Res(*this, true);
			Res *= b;
			return Res;
		}
		//重载
		INLINED void MY_LIBRARY operator-=(const LargeUnsigned& that) noexcept {
			if (that == 0) {
				return;
			}
			if (*this == 0) {
				*this = Copy(that);
				return;
			}
			LargeInteger::LongCmpt<LLCmptTraits<radix>>::SubtractFrom(that.begin(), this->begin());
			this->LL::Simplify();
		}
		//重载LinkedList链表负号
		INLINED LargeUnsigned MY_LIBRARY operator-(
			)const noexcept {
			LargeUnsigned res = Copy(*this);
			return res;
		}
		//重载LinkedList链表+=
		INLINED void MY_LIBRARY operator+=(const LargeUnsigned& that) noexcept {
			if (that == 0) {
				return;
			}
			if (*this == 0) {
				*this = Copy(that);
				return;
			}
			LargeInteger::LongCmpt<LLCmptTraits<radix>>::AddTo(that.begin(), this->begin());
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
			return (*this -= b);
		}
		void MY_LIBRARY operator++() {
			*this += 1;
		}
		void MY_LIBRARY operator--() {
			*this -= 1;
		}


		template<typename Int>
		INLINED void MY_LIBRARY operator+=(const Int& that)noexcept {
			static_assert(std::is_integral_v<Int>, "integral type required.");
			LargeUnsigned temp(that);
			*this += temp;
			temp.destruct();
		}
		template<typename Int>
		INLINED void MY_LIBRARY operator-=(const Int& that)noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeUnsigned temp(that);
			*this -= temp;
			temp.destruct();
		}
		template<typename Int>
		INLINED LargeUnsigned MY_LIBRARY operator+(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeUnsigned res = Copy(*this);
			res += that;
			return res;
		}
		template<typename Int>
		INLINED LargeUnsigned MY_LIBRARY operator-(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeUnsigned res = Copy(*this);
			res -= that;
			return res;
		}





		//位移运算
		//按独立进制而非二进制
		//左移时用默认值补齐
		/*INLINED*/LargeUnsigned& operator<<=(
			unsigned int bits) noexcept {
			for (unsigned int index = 0; index < bits; index++) {
				this->insert(this->data);
				this->data = 0;
			}
			return *this;
		}
		bool MY_LIBRARY operator==(const LargeUnsigned& that)const noexcept {
			return LL::operator==(that);
		}
		bool MY_LIBRARY operator!=(const LargeUnsigned& that)const noexcept {
			return LL::operator!=(that);
		}
		template<typename Int>
		bool MY_LIBRARY operator==(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			if (that == Data(0)) {
				return (this->LL::next == nullptr) && (this->LL::data == Data(0));
			}
			typename LongCmpt<LLCmptTraits<radix>>::template LayerIterator<LLCmptTraits<radix>::template Divide<radix>, Data> it(that);
			return (LargeInteger::LongCmpt<LLCmptTraits<radix>>::CompareTo(this->begin(), it) == Compare::Equal);
		}
		template<typename Int>
		bool MY_LIBRARY operator!=(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			return !(*this == that);
		}
		bool MY_LIBRARY operator<(const LargeUnsigned& that)const noexcept {
			return (LargeInteger::LongCmpt<LLCmptTraits<radix>>::CompareTo(this->begin(), that.begin()) == Compare::Smaller);
		}
		bool MY_LIBRARY operator>(const LargeUnsigned& that)const noexcept {
			return(LargeInteger::LongCmpt<LLCmptTraits<radix>>::CompareTo(this->begin(), that.begin()) == Compare::Larger);
		}
		bool MY_LIBRARY operator<=(const LargeUnsigned& that)const noexcept {
			return !(*this > that);
		}
		bool MY_LIBRARY operator>=(const LargeUnsigned& that)const noexcept {
			return !(*this < that);
		}
		template<typename Int>
		bool MY_LIBRARY operator<(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeUnsigned temp(that);
			bool&& Res = (*this < temp);
			temp.destruct();
			return Res;
		}
		template<typename Int>
		bool MY_LIBRARY operator>(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeUnsigned temp(that);
			bool&& Res = (*this > temp);
			temp.destruct();
			return Res;
		}
		template<typename Int>
		bool MY_LIBRARY operator<=(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeUnsigned T(that);
			return (*this <= T);
		}
		template<typename Int>
		bool MY_LIBRARY operator>=(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeUnsigned T = LargeUnsigned(that);
			return (*this >= T);
		}
		template<typename Iter>
		class Sim {
		public:
			MY_LIBRARY Sim(Iter it)noexcept {
				it->Simplify();
			}

			MY_LIBRARY ~Sim() = default;
		};
		void MY_LIBRARY operator%=(const LargeUnsigned& that)noexcept {
			assert(that != 0);

			LargeInteger::LongCmpt<LLCmptTraits<radix>>::template DivideInto<Sim<decltype(this->begin())>, decltype(that.begin()), decltype(this->begin())>(that.begin(), this->begin());
			this->Simplify();
		}
		void MY_LIBRARY operator/=(const LargeUnsigned& that)noexcept {
			assert(that != 0);
			LargeUnsigned Res(0);
			LargeInteger::LongCmpt<LLCmptTraits<radix>>::template DivideInto<Sim<decltype(this->begin())>, decltype(Res), decltype(that.begin()), decltype(this->begin())>(Res, that.begin(), this->begin());
			*this = Res;
			this->Simplify();
		}
		template<typename Int>
		void MY_LIBRARY operator%=(const Int& that)noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeUnsigned temp(that);
			*this %= temp;
			temp.destruct();
		}
		template<typename Int>
		void MY_LIBRARY operator/=(const Int& that)noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeUnsigned temp(that);
			*this /= temp;
			temp.destruct();
		}

		//位移运算
		//按独立进制而非二进制
		//右移时第一位销毁
		/*INLINED*/LargeUnsigned& operator>>=(unsigned int bits) noexcept {
			for (unsigned int index = 0; index < bits; index++) {
				this->data = this->LL::pop();
			}
			return *this;
		}


		//覆盖赋值
		template<typename Int>
		/*INLINED*/LargeUnsigned& MY_LIBRARY operator=(
			Int Val
			) noexcept {
			if (Val == 0) {
				this->destruct();
			}
			typename LongCmpt<StdCmptTraits<Int>>::template LayerIterator<StdCmptTraits<Int>::template Divide<radix>, Int> it(Val);
			for (auto index = this->begin(); !!it; ) {
				*index = Data(*it);
				++it;
				if (!!it) {
					++index;
				}
			}
			return *this;
		}


		//获取存储的值
		//可能溢出
		template<typename Val=__int64>
		/*INLINED*/Val MY_LIBRARY GetValue()const noexcept {
			Val value = 0;
			unsigned __int64 n = 0;
			auto OprtPtr = this->begin();
			if (OprtPtr == nullptr) {
				return 0;
			}
			while (true) {
				if (OprtPtr + 1 != nullptr) {
					++OprtPtr;
					value += ((Val)((*OprtPtr)())) * Power(static_cast<Val>(radix), n);
					n++;
				}
				else {
					break;
				}
			}
			return (value);
		}

		~LargeUnsigned() { }
	};
	template<typename LL, auto radix>
	class LargeSigned :protected LargeUnsigned<LL, radix> {
		using radix_t=decltype(radix);
		using Data=Num<radix_t, radix>;
		friend class Q;
	public:
		static constexpr radix_t getRadix()noexcept { return radix; }
		constexpr INLINED auto begin() noexcept {
			return this->LargeUnsigned<LL, radix>::begin();
		}
		constexpr INLINED auto begin()const noexcept {
			return this->LargeUnsigned<LL, radix>::begin();
		}
		constexpr INLINED auto cbegin()const noexcept {
			return this->LargeUnsigned<LL, radix>::cbegin();
		}
		constexpr INLINED auto end() noexcept {
			return this->LargeUnsigned<LL, radix>::end();
		}
		constexpr INLINED auto end() const noexcept {
			return this->LargeUnsigned<LL, radix>::end();
		}
		constexpr INLINED auto cend() const noexcept {
			return this->LargeUnsigned<LL, radix>::cend();
		}
		template<typename val> explicit MY_LIBRARY LargeSigned(val Val)noexcept
			:PosSign(Val > 0), LargeUnsigned<LL, radix>(ABS(Val)) { }
		template<typename val> explicit MY_LIBRARY LargeSigned(bool Pos, val Val)noexcept
			:PosSign(Pos), LargeUnsigned<LL, radix>(Val) {
			assert(Val >= 0);
		}
		explicit MY_LIBRARY LargeSigned(bool sign, LargeUnsigned<LL, radix> uns)noexcept
			:PosSign(sign), LargeUnsigned<LL, radix>(uns) { }

		static constexpr LargeSigned MY_LIBRARY Copy(const LargeSigned& that)noexcept {
			LargeSigned This(that.PosSign, LargeUnsigned<LL, radix>::Copy(that));
			return This;
		}
		constexpr void MY_LIBRARY _Swap(LargeSigned& that)noexcept {
			this->LargeUnsigned<LL, radix>::Swap(that);
		}
		constexpr void MY_LIBRARY Swap(LargeSigned& that)noexcept {
			this->_Swap(that);
			bool temp = that.PosSign;
			that.PosSign = this->PosSign;
			this->PosSign = temp;
		}
		constexpr void MY_LIBRARY destruct() noexcept {
			this->LL::destruct();
		}

		bool MY_LIBRARY operator==(const LargeSigned& that)const noexcept {
			return (this->LargeUnsigned<LL, radix>::operator==(*static_cast<const LargeUnsigned<LL, radix>*>(&that)) && (this->PosSign == that.PosSign));
		}
		bool MY_LIBRARY operator!=(const LargeSigned& that)const noexcept {
			return !(*this == that);
		}
		bool MY_LIBRARY operator<(const LargeSigned& that)const noexcept {
			if ((!this->PosSign) && that.PosSign) {
				return true;
			}
			if (this->PosSign && !(that.PosSign)) {
				return false;
			}
			if (this->PosSign && that.PosSign) {
				return this->LargeUnsigned<LL, radix>::operator<(*static_cast<const LargeUnsigned<LL, radix>*>(&that));
			}
			else {
				assert((!this->PosSign) && (!that.PosSign));
				return this->LargeUnsigned<LL, radix>::operator>(*static_cast<const LargeUnsigned<LL, radix>*>(&that));
			}
		}
		bool MY_LIBRARY operator>(const LargeSigned& that)const noexcept {
			if ((!this->PosSign) && that.PosSign) {
				return false;
			}
			if (this->PosSign && !(that.PosSign)) {
				return true;
			}
			if (this->PosSign && that.PosSign) {
				return this->LargeUnsigned<LL, radix>::operator>(that);
			}
			else {
				assert((!this->PosSign) && (!that.PosSign));
				return this->LargeUnsigned<LL, radix>::operator<(that);
			}
		}
		bool MY_LIBRARY operator<=(const LargeSigned& that)const noexcept {
			return !(*this > that);
		}
		bool MY_LIBRARY operator>=(const LargeSigned& that)const noexcept {
			return !(*this < that);
		}
		template<typename Int>
		bool MY_LIBRARY operator==(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			return (this->LargeUnsigned<LL, radix>::operator==(that) && (that == 0 || (this->PosSign == (that > 0))));
		}
		template<typename Int>
		bool MY_LIBRARY operator!=(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			return !(*this == that);
		}
		template<typename Int>
		bool MY_LIBRARY operator>(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			return (this->LargeUnsigned<LL, radix>::operator>(that));
		}
		template<typename Int>
		bool MY_LIBRARY operator<(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			return (this->LargeUnsigned<LL, radix>::operator<(that));
		}
		template<typename Int>
		bool MY_LIBRARY operator>=(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			return (this->LargeUnsigned<LL, radix>::operator>=(that));
		}
		template<typename Int>
		bool MY_LIBRARY operator<=(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			return (this->LargeUnsigned<LL, radix>::operator<=(that));
		}

		template<typename Val>
		LargeSigned& MY_LIBRARY operator=(Val value) noexcept {
			if (value >= 0) {
				this->PosSign = true;
			}
			else {
				this->PosSign = false;
				value = -value;
			}
			this->LargeUnsigned<LL, radix>::operator=(value);
			return *this;
		}
		LargeSigned& MY_LIBRARY operator+=(const LargeSigned& that) noexcept {
			if ((this->PosSign && that.PosSign) || (!this->PosSign && !that.PosSign)) {
				LargeUnsigned<LL, radix>::operator+=(*static_cast<const LargeUnsigned<LL, radix>*>(&that));
			}
			else {
				LargeInteger::Compare Cmpr = LargeInteger::LongCmpt<LLCmptTraits<radix>>::CompareTo(this->begin(), that.begin());
				if (Cmpr == LargeInteger::Compare::Equal) {
					this->destruct();
				}
				if (Cmpr == LargeInteger::Compare::Larger) {
					this->LargeUnsigned<LL, radix>::operator-=(*static_cast<const LargeUnsigned<LL, radix>*>(&that));
				}
				else {
					LargeSigned temp = Copy(that);
					temp.LargeUnsigned<LL, radix>::operator-=(*static_cast<const LargeUnsigned<LL, radix>*>(this));
					*this = temp;
					//this->PosSign = !this->PosSign;
				}
			}
			return *this;
		}
		LargeSigned MY_LIBRARY operator+(const LargeSigned& that) const noexcept {
			LargeSigned temp = Copy(*this);
			temp += that;
			return temp;
		}
		LargeSigned& MY_LIBRARY operator-=(const LargeSigned& that) noexcept {
			if ((this->PosSign && !that.PosSign) || (!this->PosSign && that.PosSign)) {
				LargeUnsigned<LL, radix>::operator+=(*static_cast<const LargeUnsigned<LL, radix>*>(&that));
			}
			else {
				LargeInteger::Compare Cmpr = LargeInteger::LongCmpt<LLCmptTraits<radix>>::CompareTo(this->begin(), that.begin());
				if (Cmpr == LargeInteger::Compare::Equal) {
					this->destruct();
				}
				if (Cmpr == LargeInteger::Compare::Larger) {
					LargeInteger::LongCmpt<LLCmptTraits<radix>>::SubtractFrom(that.begin(), this->begin());
				}
				else {
					LargeSigned temp = Copy(that);
					temp.LargeUnsigned<LL, radix>::operator-=(*static_cast<const LargeUnsigned<LL, radix>*>(this));
					*this = temp;
					this->PosSign = !this->PosSign;
				}
			}
			return *this;
		}
		LargeSigned MY_LIBRARY operator-(const LargeSigned& that) const noexcept {
			LargeSigned temp = Copy(*this);
			temp -= that;
			return temp;
		}
		template<typename Int>
		LargeSigned& MY_LIBRARY operator+=(const Int& that) noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeSigned That(that);
			*this += That;
			That.destruct();
			return *this;
		}
		template<typename Int>
		LargeSigned MY_LIBRARY operator+(const Int& that) const noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeSigned temp = Copy(*this);
			temp += that;
			return temp;
		}
		template<typename Int>
		LargeSigned& MY_LIBRARY operator-=(const Int& that) noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeSigned That(that);
			*this -= That;
			That.destruct();
			return *this;
		}
		template<typename Int>
		LargeSigned MY_LIBRARY operator-(const Int& that) const noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeSigned temp = Copy(*this);
			temp -= that;
			return temp;
		}
		LargeSigned& MY_LIBRARY operator*=(const LargeSigned& that) noexcept {
			if (!that.PosSign) {
				this->PosSign = !this->PosSign;
			}
			this->LargeUnsigned<LL, radix>::operator*=(*static_cast<const LargeUnsigned<LL, radix>*>(&that));
			return *this;
		}
		LargeSigned MY_LIBRARY operator*(const LargeSigned& that) const noexcept {
			LargeSigned temp = Copy(*this);
			temp *= that;
			return temp;
		}
		template<typename Int>
		LargeSigned& MY_LIBRARY operator*=(const Int& that) noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeSigned That(that);
			*this *= That;
			That.destruct();
			return *this;
		}
		template<typename Int>
		LargeSigned MY_LIBRARY operator*(const Int& that) const noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeSigned temp = Copy(*this);
			temp *= that;
			return temp;
		}
		LargeSigned& MY_LIBRARY operator%=(const LargeSigned& that) noexcept {
			if (!that.PosSign) {
				this->PosSign = !this->PosSign;
			}
			this->LargeUnsigned<LL, radix>::operator%=(*static_cast<const LargeUnsigned<LL, radix>*>(&that));
			return *this;
		}
		LargeSigned MY_LIBRARY operator%(const LargeSigned& that) const noexcept {
			LargeSigned temp = Copy(*this);
			temp %= that;
			return temp;
		}
		LargeSigned& MY_LIBRARY operator/=(const LargeSigned& that) noexcept {
			if (!that.PosSign) {
				this->PosSign = !this->PosSign;
			}
			this->LargeUnsigned<LL, radix>::operator/=(*static_cast<const LargeUnsigned<LL, radix>*>(&that));
			return *this;
		}
		LargeSigned MY_LIBRARY operator/(const LargeSigned& that) const noexcept {
			LargeSigned temp = Copy(*this);
			temp /= that;
			return temp;
		}
		template<typename Int>
		LargeSigned& MY_LIBRARY operator%=(const Int& that) noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeSigned That(that);
			*this %= That;
			That.destruct();
			return *this;
		}
		template<typename Int>
		LargeSigned MY_LIBRARY operator%(const Int& that) const noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeSigned temp = Copy(*this);
			temp %= that;
			return temp;
		}
		template<typename Int>
		LargeSigned& MY_LIBRARY operator/=(const Int& that) noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeSigned That(that);
			*this /= That;
			That.destruct();
			return *this;
		}
		template<typename Int>
		LargeSigned MY_LIBRARY operator/(const Int& that) const noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeSigned temp = Copy(*this);
			temp /= that;
			return temp;
		}
		void MY_LIBRARY SetToContradict() noexcept {
			this->PosSign = !this->PosSign;
		}

		//获取存储的值
		//可能溢出
		template<typename val=__int64>
		/*INLINED*/val MY_LIBRARY GetValue()const noexcept {
			val&& value = LargeUnsigned<LL, radix>::GetValue<val>();
			if (!PosSign) {
				value = -value;
			}
			return value;
		}

		//二进制输出到控制台窗口
		/*INLINED*/friend std::ostream& MY_LIBRARY operator<<(
			std::ostream& out,
			const LargeSigned& l
			) noexcept {
			return l.LargeSigned::Print(out);
		}

		INLINED std::ostream& MY_LIBRARY Print(std::ostream& o = std::cout) const noexcept {
			if (!this->PosSign) {
				o << "-";
			}
			return LargeUnsigned<LL, radix>::template _Print<decltype(this->begin()), radix>(this->begin(), o);
		}

		~LargeSigned() { }

	private:
		bool PosSign;
	};

}