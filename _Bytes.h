#pragma once
#include "Shared.h"


#pragma pack(push)
#pragma pack(1)

namespace Integer {
	constexpr std::pair<size_t, size_t> MY_LIBRARY IntelligentLength(size_t sz)noexcept {
		if (sz >= 8)
		{
			return std::pair<size_t, size_t>(sz - 8, 8);
		}
		else if (sz >= 4)
		{
			return std::pair<size_t, size_t>(sz - 4, 4);
		}
		else if (sz >= 2)
		{
			return std::pair<size_t, size_t>(sz - 2, 2);
		}
		else if (sz >= 1)
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
	class _Bytes<0> {
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
			return true;
		}
		constexpr bool PlusOne()noexcept {
			return true;
		}
		constexpr bool MinusOne()noexcept {
			return true;
		}
	public:
		constexpr size_t MY_LIBRARY GetBits()const noexcept { return 0; }
		constexpr MY_LIBRARY _Bytes() = default;
		template<typename Val>constexpr MY_LIBRARY _Bytes(Val val) {
			assert(val == 0);
		}
		MY_LIBRARY ~_Bytes() = default;
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
	class _Bytes :public _Bytes<IntelligentLength(Length).first>
	{
	private:
		typename _Int<IntelligentLength(Length).second>::type Byte;
	protected:
		constexpr bool add_o(const _Bytes& that)noexcept {
			bool Carry = this->_Bytes<IntelligentLength(Length).first>::add_o(*static_cast<const _Bytes<IntelligentLength(Length).first>*>(&that));
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
			bool Carry = this->_Bytes<IntelligentLength(Length).first>::sub_u(*static_cast<const _Bytes<IntelligentLength(Length).first>*>(&that));
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
			if (this->_Bytes<IntelligentLength(Length).first>::Comp())
			{
				bool Carry = (this->Byte == std::numeric_limits<value_type>::max());
				this->Byte += 1;
				return Carry;
			}
			return false;
		}
		constexpr void And(const _Bytes* that)noexcept {
			this->Byte &= that->Byte;
			this->_Bytes<IntelligentLength(Length).first>::And(static_cast<const _Bytes<IntelligentLength(Length).first>*>(that));
		}
		constexpr void Or(const _Bytes* that)noexcept {
			this->Byte |= that->Byte;
			this->_Bytes<IntelligentLength(Length).first>::Or(static_cast<const _Bytes<IntelligentLength(Length).first>*>(that));
		}
		constexpr void XOr(const _Bytes* that)noexcept {
			this->Byte ^= that->Byte;
			this->_Bytes<IntelligentLength(Length).first>::XOr(static_cast<const _Bytes<IntelligentLength(Length).first>*>(that));
		}
		constexpr bool SHL()noexcept {
			bool&& temp = this->_Bytes<IntelligentLength(Length).first>::SHL();
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
			this->_Bytes<IntelligentLength(Length).first>::SHR(temp);
		}
		constexpr void _SHR(bool fill, bool& tail)noexcept {
			bool temp = this->Byte & 0x1;
			this->Byte >>= 1;
			if (fill)
			{
				this->Byte |= value_type(value_type(1) << (sizeof(value_type) * LargeInteger::BitsPerByte - 1));
			}
			tail = this->Byte & 0x1;
			return this->_Bytes<IntelligentLength(Length).first>::_SHR(temp, tail);
		}
		constexpr bool Odd()noexcept {
			if constexpr (IntelligentLength(Length).first == 0)
				return (this->Byte & 0x1);
			else return this->_Bytes<IntelligentLength(Length).first>::Odd();
		}
		constexpr bool PlusOne()noexcept {
			if (this->_Bytes<IntelligentLength(Length).first>::PlusOne())
			{
				bool temp = (this->Byte == std::numeric_limits<value_type>::max());
				this->Byte += 1;
				return temp;
			}
			else return false;
		}
		constexpr bool MinusOne()noexcept {
			if (this->_Bytes<IntelligentLength(Length).first>::MinusOne())
			{
				bool temp = (this->Byte == 0);
				this->Byte -= 1;
				return temp;
			}
			else return false;
		}
		_Bytes(const _Bytes& that) = default;
	public:
		using value_type=decltype(Byte);
		template<typename Val, typename... Vals>
		explicit constexpr MY_LIBRARY _Bytes(Val val, Vals... vals)
			noexcept : Byte(val), _Bytes<IntelligentLength(Length).first>(vals...) {}; //= default;
		template<typename Val>
		constexpr MY_LIBRARY _Bytes(Val val)
			noexcept :
			Byte(static_cast<value_type>(val)),
			_Bytes<IntelligentLength(Length).first>(
			(sizeof(Val) > sizeof(value_type)) ?
				(val >>= (sizeof(value_type) * LargeInteger::BitsPerByte))
				: Val(0)
				) {}
		constexpr MY_LIBRARY _Bytes()
			noexcept :Byte(0), _Bytes<IntelligentLength(Length).first>() {};
		MY_LIBRARY ~_Bytes() = default;
		constexpr _Bytes& Comple()noexcept {
			this->Comp();
			return *this;
		}
		constexpr bool NonZero()const noexcept {
			if (this->Byte != 0)
			{
				return false;
			}
			else return this->_Bytes<IntelligentLength(Length).first>::NonZero();
		}
		constexpr _Bytes& operator<<=(size_t sz)noexcept {
			for (size_t i = 0; i < sz; i++) {
				this->SHL();
			}
			return *this;
		}
		constexpr _Bytes& operator>>=(size_t sz)noexcept {
			for (size_t i = 0; i < sz; i++) {
				this->SHR(false);
			}
			return *this;
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
			return *this;
		}
		constexpr _Bytes& operator--()noexcept {
			return *this;
		}
		constexpr _Bytes& MY_LIBRARY operator*=(const _Bytes& that)noexcept {
			_Bytes This(*this);
			memset(this, 0, Length);
			auto temp = that;
			bool Will;
			for (size_t j = 0; j < Length * LargeInteger::BitsPerByte && temp.NonZero(); ++j)
			{
				Will = temp.Odd();
				if (Will) {
					*this += This;
				}
				This.SHL();
				//temp._SHR(false, Will);
				temp.SHR(false);
			}
			return *this;
		}
		constexpr size_t MY_LIBRARY GetBits()const noexcept {
			if (this->Byte == 0) {
				return this->_Bytes<IntelligentLength(Length).first>::GetBits();
			}
			else {
				return (IntelligentLength(Length).first * LargeInteger::BitsPerByte + getBits(this->Byte));
			}
		}
		constexpr _Bytes MY_LIBRARY Divide(const _Bytes& that)noexcept {
			if (*this < that) {
				assert(this->GetBits() <= that.GetBits());
				return _Bytes();
			}
			else if (*this == that) {
				assert(this->GetBits() == that.GetBits());
				memset(this, 0, Length);
				return _Bytes().PlusOne();
			}
			else {
				_Bytes Quo(0), temp(that);
				size_t diff = this->GetBits() - that.GetBits();
				assert(this->GetBits() >= that.GetBits());
				temp <<= diff;
				for (size_t i = 0; i <= diff; i++)
				{
					if (*this >= temp) {
						*this -= temp;
						Quo.PlusOne();
					}
					temp >>= 1;
					if (i != diff)
					{
						Quo <<= 1;
					}
				}
				return Quo;
			}
		}
		constexpr bool operator>(const _Bytes& that)const noexcept {
			return (
				(this->Byte > that.Byte) ?
				true
				: ((this->Byte < that.Byte) ?
					false
					: (this->_Bytes<IntelligentLength(Length).first>::operator>(*static_cast<const _Bytes<IntelligentLength(Length).first>*>(&that)))));
		}
		constexpr bool operator<(const _Bytes& that)const noexcept {
			return (
				(this->Byte < that.Byte) ?
				true
				: ((this->Byte > that.Byte) ?
					false
					: (this->_Bytes<IntelligentLength(Length).first>::operator<(*static_cast<const _Bytes<IntelligentLength(Length).first>*>(&that)))));
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
				: (this->_Bytes<IntelligentLength(Length).first>::operator==(*static_cast<const _Bytes<IntelligentLength(Length).first>*>(&that))));
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
		constexpr static size_t getAccount()noexcept { return (_Bytes<IntelligentLength(Length).first>::getAccount() + 1); }
		constexpr friend std::ostream& operator<<(std::ostream& o, const _Bytes& b)noexcept {
			o << std::hex << "type = " << typeid(decltype(b.Byte)).name() << ", data = " << (size_t)b.Byte << std::dec << ".";
			return o << *static_cast<const _Bytes<IntelligentLength(Length).first>*>(&b);
		}
	};
}
#pragma pack(pop)