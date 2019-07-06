#pragma once

#include <memory>
#include "Shared.h"

namespace Array {
	constexpr size_t BitsPerByte = 8;
	template<size_t Length>
	class Bytes;
	template<typename Data>
	constexpr size_t GetLength(Data data);
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
		template<typename Data> inline __stdcall Bytes(const Data data);
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
				Byte[Length] += that.Byte[Length];
			}
#ifdef _DEBUG

#endif // _DEBUG
			return *this;
		}
		Bytes& __stdcall operator+(const Bytes& that) const{
			Bytes Ret = *this;
			return (Ret += that);
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
		const value_type& operator[](size_t index) { return this->Byte[index]; }
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


	template<typename Data>
	constexpr inline size_t GetLength(Data data) {
		size_t res = 1;
		do{
			res++;
		} while ((data >>= 8) != 0);
		return res;
	}
};