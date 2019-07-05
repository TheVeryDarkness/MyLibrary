#pragma once

#include "Bytes.h"
namespace Array {
	//***************************************************
	//***************************************************
	//
	//
	//***************************************************
	//***************************************************

	template<size_t Length = 1, bool Positive = true>
	class ByteInt:protected Bytes<Length>
	{
	public:
		__stdcall ByteInt(const ByteInt& that) :Byte(that.Byte) {}
		template<size_t OriginLength> explicit __stdcall ByteInt(
			const ByteInt<OriginLength, Positive>&
		);
		template<bool OriginPositive> explicit __stdcall ByteInt(
			const ByteInt<Length, OriginPositive>&
		);
		ByteInt& __stdcall operator=(unsigned char Value) {
			if constexpr (Length != 0)
			{
				memset(this->Byte, 0, Length);
				this->Byte[0] = Value;
			}
			return *this;
		}
		ByteInt& __stdcall operator+=(const ByteInt& that) {
			if constexpr (Length != 0)
			{
				for (size_t i = 0; i < Length; i++)
				{
					Byte[i + 1] += ((Byte[i] > UCHAR_MAX - that.Byte[i]) ? 1 : 0);
					Byte[i] += that.Byte[i];
				}
				Byte[Length] += that.Byte[Length];
			}
			return *this;
		}
		ByteInt __stdcall operator<<(size_t Bits)const {
			ByteInt ret;
			if constexpr (Length != 0)
			{
				for (size_t j = 0; j < ByteInt; j++)
				{
					for (size_t i = 0; i < Length; i++)
					{

					}
				}
			}
			return ret;
		}
		ByteInt<Length, !Positive> __stdcall operator-() {
			return static_cast<ByteInt<Length, !Positive>>(*this);
		}
	};

	template<size_t Length, bool Positive>
	template<size_t OriginLength>
	__stdcall ByteInt<Length, Positive>::ByteInt(
		const ByteInt<OriginLength, Positive>& that
	) {
		if constexpr (Length >= OriginLength)
			memcpy(&this->Byte, &that.Byte, OriginLength);
		else
			memcpy(&this->Byte, &that.Byte, Length);
		return;
	}

	template<size_t Length, bool Positive>
	template<bool OriginPositive>
	__stdcall  ByteInt<Length, Positive>::ByteInt(
		const ByteInt<Length, OriginPositive>& that
	) {
		memcpy(&this->Byte, &that.data, Length);
	}
}