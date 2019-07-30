#pragma once

#include <memory>
#include <type_traits>
#include "Bytes.h"
#include "ComputeTemplate.h"
#include "Shared.h"

namespace LargeInteger {
	constexpr size_t BitsPerByte = 8;

	template<size_t Length>
	class Bytes;

	template<typename Data>
	constexpr size_t GetMinLength(Data data)noexcept;

	template<size_t Length>
	struct BytesIterator;
	template<size_t Length>
	class BytesTraits;
	template<typename Data, Data _Max>
	class SampleTraits;

	using value_type=unsigned char;
	//***************************************************
	//***************************************************
	//
	//
	//***************************************************
	//***************************************************



	template<size_t Length = 1>
	class Bytes
	{
	private:
		friend class BytesTraits<Length>;
		value_type Byte[Length] = {};
	public:
		constexpr explicit MY_LIBRARY Bytes()noexcept {}
		template<size_t OriginLength> constexpr explicit MY_LIBRARY Bytes(const Bytes<OriginLength>&)noexcept;
		template<typename Data>constexpr explicit  MY_LIBRARY Bytes(const Data data)noexcept;
		template<typename Data>constexpr MY_LIBRARY operator Data()noexcept;
		constexpr size_t MY_LIBRARY GetLength()const noexcept;
		constexpr Bytes& MY_LIBRARY operator=(unsigned char Value) noexcept {
			if constexpr (Length != 0)
			{
				memset(this->Byte, 0, Length);
				this->Byte[0] = Value;
			}
			return *this;
		}
		constexpr Bytes& MY_LIBRARY operator+=(const Bytes& that)noexcept {
			if constexpr (Length != 0)
			{
				value_type Carry = 0;
				for (size_t i = 0; i < Length; i++)
				{
					//If (a + b) overflows, then a > ~b.
					if (Carry > 0)
					{
						if (Byte[i] > static_cast<value_type>(~Carry))
						{
							Byte[i] = 0;
							Carry = 1;
						}
						else
						{
							Byte[i] += 1;
							Carry = 0;
						}
					}
					if (Byte[i] > static_cast<value_type>(~that.Byte[i]))
					{
						Carry = 1;
					}
					Byte[i] += that.Byte[i];
				}
			}
			return *this;
		}
		constexpr value_type MY_LIBRARY add_s(const Bytes& that) noexcept {
			value_type Carry = 0;
			if constexpr (Length != 0)
			{
				for (size_t i = 0; i < Length; i++)
				{
					//If (a + b) overflows, then a > ~b.
					if (Carry > 0)
					{
						if (Byte[i] > static_cast<value_type>(~Carry))
						{
							Byte[i] = 0;
							Carry = 1;
						}
						else
						{
							Byte[i] += 1;
							Carry = 0;
						}
					}
					if (Byte[i] > static_cast<value_type>(~that.Byte[i]))
					{
						Carry = 1;
					}
					Byte[i] += that.Byte[i];
				}
			}
			return Carry;
		}
		constexpr value_type MY_LIBRARY sub_s(const Bytes& that) noexcept {
			return (((this->add_s(~that) | this->add_s(Bytes(1))) > 0) ? 0 : 1);
		}
		constexpr Bytes MY_LIBRARY operator+(const Bytes& that) const noexcept {
			Bytes Ret = *this;
			return (Ret += that);
		}
		constexpr Bytes& MY_LIBRARY operator++() noexcept {
			return (*this += Bytes(1));
		}
		constexpr Bytes& MY_LIBRARY operator--() noexcept {
			return (*this -= Bytes(1));
		}
		constexpr Bytes MY_LIBRARY operator~ ()const noexcept {
			Bytes ret;
			for (size_t i = 0; i < Length; i++)
			{
				ret.Byte[i] = ~this->Byte[i];
			}
			return ret;
		}
		constexpr Bytes& MY_LIBRARY operator/=(const Bytes& that) noexcept {
			Bytes Res;
			LongCmpt::DivideInto<Bytes, BytesIterator<Length>, value_type, BytesTraits<Length>>(Res, &that, this);
			*this = Res;
			return *this;
		}
		constexpr Bytes MY_LIBRARY operator/(const Bytes& that)const noexcept {
			Bytes Ret = *this;
			return (Ret /= that);
		}
		constexpr Bytes& MY_LIBRARY operator%=(const Bytes& that) noexcept {
			LongCmpt::DivideInto<BytesIterator<Length>, value_type, BytesTraits<Length>>(&that, this);
			return *this;
		}
		constexpr Bytes MY_LIBRARY operator%(const Bytes& that)const noexcept {
			Bytes Ret = *this;
			return (Ret %= that);
		}
		constexpr Bytes& MY_LIBRARY operator-=(const Bytes& that) noexcept {
			Bytes Minus = ~that + Bytes(1);
			return(*this += Minus);
		}
		constexpr Bytes MY_LIBRARY operator-(const Bytes& that)const noexcept {
			Bytes Ret = *this;
			return (Ret -= that);
		}
		constexpr Bytes& MY_LIBRARY operator|=(const Bytes& that) noexcept {
			for (size_t i = 0; i < Length; i++)
			{
				this->Byte[i] |= that.Byte[i];
			}
			return *this;
		}
		constexpr Bytes MY_LIBRARY operator|(const Bytes& that)const noexcept {
			Bytes ret = *this;
			return (ret |= that);
		}
		constexpr Bytes& MY_LIBRARY operator&=(const Bytes& that) noexcept {
			for (size_t i = 0; i < Length; i++)
			{
				this->Byte[i] &= that.Byte[i];
			}
			return *this;
		}
		constexpr Bytes MY_LIBRARY operator&(const Bytes& that)const noexcept {
			Bytes ret = *this;
			return (ret &= that);
		}
		constexpr Bytes MY_LIBRARY operator^=(const Bytes& that) noexcept {
			for (size_t i = 0; i < Length; i++)
			{
				this->Byte[i] ^= that.Byte[i];
			}
			return *this;
		}
		constexpr Bytes MY_LIBRARY operator^(const Bytes& that)const noexcept {
			Bytes ret = *this;
			return (ret ^= that);
		}
		constexpr bool MY_LIBRARY operator!=(const Bytes& that)const noexcept {
			for (size_t i = 0; i < Length; i++)
			{
				if (this->Byte[i] != that.Byte[i])
				{
					return true;
				}
			}
			return false;
		}
		constexpr bool MY_LIBRARY operator==(const Bytes& that)const noexcept {
			return !(*this != that);
		}
		constexpr Bytes& MY_LIBRARY operator<<=(size_t Bits) noexcept {
			return(*this = *this << Bits);
		}
		constexpr Bytes& MY_LIBRARY operator>>=(size_t Bits) noexcept {
			return(*this = *this >> Bits);
		}
		constexpr Bytes MY_LIBRARY operator<<(size_t Bits)const noexcept {
			if ((Bits / Length) >= BitsPerByte)
			{
				return Bytes();
			}
			Bytes ret;
			if constexpr (Length != 0)
			{
				if (Bits >= BitsPerByte)
				{
					memcpy(ret.Byte + Bits / BitsPerByte, this->Byte, Length - Bits / BitsPerByte);
				}
				if ((Bits % BitsPerByte) != 0)
				{
					value_type Pre = {}, temp = {};
					for (size_t i = Bits / BitsPerByte; i < Length; i++)
					{
						temp = ret.Byte[i] >> (BitsPerByte - Bits % BitsPerByte);
						ret.Byte[i] = (ret.Byte[i] << (Bits % BitsPerByte)) | Pre;
						Pre = temp;
					}
				}
			}
			return ret;
		}
		constexpr Bytes MY_LIBRARY operator>>(size_t Bits)const noexcept {
			if ((Bits / Length) >= BitsPerByte)
			{
				return Bytes();
			}
			Bytes ret;
			if constexpr (Length != 0)
			{
				if (Bits >= BitsPerByte)
				{
					memcpy(ret.Byte, this->Byte + Bits / BitsPerByte, Length - Bits / BitsPerByte);
				}
				if ((Bits % BitsPerByte) != 0)
				{
					value_type Pre = {}, temp = {};
					for (size_t i = Length - 1; i >= Bits / BitsPerByte; i--)
					{
						temp = ret.Byte[i] << (BitsPerByte - Bits % BitsPerByte);
						ret.Byte[i] = (ret.Byte[i] >> (Bits % BitsPerByte)) | Pre;
						Pre = temp;
					}
				}
			}
			return ret;
		}
		constexpr bool MY_LIBRARY operator<(const Bytes& that)const noexcept {
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
		constexpr bool MY_LIBRARY operator<=(const Bytes& that)const noexcept {
			if (Length == 0)return true;
			for (size_t i = Length - 1; i > 0; i--)
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
		constexpr bool MY_LIBRARY operator>(const Bytes& that)const noexcept {
			return !(*this <= that);
		}
		constexpr bool MY_LIBRARY operator>=(const Bytes& that)const noexcept {
			return !(*this < that);
		}
		constexpr const value_type& MY_LIBRARY operator[](size_t index)const noexcept { return this->Byte[index]; }
		constexpr MY_LIBRARY Bytes(const std::initializer_list<value_type>& that)noexcept;
	};
	//***************************************************
	template<size_t Length>
	template<size_t OriginLength>
	constexpr INLINED MY_LIBRARY Bytes<Length>::Bytes(
		const Bytes<OriginLength>& that
	)noexcept {
		if constexpr (Length >= OriginLength)
			memcpy(&this->Byte, &that.Byte, OriginLength);
		else
			memcpy(&this->Byte, &that.Byte, Length);
		return;
	}
	template<size_t Length>
	constexpr INLINED MY_LIBRARY LargeInteger::Bytes<Length>::Bytes(
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
	constexpr INLINED MY_LIBRARY LargeInteger::Bytes<Length>::Bytes(Data data) noexcept {
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
	constexpr INLINED MY_LIBRARY LargeInteger::Bytes<Length>::operator Data () noexcept {
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
	constexpr size_t MY_LIBRARY Bytes<Length>::GetLength()const noexcept {
		size_t Res = 0;
		Bytes<Length> This = *this;
		while (This != 0)
		{
			This >>= 1;
			Res++;
		}
		return Res;
	}

	template<typename Data>
	constexpr INLINED size_t GetMinLength(Data data) noexcept {
		static_assert(std::is_integral<Data>::value,"Integral required.");
		static_assert(Data(-1) > 0, "Unsigned type required.");
		size_t res = ((static_cast<Data>(~data) == 0) ? 1 : 0);
		do {
			res++;
		} while ((data >>= 8) != 0);
		return res;
	}

	template<size_t Length>
	struct BytesIterator
	{
		const Bytes<Length>* Head;
		size_t Index;
		constexpr MY_LIBRARY BytesIterator(const Bytes<Length>* Head, const size_t index = 0)noexcept :Head(Head), Index(index) {}
		constexpr bool MY_LIBRARY operator==(const BytesIterator& that)const noexcept { return ((this->Head == that.Head) && (this->Index == that.Index)); }
		constexpr bool MY_LIBRARY operator!=(const BytesIterator& that)const noexcept { return !(*this == that); }
	};

	template<typename Data, Data _Max = std::numeric_limits<Data>::max()>
	class SampleTraits
	{
	public:
		SampleTraits() = delete;
		~SampleTraits() = delete;
		static constexpr INLINED size_t length = LargeInteger::GetMinLength(_Max);
		static constexpr INLINED LargeInteger::Bytes<length> Max = LargeInteger::Bytes<length>(_Max);
		static constexpr INLINED LargeInteger::Bytes<length> Radix = LargeInteger::Bytes<length>(Max + LargeInteger::Bytes<length>(1));

		static constexpr void Add(Data& Res, Data& Carry, Data a, Data b) noexcept {
			Bytes<length> Sum = Bytes<length>(a);
			if (Carry > 0)
			{
				if (Sum >= Radix - Bytes<length>(1))
				{
					Carry = 1;
					Sum = 0;
				}
				else
				{
					Carry = 0;
					Sum += Bytes<length>(1);
				}
			}
			if (Sum >= Radix - Bytes<length>(b))
			{
				Carry = 1;
				Sum -= (Radix - Bytes<length>(b));
			}
			else
			{
				Sum += Bytes<length>(b);
			}
			Res = Data(Sum);
		}
		static constexpr void SubTractFrom(Data& Res, Data& Carry, Data a, Data b) noexcept {
			Bytes<length> Dif = Bytes<length>(b);
			if (Carry > 0)
			{
				if (Dif == Bytes<length>(0))
				{
					Carry = 1;
					Dif = Max;
				}
				else
				{
					Carry = 0;
					Dif -= Bytes<length>(1);
				}
			}
			if (Dif < Bytes<length>(a))
			{
				Carry = 1;
				Dif += (Radix - Bytes<length>(a));
			}
			else
			{
				Dif -= Bytes<length>(a);
			}
			Res = Data(Dif);
		}
		static constexpr void Multiply(Data& Res, Data& Carry, Data a, Data b) noexcept {
			static_assert(!(Radix.Byte[length - 1] >> (BitsPerByte - 1) == 1),"Overflow in multiply");
			constexpr size_t _length = 2 * length;
			Bytes<_length> Prod = Bytes<_length>(a);
			Prod *= Bytes<_length>(b);
			Prod += Bytes<_length>(Carry);
			Carry = Data(Prod / Bytes<_length>(Radix));
			Res = Data(Prod % Bytes<_length>(Radix));
		}
	};

	template<size_t Length>
	class BytesTraits :public Bytes<Length>, public SampleTraits<value_type>
	{
	public:
		BytesTraits() = delete;
		~BytesTraits() = delete;

		constexpr static INLINED Bytes<Length> NullObject = Bytes<Length>();
		constexpr static INLINED BytesIterator<Length> NullIterator = { &NullObject,0 };

		static constexpr void MY_LIBRARY assign(Bytes<Length>* ptr, size_t s) noexcept {
			*ptr <<= 8;
		}
		static constexpr BytesIterator<Length> MY_LIBRARY GetNext(const BytesIterator<Length>& ptr) noexcept {
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

		static constexpr value_type& MY_LIBRARY GetData(BytesIterator<Length> ptr) noexcept {
			return const_cast<value_type*>(ptr.Head->Byte)[ptr.Index];
		}
		static constexpr void MY_LIBRARY InsertAfter(const BytesIterator<Length>* ptr, value_type data) noexcept {}
	private:

	};

};
template<size_t _Length>class std::numeric_limits<LargeInteger::Bytes<_Length>> {
	constexpr LargeInteger::Bytes<_Length> max() noexcept { return LargeInteger::~Bytes<_Length>(0); }
	constexpr LargeInteger::Bytes<_Length> min()noexcept { return LargeInteger::Bytes<_Length>(0); }
};