#pragma once

#include "Shared.h"
#include "Statistics.h"
#include "ComputeTemplate.h"
#include <iostream>
#include <cassert>

namespace LargeInteger {
	template<typename LL, auto radix> class LargeUnsigned;
	template<typename LL, auto radix> class LargeSigned;
	template<auto Radix>
	class LLCmptTraits :public LargeInteger::StdCmptTraits<decltype(Radix)> {
	public:
		using Data=decltype(Radix);
		MY_LIB LLCmptTraits() = delete;
		MY_LIB ~LLCmptTraits() = delete;
		class Add {
		public:
			MY_LIB Add()noexcept { }
			MY_LIB ~Add()noexcept { }
			std::pair<Data, Data> MY_LIB operator()(Data Carry, const Data& a, const Data& b)noexcept {
				if (Radix == 0)
					return std::pair<Data, Data>(
						a + b + Carry,
						Data(
						(Carry > 0) ?
							(((a > (static_cast<Data>(~1) - b)) || (b > static_cast<Data>(Radix - 1))) ? 1 : 0)
							:
							((a > static_cast<Data>(~b) ? 1 : 0))
						)
						);
				else return std::pair<Data, Data>(
					(a + b + Carry) % Radix,
					Data(
					(Carry > 0) ?
						(((a > static_cast<Data>((Radix - 1) - b)) || (b > static_cast<Data>(Radix - 1))) ? 1 : 0)
						:
						((a > static_cast<Data>(Radix - b) ? 1 : 0))
					)
					);
			}
		};

		class SubtractFrom {
		public:
			MY_LIB SubtractFrom()noexcept { }
			MY_LIB ~SubtractFrom()noexcept { }
			std::pair<Data, Data> MY_LIB operator()(Data Carry, const Data &a, const Data &b)noexcept {
				if constexpr (Radix == 0)
					return std::pair<Data, Data>(
						(b - a - Carry),
						Data(
						(Carry > 0) ?
							((b <= a) ? 1 : 0)
							:
							((b < a) ? 1 : 0))
						);
				else return std::pair<Data, Data>(
					(b - a - Carry) % Radix,
					Data(
					(Carry > 0) ?
						((b <= a) ? 1 : 0)
						:
						((b < a) ? 1 : 0))
					);
			}
		};
		class Multiply {
		public:
			using Num=decltype(Radix);
			using Data=Num;
			Multiply() = default;
			~Multiply() = default;

			std::pair<Num, Num> MY_LIB operator()(Num Carry, Num a, Num b)const noexcept {
				using LargeInteger::_Bytes;
				if constexpr (Radix == Data(0)) {
					_Bytes<sizeof(Data) * 2> This(a);
					This *= _Bytes<sizeof(Data) * 2>::Make_s(b);
					This += _Bytes<sizeof(Data) * 2>::Make_s(Carry);
					return std::pair<Num, Num>(Num(Data(This)), Num(Data(This >> LargeInteger::BitsPerByte * sizeof(Num))));
				}
				else {
					if constexpr (Radix > std::numeric_limits<Data>::max() / Radix) {
						_Bytes<GetMinLength(Radix) * 2> This = _Bytes<GetMinLength(Radix) * 2>::Make_s(a);
						This *= _Bytes<GetMinLength(Radix) * 2>::Make_s(b);
						This += _Bytes<GetMinLength(Radix) * 2>::Make_s(Carry);
						_Bytes<GetMinLength(Radix) * 2> radix = _Bytes<GetMinLength(Radix) * 2>::Make_s(Radix);
						_Bytes<GetMinLength(Radix) * 2> Res = This.Divide(radix);
						return std::pair<Num, Num>(
							Num(Data(This)),
							Num(Data(Res))
							);
					}
					else {
						a *= b;
						a += Carry;
						return std::pair<Num, Num>(Num(a % Radix), Num(a / Radix));
					}
				}
			}

		private:

		};
	};

	template<typename Cntnr, typename BaseType>
	//简单输出到控制台窗口
	//需要用户补换行
	INLINED void MY_LIB SinglePrint(
		const Cntnr& that,
		std::ostream& out = std::cout,
		bool ShowComma = true,
		unsigned MinLength = 0,
		BaseType base = 10
	) noexcept {
		if (that + 1 != nullptr) {
			SinglePrint(that + 1, out, ShowComma, MinLength, base);
			out << ((ShowComma) ? "," : "");
			char* c = DBG_NEW char[MinLength + static_cast<size_t>(1)]();
			assert(base < BaseType(INT_MAX));
			std::to_chars_result rs = std::to_chars(c, &(c[MinLength]), (*that), base);
			assert(rs.ec == std::errc());
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
		using Data=radix_t;

		template<typename Cntnr>
		/*INLINED*/void MY_LIB mul(Cntnr b) noexcept {
			LargeUnsigned This(*this);
			this->next = nullptr;
			this->data = Data(radix_t(0));
			for (auto OprtPtr = b; OprtPtr != nullptr; ++OprtPtr) {
				typename LargeInteger::LongCmpt<typename LargeInteger::LLCmptTraits<radix>>::template LineIterator<typename LargeInteger::LLCmptTraits<radix>::Multiply, decltype(This.cbegin()), Data> temp(*OprtPtr, This.cbegin());
				LargeInteger::LongCmpt<typename LargeInteger::LLCmptTraits<radix>>::AddTo(temp, this->begin());
				This <<= 1;
			}
			This.release();
		}
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
		constexpr INLINED auto rbegin() noexcept {
			return this->LL::rbegin();
		}
		constexpr INLINED auto rbegin()const noexcept {
			return this->LL::rbegin();
		}
		constexpr INLINED auto crbegin()const noexcept {
			return this->LL::crbegin();
		}
		constexpr INLINED auto rend() noexcept {
			return this->LL::rend();
		}
		constexpr INLINED auto rend() const noexcept {
			return this->LL::rend();
		}
		constexpr INLINED auto crend() const noexcept {
			return this->LL::crend();
		}
		template<typename val>
		explicit MY_LIB LargeUnsigned(val Val)noexcept :LL(0) {
			static_assert(std::is_integral_v<val>);
			static_assert(radix != radix_t(1));
			static_assert(!std::is_same_v<val, bool>, "Never use bool type");
			typename LongCmpt<StdCmptTraits<val>>::template LayerIterator<typename StdCmptTraits<val>::template Divide<radix>, val> it(Val);
			for (auto index = this->begin(); !!it; ) {
				*index = *it;
				++it;
				if (!!it) {
					++index;
				}
			}
			return;
		}
		explicit MY_LIB LargeUnsigned(LL& ll)noexcept:LL(ll) { }
		static constexpr LargeUnsigned MY_LIB Copy(const LargeUnsigned& that)noexcept {
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

		constexpr void MY_LIB destruct()noexcept {
			this->LL::destruct();
		}

		//二进制输出到控制台窗口
		/*INLINED*/friend std::ostream& MY_LIB operator<<(
			std::ostream& out,
			const LargeUnsigned& l
			) noexcept {
			return _Print<decltype(l.cbegin()), radix>(l.cbegin(), out);
		}
		template<typename Cntnr, auto Radix = 0>
		//二进制输出到控制台窗口
		//不再自动换行
		static /*INLINED*/std::ostream & MY_LIB _Print(
			const Cntnr & that,
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
		INLINED std::ostream& MY_LIB Print(std::ostream& o = std::cout) const noexcept {
			return _Print<LL, radix>(static_cast<LL>(*this), o);
		}
		template<typename Int>
		/*INLINED*/ void MY_LIB operator*=(const Int& that) noexcept {
			static_assert(std::is_integral_v<Int>);
			typename LongCmpt<StdCmptTraits<Int>>::template LayerIterator<typename StdCmptTraits<Int>::template Divide<radix>, Int> it(that);
			this->mul(it);
		}
		template<typename Int>
		/*INLINED*/LargeUnsigned MY_LIB operator*(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeUnsigned temp(that);
			*this *= temp;
			temp.destruct();
		}
		constexpr void MY_LIB Swap(LargeUnsigned& that)noexcept {
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
		/*INLINED*/void MY_LIB operator*=(const LargeUnsigned& b) noexcept {
			this->mul(b.begin());
		}
		//重载
		/*INLINED*/LargeUnsigned MY_LIB operator*(const LargeUnsigned& b)const noexcept {
			LargeUnsigned Res(*this, true);
			Res *= b;
			return Res;
		}
		//重载
		INLINED void MY_LIB operator-=(const LargeUnsigned& that) noexcept {
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
		INLINED LargeUnsigned MY_LIB operator-(
			)const noexcept {
			LargeUnsigned res = Copy(*this);
			return res;
		}
		//重载LinkedList链表+=
		INLINED void MY_LIB operator+=(const LargeUnsigned& that) noexcept {
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
		INLINED LargeUnsigned MY_LIB operator+(
			const LargeUnsigned& b//操作数
			)  const noexcept {
			LargeUnsigned Result(*this, true);//存储结果
			Result += b;
			return Result;
		}
		//重载LinkedList链表减号
		/*INLINED*/LargeUnsigned MY_LIB operator-(
			const LargeUnsigned& b//操作数
			)const noexcept {
			return (*this -= b);
		}
		void MY_LIB operator++() {
			*this += 1;
		}
		void MY_LIB operator--() {
			*this -= 1;
		}


		template<typename Int>
		INLINED void MY_LIB operator+=(const Int& that)noexcept {
			static_assert(std::is_integral_v<Int>, "integral type required.");
			if (that == 0) {
				return;
			}
			if (*this == 0) {
				*this = that;
				return;
			}
			typename LongCmpt<StdCmptTraits<Int>>::template LayerIterator<typename StdCmptTraits<Int>::template Divide<radix>, Int> it(that);
			LargeInteger::LongCmpt<LLCmptTraits<radix>>::AddTo(it, this->begin());
		}
		template<typename Int>
		INLINED void MY_LIB operator-=(const Int& that)noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeUnsigned temp(that);
			*this -= temp;
			temp.destruct();
		}
		template<typename Int>
		INLINED LargeUnsigned MY_LIB operator+(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeUnsigned res = Copy(*this);
			res += that;
			return res;
		}
		template<typename Int>
		INLINED LargeUnsigned MY_LIB operator-(const Int& that)const noexcept {
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
		bool MY_LIB operator==(const LargeUnsigned& that)const noexcept {
			return LL::operator==(that);
		}
		bool MY_LIB operator!=(const LargeUnsigned& that)const noexcept {
			return LL::operator!=(that);
		}
		template<typename Int>
		bool MY_LIB operator==(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			if (that == Int(0)) {
				return (this->LL::next == nullptr) && (this->LL::data == Data(0));
			}
			typename LongCmpt<StdCmptTraits<radix_t>>::template LayerIterator<typename StdCmptTraits<radix_t>::template Divide<radix>, radix_t> it(that);
			return (LargeInteger::LongCmpt<LLCmptTraits<radix>>::CompareTo(this->begin(), it) == Compare::Equal);
		}
		template<typename Int>
		bool MY_LIB operator!=(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			return !(*this == that);
		}
		bool MY_LIB operator<(const LargeUnsigned& that)const noexcept {
			return (LargeInteger::LongCmpt<LLCmptTraits<radix>>::CompareTo(this->begin(), that.begin()) == Compare::Smaller);
		}
		bool MY_LIB operator>(const LargeUnsigned& that)const noexcept {
			return(LargeInteger::LongCmpt<LLCmptTraits<radix>>::CompareTo(this->begin(), that.begin()) == Compare::Larger);
		}
		bool MY_LIB operator<=(const LargeUnsigned& that)const noexcept {
			return !(*this > that);
		}
		bool MY_LIB operator>=(const LargeUnsigned& that)const noexcept {
			return !(*this < that);
		}
		template<typename Int>
		bool MY_LIB operator<(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeUnsigned temp(that);
			bool&& Res = (*this < temp);
			temp.destruct();
			return Res;
		}
		template<typename Int>
		bool MY_LIB operator>(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeUnsigned temp(that);
			bool&& Res = (*this > temp);
			temp.destruct();
			return Res;
		}
		template<typename Int>
		bool MY_LIB operator<=(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeUnsigned T(that);
			return (*this <= T);
		}
		template<typename Int>
		bool MY_LIB operator>=(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeUnsigned T = LargeUnsigned(that);
			return (*this >= T);
		}
		template<typename Cntnr>
		class Sim {
		public:
			MY_LIB Sim(Cntnr it)noexcept {
				it->Simplify();
			}

			MY_LIB ~Sim() = default;
		};
		void MY_LIB operator%=(const LargeUnsigned& that)noexcept {
			assert(that != 0);
			
			LargeInteger::LongCmpt<LLCmptTraits<radix>>::template DivideInto<Sim<decltype(this->begin())>, decltype(that.begin()), decltype(this->begin())>(that.begin(), this->begin());
			this->Simplify();
		}
		void MY_LIB operator/=(const LargeUnsigned& that)noexcept {
			assert(that != 0);
			LargeUnsigned Res(0);
			LargeInteger::LongCmpt<LLCmptTraits<radix>>::template DivideInto<Sim<decltype(this->begin())>, decltype(Res), decltype(that.begin()), decltype(this->begin())>(Res, that.begin(), this->begin());
			*this = Res;
			this->Simplify();
		}
		LargeUnsigned MY_LIB Divide(const LargeUnsigned& that)noexcept {
			assert(that != 0);
			LargeUnsigned Res(0);
			LargeInteger::LongCmpt<LLCmptTraits<radix>>::template DivideInto<Sim<decltype(this->begin())>, decltype(Res), decltype(that.begin()), decltype(this->begin())>(Res, that.begin(), this->begin());
			
			this->Simplify();
			return Res;
		}
		template<typename Int>
		void MY_LIB operator%=(const Int& that)noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeUnsigned temp(that);
			*this %= temp;
			temp.destruct();
		}
		template<typename Int>
		void MY_LIB operator/=(const Int& that)noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeUnsigned temp(that);
			*this /= temp;
			temp.destruct();
		}
		template<typename Int>
		LargeUnsigned MY_LIB Divide(const Int& that)noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeUnsigned temp(that);
			auto &&res = this->Divide(temp);
			temp.destruct();
			return res;
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
		/*INLINED*/LargeUnsigned& MY_LIB operator=(
			Int Val
			) noexcept {
			if (Val == 0) {
				this->destruct();
			}
			typename LongCmpt<StdCmptTraits<Int>>::template LayerIterator<typename StdCmptTraits<Int>::template Divide<radix>, Int> it(Val);
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
		/*INLINED*/Val MY_LIB GetValue()const noexcept {
			Val value = 0;
			unsigned __int64 n = 0;
			auto OprtPtr = this->cbegin();
			if (OprtPtr == nullptr) {
				return 0;
			}
			while (true) {
				value += ((Val)((*OprtPtr))) * Power(static_cast<Val>(radix), n);
				if (OprtPtr + 1 != nullptr) {
					++OprtPtr;
					++n;
				}
				else  break;
			}
			return (value);
		}
		const auto& MY_LIB GetThis()const noexcept {
			return this->data;
		}

		~LargeUnsigned() = default;
	};
	template<typename LL, auto radix>
	class LargeSigned :protected LargeUnsigned<LL, radix> {
		using radix_t=decltype(radix);
		using Data=radix_t;
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
		template<typename val> explicit MY_LIB LargeSigned(val Val)noexcept
			:PosSign(Val > 0), LargeUnsigned<LL, radix>(ABS(Val)) { }
		template<typename val> explicit MY_LIB LargeSigned(bool Pos, val Val)noexcept
			:PosSign(Pos), LargeUnsigned<LL, radix>(Val) {
			assert(Val >= 0);
		}
		explicit MY_LIB LargeSigned(bool sign, LargeUnsigned<LL, radix> uns)noexcept
			:PosSign(sign), LargeUnsigned<LL, radix>(uns) { }

		static constexpr LargeSigned MY_LIB Copy(const LargeSigned& that)noexcept {
			LargeSigned This(that.PosSign, LargeUnsigned<LL, radix>::Copy(that));
			return This;
		}
		constexpr void MY_LIB _Swap(LargeSigned& that)noexcept {
			this->LargeUnsigned<LL, radix>::Swap(that);
		}
		constexpr void MY_LIB Swap(LargeSigned& that)noexcept {
			this->_Swap(that);
			bool temp = that.PosSign;
			that.PosSign = this->PosSign;
			this->PosSign = temp;
		}
		constexpr void MY_LIB destruct() noexcept {
			this->LL::destruct();
		}

		bool MY_LIB operator==(const LargeSigned& that)const noexcept {
			return (this->LargeUnsigned<LL, radix>::operator==(*static_cast<const LargeUnsigned<LL, radix>*>(&that)) && (this->PosSign == that.PosSign));
		}
		bool MY_LIB operator!=(const LargeSigned& that)const noexcept {
			return !(*this == that);
		}
		bool MY_LIB operator<(const LargeSigned& that)const noexcept {
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
		bool MY_LIB operator>(const LargeSigned& that)const noexcept {
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
		bool MY_LIB operator<=(const LargeSigned& that)const noexcept {
			return !(*this > that);
		}
		bool MY_LIB operator>=(const LargeSigned& that)const noexcept {
			return !(*this < that);
		}
		template<typename Int>
		bool MY_LIB operator==(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			return ((that == 0 || (this->PosSign == (that > 0))) && this->LargeUnsigned<LL, radix>::operator==(that));
		}
		template<typename Int>
		bool MY_LIB operator!=(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			return !(*this == that);
		}
		template<typename Int>
		bool MY_LIB operator>(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			return (this->LargeUnsigned<LL, radix>::operator>(that));
		}
		template<typename Int>
		bool MY_LIB operator<(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			return (this->LargeUnsigned<LL, radix>::operator<(that));
		}
		template<typename Int>
		bool MY_LIB operator>=(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			return (this->LargeUnsigned<LL, radix>::operator>=(that));
		}
		template<typename Int>
		bool MY_LIB operator<=(const Int& that)const noexcept {
			static_assert(std::is_integral_v<Int>);
			return (this->LargeUnsigned<LL, radix>::operator<=(that));
		}

		template<typename Val>
		LargeSigned& MY_LIB operator=(Val value) noexcept {
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
		LargeSigned& MY_LIB operator+=(const LargeSigned& that) noexcept {
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
		LargeSigned MY_LIB operator+(const LargeSigned& that) const noexcept {
			LargeSigned temp = Copy(*this);
			temp += that;
			return temp;
		}
		LargeSigned& MY_LIB operator-=(const LargeSigned& that) noexcept {
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
		LargeSigned MY_LIB operator-(const LargeSigned& that) const noexcept {
			LargeSigned temp = Copy(*this);
			temp -= that;
			return temp;
		}
		template<typename Int>
		LargeSigned& MY_LIB operator+=(const Int& that) noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeSigned That(that);
			*this += That;
			That.destruct();
			return *this;
		}
		template<typename Int>
		LargeSigned MY_LIB operator+(const Int& that) const noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeSigned temp = Copy(*this);
			temp += that;
			return temp;
		}
		template<typename Int>
		LargeSigned& MY_LIB operator-=(const Int& that) noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeSigned That(that);
			*this -= That;
			That.destruct();
			return *this;
		}
		template<typename Int>
		LargeSigned MY_LIB operator-(const Int& that) const noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeSigned temp = Copy(*this);
			temp -= that;
			return temp;
		}
		LargeSigned& MY_LIB operator*=(const LargeSigned& that) noexcept {
			if (!that.PosSign) {
				this->PosSign = !this->PosSign;
			}
			this->LargeUnsigned<LL, radix>::operator*=(*static_cast<const LargeUnsigned<LL, radix>*>(&that));
			return *this;
		}
		LargeSigned MY_LIB operator*(const LargeSigned& that) const noexcept {
			LargeSigned temp = Copy(*this);
			temp *= that;
			return temp;
		}
		template<typename Int>
		LargeSigned& MY_LIB operator*=(const Int& that) noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeSigned That(that);
			*this *= That;
			That.destruct();
			return *this;
		}
		template<typename Int>
		LargeSigned MY_LIB operator*(const Int& that) const noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeSigned temp = Copy(*this);
			temp *= that;
			return temp;
		}
		LargeSigned& MY_LIB operator%=(const LargeSigned& that) noexcept {
			if (!that.PosSign) {
				this->PosSign = !this->PosSign;
			}
			this->LargeUnsigned<LL, radix>::operator%=(*static_cast<const LargeUnsigned<LL, radix>*>(&that));
			return *this;
		}
		LargeSigned MY_LIB operator%(const LargeSigned& that) const noexcept {
			LargeSigned temp = Copy(*this);
			temp %= that;
			return temp;
		}
		LargeSigned& MY_LIB operator/=(const LargeSigned& that) noexcept {
			if (!that.PosSign) {
				this->PosSign = !this->PosSign;
			}
			this->LargeUnsigned<LL, radix>::operator/=(*static_cast<const LargeUnsigned<LL, radix>*>(&that));
			return *this;
		}
		LargeSigned MY_LIB operator/(const LargeSigned& that) const noexcept {
			LargeSigned temp = Copy(*this);
			temp /= that;
			return temp;
		}
		template<typename Int>
		LargeSigned& MY_LIB operator%=(const Int& that) noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeSigned That(that);
			*this %= That;
			That.destruct();
			return *this;
		}
		template<typename Int>
		LargeSigned MY_LIB operator%(const Int& that) const noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeSigned temp = Copy(*this);
			temp %= that;
			return temp;
		}
		template<typename Int>
		LargeSigned& MY_LIB operator/=(const Int& that) noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeSigned That(that);
			*this /= That;
			That.destruct();
			return *this;
		}
		template<typename Int>
		LargeSigned MY_LIB operator/(const Int& that) const noexcept {
			static_assert(std::is_integral_v<Int>);
			LargeSigned temp = Copy(*this);
			temp /= that;
			return temp;
		}
		void MY_LIB SetToContradict() noexcept {
			this->PosSign = !this->PosSign;
		}

		//获取存储的值
		//可能溢出
		template<typename val=__int64>
		/*INLINED*/val MY_LIB GetValue()const noexcept {
			val&& value = LargeUnsigned<LL, radix>::template GetValue<val>();
			if (!PosSign) {
				value = -value;
			}
			return value;
		}

		const auto &MY_LIB GetThis()const noexcept {
			return this->data;
		}

		//二进制输出到控制台窗口
		/*INLINED*/friend std::ostream& MY_LIB operator<<(
			std::ostream& out,
			const LargeSigned& l
			) noexcept {
			return l.LargeSigned::Print(out);
		}

		INLINED std::ostream& MY_LIB Print(std::ostream& o = std::cout) const noexcept {
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