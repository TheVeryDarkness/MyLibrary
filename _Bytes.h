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
	public:
		constexpr MY_LIBRARY _Bytes() = default;
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
	public:
		using value_type=decltype(Byte);
		template<typename Val, typename... Vals>
		constexpr MY_LIBRARY _Bytes(Val val, Vals... vals)
			noexcept : Byte(val), _Bytes<IntelligentLength(Length).first>(vals...) {}; //= default;
		MY_LIBRARY ~_Bytes() = default;
		constexpr _Bytes Comple()noexcept {
			this->Comp();
			return *this;
		}
		constexpr _Bytes& operator+=(const _Bytes& that)noexcept {
			this->add_o(that);
			return *this;
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
				(this->Byte == that.Byte) ?
				true
				: (this->_Bytes<IntelligentLength(Length).first>::operator==(*static_cast<const _Bytes<IntelligentLength(Length).first>*>(&that))));
		}
		constexpr bool operator!=(const _Bytes& that)const noexcept {
			return !(*this == that);
		}
		constexpr static size_t getAccount()noexcept { return (_Bytes<IntelligentLength(Length).first>::getAccount() + 1); }
		constexpr friend std::ostream& operator<<(std::ostream& o, const _Bytes& b)noexcept {
			o << std::hex << "type = " << typeid(decltype(b.Byte)).name() << ", data = " << (size_t)b.Byte << std::dec << ".";
			return o << *static_cast<const _Bytes<IntelligentLength(Length).first>*>(&b);
		}
	};
}
#pragma pack(pop)