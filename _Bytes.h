#pragma once
#include "Shared.h"
#include <cstring>

namespace LargeInteger {
	template<size_t sz>
	constexpr std::pair<size_t, size_t> MY_LIB IntelligentLength()noexcept {
		if constexpr (sz >= 8)
		{
			return std::pair<size_t, size_t>(sz - 8, 8);
		}
		else if constexpr (sz >= 4)
		{
			return std::pair<size_t, size_t>(sz - 4, 4);
		}
		else if constexpr (sz >= 2)
		{
			return std::pair<size_t, size_t>(sz - 2, 2);
		}
		else if constexpr (sz >= 1)
		{
			return std::pair<size_t, size_t>(sz - 1, 1);
		}
	}

	template<size_t Length>
	class _Bytes;

	template<size_t Length>
	class _Int;

	template<>class _Int<1> {
	public:
		using type=unsigned __int8;
	};
	template<>class _Int<2> {
	public:
		using type=unsigned __int16;
	};
	template<>class _Int<4> {
	public:
		using type=unsigned __int32;
	};
	template<>class _Int<8> {
	public:
		using type=unsigned __int64;
	};

	template<>
	class alignas(1) _Bytes<0> {
	protected:
		constexpr bool Comp()noexcept {
			return true;
		}
		constexpr bool add_o(const _Bytes& that)noexcept {
			return false;
		}
		constexpr bool sub_u(const _Bytes& that)noexcept {
			return false;
		}
		constexpr void And(const _Bytes* that)noexcept { return; }
		constexpr void Or(const _Bytes* that)noexcept { return; }
		constexpr void XOr(const _Bytes* that)noexcept { return; }
		constexpr bool SHL()noexcept { 
			return false;
		}
		constexpr void SHR(bool fill)noexcept {
			return; 
		}
		constexpr void _SHR(bool fill, bool& tail)noexcept {
			return;
		}
		constexpr void Odd(bool& buf)noexcept {}
		constexpr bool NonZero()const noexcept {
			return false;
		}
		constexpr bool PlusOne()noexcept {
			return true;
		}
		constexpr bool MinusOne()noexcept {
			return true;
		}
		template<typename Val>constexpr MY_LIB _Bytes(Val val) {}
		template<typename Val> void Maker(Val& val)noexcept { return; }
	public:
		constexpr size_t MY_LIB GetBits()const noexcept { return 0; }
		constexpr MY_LIB _Bytes() = default;
		MY_LIB ~_Bytes() = default;
		constexpr bool operator>(const _Bytes& that)const noexcept {
			return false;
		}
		constexpr bool operator<(const _Bytes& that)const noexcept {
			return false;
		}
		constexpr bool operator==(const _Bytes& that)const noexcept {
			return true;
		}
		static constexpr size_t getAccount()noexcept {
			return 0;
		}
		constexpr friend std::ostream& operator<<(std::ostream& o, const _Bytes& b)noexcept {
			return o;
		}
	};

	template<size_t Length>
	class /*alignas(1)*/ _Bytes :public _Bytes<IntelligentLength<Length>().first>
	{
	private:
		typename _Int<IntelligentLength<Length>().second>::type Byte;
		using super=_Bytes<IntelligentLength<Length>().first>;
	protected:
		using value_type=decltype(Byte);
		constexpr bool add_o(const _Bytes& that)noexcept {
			bool Carry = this->super::add_o(*static_cast<const super*>(&that));
			//If (a + b) overflows, then a > ~b.
			if (Carry) {
				if (Byte == std::numeric_limits<value_type>::max()) {
					Byte = 0;
					Carry = true;
				}
				else {
					Byte += 1;
					Carry = false;
				}
			}
			if (Byte > static_cast<value_type>(~that.Byte)) {
				Carry = true;
			}
			Byte += that.Byte;
			return Carry;
		}
		constexpr bool sub_u(const _Bytes& that)noexcept {
			bool Carry = this->super::sub_u(*static_cast<const super*>(&that));
			//If (a - b) underflows, then a < b.
			if (Carry) {
				if (Byte == 0) {
					Byte = std::numeric_limits<value_type>::max();
					Carry = true;
				}
				else {
					Byte -= 1;
					Carry = false;
				}
			}
			if (Byte < that.Byte) {
				Carry = true;
			}
			Byte -= that.Byte;
			return Carry;
		}
		constexpr bool Comp()noexcept {
			this->Byte = ~this->Byte;
			if (this->super::Comp())
			{
				bool Carry = (this->Byte == std::numeric_limits<value_type>::max());
				this->Byte += 1;
				return Carry;
			}
			return false;
		}
		constexpr void And(const _Bytes* that)noexcept {
			this->Byte &= that->Byte;
			this->super::And(static_cast<const super*>(that));
		}
		constexpr void Or(const _Bytes* that)noexcept {
			this->Byte |= that->Byte;
			this->super::Or(static_cast<const super*>(that));
		}
		constexpr void XOr(const _Bytes* that)noexcept {
			this->Byte ^= that->Byte;
			this->super::XOr(static_cast<const super*>(that));
		}
		constexpr bool SHL()noexcept {
			bool&& temp = this->super::SHL();
			bool Fill = (this->Byte >> (sizeof(value_type) * LargeInteger::BitsPerByte - 1)) & 0x1;
			this->Byte <<= 1;
			if (temp)
			{
				this->Byte |= 0x1;
			}
			return Fill;
		}
		constexpr void SHR(bool fill)noexcept {
			bool temp = this->Byte & 0x1;
			this->Byte >>= 1;
			if (fill)
			{
				this->Byte |= value_type(value_type(1) << (sizeof(value_type) * LargeInteger::BitsPerByte - 1));
			}
			this->super::SHR(temp);
		}
		constexpr void SHR()noexcept {
			bool temp = this->Byte & 0x1;
			this->Byte >>= 1;
			this->super::SHR(temp);
		}
		constexpr void _SHR(bool fill, bool& tail)noexcept {
			bool temp = this->Byte & 0x1;
			this->Byte >>= 1;
			if (fill)
			{
				this->Byte |= value_type(value_type(1) << (sizeof(value_type) * LargeInteger::BitsPerByte - 1));
			}
			tail = this->Byte & 0x1;
			return this->super::_SHR(temp, tail);
		}
		constexpr bool Odd()noexcept {
			if constexpr (IntelligentLength<Length>().first == 0)
				return (this->Byte & 0x1);
			else return this->super::Odd();
		}
		constexpr bool PlusOne()noexcept {
			if (this->super::PlusOne())
			{
				bool temp = (this->Byte == std::numeric_limits<value_type>::max());
				this->Byte += 1;
				return temp;
			}
			else return false;
		}
		constexpr bool MinusOne()noexcept {
			if (this->super::MinusOne())
			{
				bool temp = (this->Byte == 0);
				this->Byte -= 1;
				return temp;
			}
			else return false;
		}
		template<typename Val> void Maker(Val& val)noexcept {
			this->super::Maker(val);
			this->Byte = static_cast<value_type>(val); 
			val = ((sizeof(Val) > sizeof(value_type)) ?
				(val >>= (sizeof(value_type) * LargeInteger::BitsPerByte))
				: Val(0));
		}
	public:
		template<typename Val, typename... Vals>
		constexpr explicit MY_LIB _Bytes(Val val, Vals... vals)
			noexcept : Byte(val), super(vals...) {
			static_assert(std::is_integral_v<Val>);
		}; //= default;
		constexpr explicit MY_LIB _Bytes() noexcept :Byte(0), super() {};

		MY_LIB _Bytes(const _Bytes& that) = default;
		MY_LIB _Bytes(_Bytes&& that) = default;

		template<typename Val> 
		constexpr _Bytes Make(Val val)noexcept {
			this->Maker(val);
			ASSERT("Overflow in construct!", val == 0);
			return *this;
		}
		template<typename Val> 
		constexpr static _Bytes Make_s(Val val)noexcept {
			_Bytes res;
			return res.Make(val);
		}
		explicit constexpr MY_LIB _Bytes(value_type&& val)
			noexcept :Byte(static_cast<value_type>(val)){
			static_assert(std::is_integral_v<std::decay_t<value_type>>);
			static_assert(sizeof(val) == Length);
		}
		//constexpr explicit MY_LIB _Bytes(value_type val)noexcept :Byte(val) {}

		template<typename Val>
		constexpr MY_LIB operator Val()
			noexcept{
			static_assert (IntelligentLength<Length>().first == 0);
			assert(this->Byte < std::numeric_limits<Val>::max());
			return Val(this->Byte);
		};
		MY_LIB ~_Bytes() = default;
		constexpr _Bytes& Comple()noexcept {
			this->Comp();
			return *this;
		}
		constexpr bool NonZero()const noexcept {
			return (this->Byte != 0)?
				true
				: (this->super::NonZero());
		}
		constexpr _Bytes& operator<<=(size_t sz)noexcept {
			for (size_t i = 0; i < sz; i++) {
				this->SHL();
			}
			return *this;
		}
		constexpr _Bytes& operator>>=(size_t sz)noexcept {
			for (size_t i = 0; i < sz; i++) {
				this->SHR();
			}
			return *this;
		}
		constexpr _Bytes operator<<(size_t sz)const noexcept {
			_Bytes copy(*this);
			for (size_t i = 0; i < sz; i++) {
				copy.SHL();
			}
			return copy;
		}
		constexpr _Bytes operator>>(size_t sz)const noexcept {
			_Bytes copy(*this);
			for (size_t i = 0; i < sz; i++) {
				copy.SHR();
			}
			return copy;
		}
		constexpr _Bytes& operator+=(const _Bytes& that)noexcept {
			this->add_o(that);
			return *this;
		}
		constexpr _Bytes& operator-=(const _Bytes& that)noexcept {
			this->sub_u(that);
			return *this;
		}
		constexpr _Bytes& operator++()noexcept {
			this->PlusOne();
			return *this;
		}
		constexpr _Bytes& operator--()noexcept {
			this->MinusOne();
			return *this;
		}
		constexpr _Bytes& MY_LIB operator*=(const _Bytes& that)noexcept {
			_Bytes This = *this;
			memset(this, 0, Length);
			auto temp = that;
			for (size_t j = 0; j < Length * LargeInteger::BitsPerByte && temp.NonZero(); ++j)
			{
				if (temp.Odd()) {
					*this += This;
				}
				This.SHL();
				temp.SHR(false);
			}
			return *this;
		}
		constexpr size_t MY_LIB GetBits()const noexcept {
			if (this->Byte == 0) {
				return this->super::GetBits();
			}
			else {
				return (IntelligentLength<Length>().first * LargeInteger::BitsPerByte + getBits(this->Byte));
			}
		}
		constexpr _Bytes MY_LIB Divide(const _Bytes& that)noexcept {
			if constexpr (IntelligentLength<Length>().first == 0) {
				_Bytes&& Quo = _Bytes(static_cast<value_type>(this->Byte / that.Byte));
				this->Byte %= that.Byte;
				return Quo;
			}
			else {
				if (*this < that) {
					assert(this->GetBits() <= that.GetBits());
					return _Bytes();
				}
				else if (*this == that) {
					assert(this->GetBits() == that.GetBits());
					memset(this, 0, Length);
					_Bytes res;
					res.PlusOne();
					return res;
				}
				else {
					_Bytes Quo(0), temp(that);
					size_t diff = this->GetBits() - that.GetBits();
					assert(this->GetBits() >= that.GetBits());
					temp <<= diff;
					for (size_t i = 0; i <= diff; i++) {
						if (*this >= temp) {
							*this -= temp;
							Quo.PlusOne();
						}
						temp >>= 1;
						if (i != diff) {
							Quo <<= 1;
						}
					}
					return Quo;
				}
			}
		}
		constexpr bool operator>(const _Bytes& that)const noexcept {
			return (
				(this->Byte > that.Byte) ?
				true
				: ((this->Byte < that.Byte) ?
					false
					: (this->super::operator>(*static_cast<const super*>(&that)))));
		}
		constexpr bool operator<(const _Bytes& that)const noexcept {
			return (
				(this->Byte < that.Byte) ?
				true
				: ((this->Byte > that.Byte) ?
					false
					: (this->super::operator<(*static_cast<const super*>(&that)))));
		}
		constexpr bool operator>=(const _Bytes& that)const noexcept {
			return !(*this < that);
		}
		constexpr bool operator<=(const _Bytes& that)const noexcept {
			return !(*this > that);
		}
		constexpr bool operator==(const _Bytes& that)const noexcept {
			return (
				(this->Byte != that.Byte) ?
				false
				: (this->super::operator==(*static_cast<const super*>(&that))));
		}
		constexpr bool operator!=(const _Bytes& that)const noexcept {
			return !(*this == that);
		}
		constexpr _Bytes& operator&=(const _Bytes& that)noexcept {
			this->And(&that);
			return *this;
		}
		constexpr _Bytes& operator|=(const _Bytes& that)noexcept {
			this->Or(&that);
			return *this;
		}
		constexpr _Bytes& operator^=(const _Bytes& that)noexcept {
			this->XOr(&that);
			return *this;
		}
		constexpr static size_t getAccount()noexcept { return (super::getAccount() + 1); }
		constexpr friend std::ostream& operator<<(std::ostream& o, const _Bytes& b)noexcept {
			o << std::hex << "type = " << typeid(decltype(b.Byte)).name() << ", data = " << (size_t)b.Byte << std::dec << ".";
			return o << *static_cast<const super*>(&b);
		}
	};
}