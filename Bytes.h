#pragma once

#include <memory>
#include <cstring>
#include <type_traits>
#include "Shared.h"
#include "Statistics.h"

namespace LargeInteger {

	template<size_t Length>
	class Bytes;

	using value_type=unsigned char;
	//***************************************************
	//***************************************************
	//
	//
	//***************************************************
	//***************************************************

	char MY_LIB ENDIAN()noexcept {
		union { char c[4]; unsigned __int32 l; }endian_test = { { 'l', '?', '?', 'b' } };
		return((char)endian_test.l);
	}


	template<size_t Length>
	class Bytes
	{
	private:
		value_type Byte[Length] = {};
	public:
		constexpr explicit MY_LIB Bytes()noexcept {}
		template<size_t OriginLength> constexpr explicit MY_LIB Bytes(const Bytes<OriginLength>&)noexcept;
		template<typename Data>constexpr explicit MY_LIB Bytes(const Data data)noexcept;
		template<typename Data>constexpr MY_LIB operator Data()noexcept;
		constexpr size_t MY_LIB GetLength()const noexcept;

		constexpr MY_LIB Bytes(const value_type data) noexcept :Byte{ data } {}
		constexpr Bytes& MY_LIB operator=(unsigned char Value) noexcept {
			if constexpr (Length != 0)
			{
				memset(this->Byte, 0, Length);
				this->Byte[0] = Value;
			}
			return *this;
		}
		constexpr Bytes& MY_LIB operator+=(const Bytes& that)noexcept {
			static_assert(Length != 0);
			bool Carry = false;
			for (size_t i = 0; i < Length; ++i) {
				//If (a + b) overflows, then a > ~b.
				if (Carry) {
					if (Byte[i] == std::numeric_limits<value_type>::max()) {
						Byte[i] = 0;
						Carry = true;
					}
					else {
						Byte[i] += 1;
						Carry = false;
					}
				}
				if (Byte[i] > static_cast<value_type>(~that.Byte[i])) {
					Carry = true;
				}
				Byte[i] += that.Byte[i];
			}
			return *this;
		}
		constexpr value_type MY_LIB add_s(const Bytes& that) noexcept {
			static_assert(Length != 0);
			bool Carry = false;
			for (size_t i = 0; i < Length; ++i) {
				//If (a + b) overflows, then a > ~b.
				if (Carry) {
					if (Byte[i] == std::numeric_limits<value_type>::max()) {
						Byte[i] = 0;
						Carry = true;
					}
					else {
						Byte[i] += 1;
						Carry = false;
					}
				}
				if (Byte[i] > static_cast<value_type>(~that.Byte[i])) {
					Carry = true;
				}
				Byte[i] += that.Byte[i];
			}
			return Carry;
		}
		constexpr value_type MY_LIB sub_s(const Bytes& that) noexcept {
			return (((this->add_s(~that) | this->add_s(Bytes(1))) > 0) ? 0 : 1);
		}
		constexpr Bytes MY_LIB operator+(const Bytes& that) const noexcept {
			Bytes Ret = *this;
			return (Ret += that);
		}
		constexpr Bytes& MY_LIB operator++() noexcept {
			return (*this += Bytes(1));
		}
		constexpr Bytes& MY_LIB operator--() noexcept {
			return (*this -= Bytes(1));
		}
		constexpr Bytes& MY_LIB operator*=(const Bytes& that)noexcept {
			Bytes This(*this);
			memset(this->Byte, 0, Length);
			for (size_t i = 0; i < Length; i++)
			{
				value_type temp = that.Byte[i];
				for (unsigned char j = 0; j < BitsPerByte; j++)
				{
					if ((temp & 0x1) > 0) {
						*this += This;
					}
					This <<= 1;
					temp >>= 1;
				}
			}
			return *this;
		}
		constexpr Bytes MY_LIB operator*(const Bytes& that) const noexcept {
			Bytes Ret = *this;
			return (Ret *= that);
		}
		constexpr Bytes MY_LIB operator~ ()const noexcept {
			Bytes ret;
			for (size_t i = 0; i < Length; i++) {
				ret.Byte[i] = ~this->Byte[i];
			}
			return ret;
		}
		constexpr size_t MY_LIB GetBits()const noexcept {
			size_t i = 0;
			size_t t = Length - 1;
			for (;; --t)
			{
				if (Byte[t] != 0)
				{
					i += (t - 1) * BitsPerByte;
					break;
				}
				if (t == 0)
				{
					break;
				}
			}
			value_type temp = Byte[t]; 
			size_t s = 0;
			for (; s < BitsPerByte && (temp != 0); ++s)
			{
				temp >>= 1;
			}
			i += s;
			return i;
		}
		//return the quotient
		constexpr Bytes MY_LIB Divide(const Bytes& that)noexcept {
			if (*this < that) {
				return Bytes(0);
			}
			else if (*this == that) {
				memset(this, 0, Length);
				return Bytes(1);
			}
			else {
				Bytes Quo(0),temp(that);
				size_t diff = this->GetBits() - that.GetBits();
				temp <<= diff;
				for (size_t i = 0; i <= diff; i++)
				{
					if (*this >= temp) {
						*this -= temp;
						Quo.Byte[0] |= 0x1;
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
		constexpr Bytes& MY_LIB operator/=(const Bytes& that) noexcept {
			*this = Divide(that);
			return *this;
		}
		constexpr Bytes MY_LIB operator/(const Bytes& that)const noexcept {
			Bytes Ret = *this;
			return (Ret /= that);
		}
		constexpr Bytes& MY_LIB operator%=(const Bytes& that) noexcept {
			Divide(that);
			return *this;
		}
		constexpr Bytes MY_LIB operator%(const Bytes& that)const noexcept {
			Bytes Ret = *this;
			return (Ret %= that);
		}
		constexpr Bytes& MY_LIB operator-=(const Bytes& that) noexcept {
			Bytes Minus = ~that + Bytes(1);
			return(*this += Minus);
		}
		constexpr Bytes MY_LIB operator-(const Bytes& that)const noexcept {
			Bytes Ret = *this;
			return (Ret -= that);
		}
		constexpr Bytes& MY_LIB operator|=(const Bytes& that) noexcept {
			for (size_t i = 0; i < Length; i++)
			{
				this->Byte[i] |= that.Byte[i];
			}
			return *this;
		}
		constexpr Bytes MY_LIB operator|(const Bytes& that)const noexcept {
			Bytes ret = *this;
			return (ret |= that);
		}
		constexpr Bytes& MY_LIB operator&=(const Bytes& that) noexcept {
			for (size_t i = 0; i < Length; i++)
			{
				this->Byte[i] &= that.Byte[i];
			}
			return *this;
		}
		constexpr Bytes MY_LIB operator&(const Bytes& that)const noexcept {
			Bytes ret = *this;
			return (ret &= that);
		}
		constexpr Bytes MY_LIB operator^=(const Bytes& that) noexcept {
			for (size_t i = 0; i < Length; i++)
			{
				this->Byte[i] ^= that.Byte[i];
			}
			return *this;
		}
		constexpr Bytes MY_LIB operator^(const Bytes& that)const noexcept {
			Bytes ret = *this;
			return (ret ^= that);
		}
		constexpr bool MY_LIB operator!=(const Bytes& that)const noexcept {
			for (size_t i = 0; i < Length; i++)
			{
				if (this->Byte[i] != that.Byte[i])
				{
					return true;
				}
			}
			return false;
		}
		constexpr bool MY_LIB operator==(const Bytes& that)const noexcept {
			return !(*this != that);
		}
		constexpr Bytes MY_LIB operator<<(size_t Bits) noexcept {
			Bytes ret = *this;
			return (ret <<= Bits);
		}
		constexpr Bytes MY_LIB operator>>(size_t Bits) noexcept {
			Bytes ret = *this;
			return (ret >>= Bits);
		}
		constexpr Bytes& MY_LIB operator<<=(size_t Bits)noexcept {
			static_assert((Length != 0));
			if ((Bits / Length) >= BitsPerByte)
			{
				memset(this->Byte, 0, Length);
				return *this;
			}
			if (Bits >= BitsPerByte)
			{
				memcpy(this->Byte + Bits / BitsPerByte, this->Byte, Length - Bits / BitsPerByte);
				memset(this->Byte, 0, Bits / BitsPerByte);
			}
			if ((Bits % BitsPerByte) != 0)
			{
				value_type Pre = {}, temp = {};
				for (size_t i = Bits / BitsPerByte; i < Length; i++)
				{
					temp = this->Byte[i] >> (BitsPerByte - Bits % BitsPerByte);
					this->Byte[i] = (this->Byte[i] << (Bits % BitsPerByte)) | Pre;
					Pre = temp;
				}
			}
			return *this;
		}
		constexpr Bytes MY_LIB operator>>=(size_t Bits)noexcept {
			if ((Bits / Length) >= BitsPerByte)
			{
				memset(this->Byte, 0, Length);
				return *this;
			}
			if constexpr (Length != 0)
			{
				if (Bits >= BitsPerByte)
				{
					memcpy(this->Byte, this->Byte + Bits / BitsPerByte, Length - Bits / BitsPerByte);
					memset(this->Byte + Bits / BitsPerByte + 1, 0, Bits / BitsPerByte);
				}
				if ((Bits % BitsPerByte) != 0)
				{
					value_type Pre = {}, temp = {};
					for (size_t i = Length; i > Bits / BitsPerByte; i--)
					{
						temp = this->Byte[i - 1] << (BitsPerByte - Bits % BitsPerByte);
						this->Byte[i - 1] = (this->Byte[i - 1] >> (Bits % BitsPerByte)) | Pre;
						Pre = temp;
					}
				}
			}
			return *this;
		}
		constexpr bool MY_LIB operator<(const Bytes& that)const noexcept {
			if (Length == 0)return false;
			for (size_t i = Length - 1;; i--)
			{
				if (this->Byte[i] < that.Byte[i])
				{
					return true;
				}
				else if (this->Byte[i] > that.Byte[i])
				{
					return false;
				}
				if (i == 0)
				{
					break;
				}
			}
			return false;
		}
		constexpr bool MY_LIB operator<=(const Bytes& that)const noexcept {
			if (Length == 0)return true;
			for (size_t i = Length - 1;; i--)
			{
				if (this->Byte[i] < that.Byte[i])
				{
					return true;
				}
				else if (this->Byte[i] > that.Byte[i])
				{
					return false;
				}
				if (i == 0)
				{
					break;
				}
			}
			return true;
		}
		constexpr bool MY_LIB operator>(const Bytes& that)const noexcept {
			return !(*this <= that);
		}
		constexpr bool MY_LIB operator>=(const Bytes& that)const noexcept {
			return !(*this < that);
		}
		constexpr const value_type& MY_LIB operator[](size_t index)const noexcept { return this->Byte[index]; }
		constexpr MY_LIB Bytes(const std::initializer_list<value_type>& that)noexcept;
	};
	//***************************************************
	template<size_t Length>
	template<size_t OriginLength>
	constexpr INLINED MY_LIB Bytes<Length>::Bytes(
		const Bytes<OriginLength>& that
	)noexcept {
		if constexpr (Length >= OriginLength)
			memcpy(&this->Byte, &that.Byte, OriginLength);
		else
			memcpy(&this->Byte, &that.Byte, Length);
		return;
	}
	template<size_t Length>
	constexpr INLINED MY_LIB LargeInteger::Bytes<Length>::Bytes(
		const std::initializer_list<value_type>& that
	) noexcept {
		size_t a = 0;
		for (auto i : that)
		{
			if (a >= Length)
			{
				break;
			}
			this->Byte[a++] = i;
		}
	}

	template<size_t Length>
	template<typename Data>
	constexpr INLINED MY_LIB LargeInteger::Bytes<Length>::Bytes(Data data) noexcept {
		for (size_t i = 0; i < Length; i++)
		{
			this->Byte[i] = data & 0xff;
			data >>= 8;
		}
		/*
		if constexpr (sizeof(Data)<=Length)
		{
			memcpy(this->Byte, &data, sizeof(Data));
		}
		else
		{
			memcpy(this->Byte, &data, Length);
		}*/
	}

	template<size_t Length>
	template<typename Data>
	constexpr INLINED MY_LIB LargeInteger::Bytes<Length>::operator Data () noexcept {
		Data data = {};
		if constexpr (sizeof(Data) <= Length)
		{
			memcpy(&data, this->Byte, sizeof(Data));
		}
		else
		{
			memcpy(&data, this->Byte, Length);
		}
		return data;
	}

	template<size_t Length>
	constexpr size_t MY_LIB Bytes<Length>::GetLength()const noexcept {
		size_t Res = 0;
		Bytes<Length> This = *this;
		while (This != 0)
		{
			This >>= 1;
			++Res;
		}
		return Res;
	}
	/*
	template<size_t Length>
	struct BytesIterator
	{
		const Bytes<Length>* Head;
		size_t Index;
		constexpr MY_LIB BytesIterator(const Bytes<Length>* Head, const size_t index = 0)noexcept :Head(Head), Index(index) {}
		constexpr bool MY_LIB operator==(const BytesIterator& that)const noexcept { return ((this->Head == that.Head) && (this->Index == that.Index)); }
		constexpr bool MY_LIB operator!=(const BytesIterator& that)const noexcept { return !(*this == that); }
	};*/

	/*
	template<size_t Length>
	class BytesTraits :public StdCmptTraits<value_type>
	{
	public:
		BytesTraits() = delete;
		~BytesTraits() = delete;

		const static INLINED Bytes<Length> NullObject = Bytes<Length>();
		constexpr static INLINED BytesIterator<Length> NullIterator = { &NullObject,0 };

		static constexpr void MY_LIB assign(Bytes<Length>* ptr, size_t s) noexcept {
			*ptr <<= 8;
		}
		static constexpr BytesIterator<Length> MY_LIB GetNext(const BytesIterator<Length>& ptr) noexcept {
			if (ptr.Head != nullptr && ptr.Head != &NullObject)
			{
				if (ptr.Index >= Length - 1)
				{
					return NullIterator;
				}
				else return BytesIterator<Length>(ptr.Head, ptr.Index + 1);
			}
			else return NullIterator;
		}

		static constexpr value_type& MY_LIB GetData(const BytesIterator<Length>& ptr) noexcept {
			return const_cast<value_type*>(ptr.Head->Byte)[ptr.Index];
		}
		static constexpr void MY_LIB InsertAfter(const BytesIterator<Length>& ptr, value_type data = 0) noexcept {}
	private:

	};*/

};
template<size_t _Length>class std::numeric_limits<LargeInteger::Bytes<_Length>> {
public:
	static constexpr LargeInteger::Bytes<_Length> max() noexcept { return ~LargeInteger::Bytes<_Length>(0); }
	static constexpr LargeInteger::Bytes<_Length> min()noexcept { return LargeInteger::Bytes<_Length>(0); }
};