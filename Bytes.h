#pragma once

#include <memory>
#include "ComputeTemplate.h"
#include "Shared.h"

namespace Array {
	constexpr size_t BitsPerByte = 8;
	template<size_t Length>

	class Bytes;

	template<typename Data>
	constexpr size_t GetMinLength(Data data);

	template<size_t Length>
	class BytesTraits;
	//***************************************************
	//***************************************************
	//
	//
	//***************************************************
	//***************************************************



	template<size_t Length = 1>
	class Bytes
	{
	public:
		using value_type=unsigned char;
	private:
		value_type Byte[Length] = {};
	public:
		explicit __stdcall Bytes() {}
		template<size_t OriginLength> explicit __stdcall Bytes(const Bytes<OriginLength>&);
		template<typename Data>explicit  __stdcall Bytes(const Data data);
		template<typename Data> __stdcall operator Data();
		size_t __stdcall GetLength()const;
		Bytes& __stdcall operator=(unsigned char Value) {
			if constexpr (Length != 0)
			{
				memset(this->Byte, 0, Length);
				this->Byte[0] = Value;
			}
			return *this;
		}
		Bytes& __stdcall operator+=(const Bytes& that) {
			if constexpr (Length != 0)
			{
				for (size_t i = 0; i < Length; i++)
				{
					//If (a + b) overflows, then (a + b) < min{a, b}.
					Byte[i + 1] += ((Byte[i] + that.Byte[i] < Byte[i]) ? 1 : 0);
					Byte[i] += that.Byte[i];
				}
				Byte[Length-1] += that.Byte[Length-1];
			}
			return *this;
		}
		Bytes __stdcall operator+(const Bytes& that) const{
			Bytes Ret = *this;
			return (Ret += that);
		}
		Bytes& __stdcall operator/=(const Bytes& that){
			Bytes Res;
			LongCompute::DivideInto<value_type, value_type, BytesTraits<Length>>(*Res.Byte, *that.Byte, *this->Byte);
			return Res;
		}
		Bytes __stdcall operator/(const Bytes& that)const {
			Bytes Ret = *this;
			return (Ret /= that);
		}
		Bytes& __stdcall operator%=(const Bytes& that){
			Bytes Res;
			LongCompute::DivideInto<value_type,value_type,BytesTraits<Length>>(*Res.Byte, *that.Byte, *this->Byte);
			return *this;
		}
		Bytes __stdcall operator%(const Bytes& that)const {
			Bytes Ret = *this;
			return (Ret %= that);
		}
		Bytes& __stdcall operator-=(const Bytes& that){
			Bytes Minus = ~that + Bytes(1);
			*this += Minus;
		}
		Bytes __stdcall operator-(const Bytes& that)const {
			Bytes Ret = *this;
			return (Ret -= that);
		}
		Bytes& __stdcall operator|=(const Bytes& that){
			for (size_t i = 0; i < Length; i++)
			{
				this->Byte[i] |= that.Byte[i];
			}
			return *this;
		}
		Bytes __stdcall operator|(const Bytes& that)const {
			Bytes ret = *this;
			return (ret |= that);
		}
		Bytes& __stdcall operator&=(const Bytes& that) {
			for (size_t i = 0; i < Length; i++)
			{
				this->Byte[i] &= that.Byte[i];
			}
			return *this;
		}
		Bytes __stdcall operator&(const Bytes& that)const {
			Bytes ret = *this;
			return (ret &= that);
		}
		Bytes __stdcall operator~ () {
			Bytes ret;
			for (size_t i = 0; i < Length; i++)
			{
				ret.Byte[i] = ~this->Byte[i];
			}
			return *this;
		}
		Bytes __stdcall operator^=(const Bytes& that) {
			for (size_t i = 0; i < Length; i++)
			{
				this->Byte[i] ^= that.Byte[i];
			}
			return *this;
		}
		Bytes __stdcall operator^(const Bytes& that)const{
			Bytes ret = *this;
			return (ret ^= that);
		}
		Bytes& __stdcall operator<<=(size_t Bits) {
			return(*this = *this << Bits);
		}
		Bytes& __stdcall operator>>=(size_t Bits) {
			return(*this = *this >> Bits);
		}
		Bytes __stdcall operator<<(size_t Bits)const {
			if ((Bits / Length) >= BitsPerByte)
			{
				return Bytes();
			}
			Bytes ret;
			if constexpr(Length!=0)
			{
				if (Bits >= BitsPerByte)
				{
					memcpy(ret.Byte + Bits / BitsPerByte, this->Byte, Length - Bits / BitsPerByte);
				}
				if ((Bits % BitsPerByte)!= 0)
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
		Bytes __stdcall operator>>(size_t Bits)const {
			if ((Bits / Length) >= BitsPerByte)
			{
				return Bytes();
			}
			Bytes ret;
			if constexpr(Length!=0)
			{
				if (Bits >= BitsPerByte)
				{
					memcpy(ret.Byte, this->Byte + Bits / BitsPerByte, Length - Bits / BitsPerByte);
				}
				if ((Bits % BitsPerByte)!= 0)
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
		bool __stdcall operator<(const Bytes& that)const{
			if (Length == 0)return false;
			for (size_t i = Length-1; i > 0; i--)
			{
				if (this->Byte[i] < that.Byte[i])
				{
					return true;
				}
				else if (this->Byte[i] > that.Byte[i])
				{
					return false;
				}
				else continue;
			}
			return false;
		}
		bool __stdcall operator<=(const Bytes& that)const {
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
				else continue;
			}
			return true;
		}
		bool __stdcall operator>(const Bytes& that)const {
			return !(*this <= that);
		}
		bool __stdcall operator>=(const Bytes& that)const {
			return !(*this < that);
		}
		const value_type& __stdcall operator[](size_t index) { return this->Byte[index]; }
		__stdcall Bytes(const std::initializer_list<value_type>& that);
	};
	//***************************************************
	template<size_t Length>
	template<size_t OriginLength>
	inline __stdcall Bytes<Length>::Bytes(
		const Bytes<OriginLength>& that
	) {
		if constexpr (Length >= OriginLength)
			memcpy(&this->Byte, &that.Byte, OriginLength);
		else
			memcpy(&this->Byte, &that.Byte, Length);
		return;
	}
	template<size_t Length>
	inline __stdcall Array::Bytes<Length>::Bytes(
		const std::initializer_list<value_type>& that
	) {
		size_t a = 0;
		for (auto i:that)
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
	inline __stdcall Array::Bytes<Length>::Bytes(Data data) {
		if constexpr (sizeof(Data)<=Length)
		{
			memcpy(this->Byte, &data, sizeof(Data));
		}
		else
		{
			memcpy(this->Byte, &data, Length);
		}
	}

	template<size_t Length>
	template<typename Data>
	inline __stdcall Array::Bytes<Length>::operator Data () {
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
	size_t __stdcall Bytes<Length>::GetLength()const{
		size_t Res = 0;
		Bytes<Length> This = *this;
		while (This!=0)
		{
			This >>= 1;
			Res++;
		}
		return Res;
	}


	template<typename Data>
	constexpr inline size_t GetMinLength(Data data) {
		size_t res = 1;
		do{
			res++;
		} while ((data >>= 8) != 0);
		return res;
	}


	template<size_t Length>
	class BytesTraits:virtual public Bytes<Length>
	{
	public:
		BytesTraits() = delete;
		~BytesTraits() = delete;
		struct BytesIterator
		{
			Bytes<Length>* Head;
			size_t Index;
		};

		BytesIterator __stdcall GetNext(BytesIterator ptr){
			if (ptr.Head != nullptr)
			{

			}
			else return nullptr;
		}
	private:

	};
};