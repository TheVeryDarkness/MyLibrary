#pragma once

#include <memory>
#include "Shared.h"
namespace Array {
	template<size_t Length = 1, bool Positive = true>
	class UChar
	{
	private:
		unsigned char Data[Length];
	public:
		template<size_t OriginLength> explicit __stdcall UChar(
			const UChar<OriginLength, Positive>&
		);
		template<bool OriginPositive> explicit __stdcall UChar(
			const UChar<Length, OriginPositive>&
		);
		UChar& operator=(unsigned char Value) {
			if constexpr (Length != 0)
			{
				memset(this->Data, 0, Length);
				this->Data[0] = Value;
			}
			return *this;
		}
		UChar& operator+=(const UChar& that) {
			if constexpr (Length != 0)
			{
				for (unsigned int i = 0; i < Length - 1; i++)
				{
					Data[i + 1] += (
						(Data[i] > UCHAR_MAX - that.Data[i]) ? 1 : 0
						);
					Data[i] += that.Data[i];
				}
				Data[Length] += that.Data[Length];
			}
			return *this;
		}
	};

	template<size_t Length, bool Positive>
	template<size_t OriginLength>
	__stdcall UChar<Length,Positive>::UChar(
		const UChar<OriginLength,Positive>& that
	) {
		if constexpr (Length >= OriginLength)
			memcpy(&this->Data, &that.Data, OriginLength);
		else
			memcpy(&this->Data, &that.Data, Length);
		return;
	}

	template<size_t Length, bool Positive>
	template<bool OriginPositive>
	__stdcall  UChar<Length, Positive>::UChar(
		const UChar<Length, OriginPositive>& that
	) {
		memcpy(&this->Data, &that.data, Length);
	}
};