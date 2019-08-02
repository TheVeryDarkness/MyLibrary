#pragma once

#include "Shared.h"
#include <cassert>
#include <limits>
#include <type_traits>

namespace LargeInteger {
	//0 radix stands for common radix.
	template<typename Data, Data Radix>
	class Num
	{
	public:
		constexpr const Data& operator()()const noexcept{ return this->data; }
		constexpr MY_LIBRARY Num(Data init)noexcept
			:data((Radix == Data(0)) ? (init) : (init % Radix)) {
			static_assert(std::is_integral<Data>::value, "Integral required");
			static_assert(Data(-1) > Data(0), "Unsigned type required");
		}
		explicit constexpr MY_LIBRARY operator const Data&()const noexcept{
			return data;
		}
		explicit constexpr MY_LIBRARY operator Data&()noexcept{
			return data;
		}
		MY_LIBRARY ~Num()noexcept {}
		constexpr Num MY_LIBRARY ComplementCodeAsMinus()const noexcept{
			if constexpr (Radix != Data(0)) {
				if (data != Data(0))
				{
					return Num(Radix - data);
				}
				else return Num(0);
			}
			else {
				return Num(~this->data + Data(1));
			}
		}
		constexpr Num MY_LIBRARY operator~() const noexcept {
			if constexpr (Radix == Data(0))
			{
				return Num(~data);
			}
			else return Num((Radix - (Data(1))) - data);
		}
		constexpr bool MY_LIBRARY OverFlowInAdding(const Num& that)const noexcept { return (this->data > (~that).data); }
		constexpr bool MY_LIBRARY UnderFlowInSubtracting(const Num& that)const noexcept { return (this->data < that.data); }
		constexpr bool MY_LIBRARY operator!()const noexcept {
			return (this->data == 0);
		}
		constexpr bool MY_LIBRARY operator==(const Num& that)const noexcept { return (this->data == that.data); }
		constexpr bool MY_LIBRARY operator!=(const Num& that)const noexcept { return (this->data != that.data); }
		constexpr bool MY_LIBRARY operator==(const Data& data)const noexcept { return (this->data == data); }
		constexpr Num& MY_LIBRARY operator=(const Num& that)noexcept { this->data = that.data; return*this; }
		constexpr Num& MY_LIBRARY operator+=(const Num& that) noexcept {
			if (OverFlowInAdding(that))
			{
				return (*this -= that.ComplementCodeAsMinus());
			}
			else {
				this->data += that.data;
				return *this;
			}
		}
		constexpr Num MY_LIBRARY operator+(const Num& that)const noexcept {
			Num Copy = *this;
			return (Copy += that);
		}
		constexpr Num& MY_LIBRARY operator-=(const Num& that) noexcept {
			if (this->UnderFlowInSubtracting(that))
			{
				return (*this += that.ComplementCodeAsMinus());
			}
			else {
				this->data -= that.data;
				return *this;
			}
		}
		constexpr Num MY_LIBRARY operator-(const Num& that)const noexcept {
			Num Copy = *this;
			return (Copy -= that);
		}
		constexpr Num& MY_LIBRARY operator/=(const Num& that)noexcept {
			this->data /= that.data;
			return *this;
		}
		constexpr Num& MY_LIBRARY operator%=(const Num& that) noexcept {
			this->data %= that.data;
			return *this;
		}
		constexpr Num MY_LIBRARY operator/(const Num& that) noexcept {
			Num Copy(*this);
			return (Copy /= that);
		}
		constexpr Num MY_LIBRARY operator%(const Num& that) noexcept {
			Num Copy(*this);
			return (Copy %= that);
		}
		//Be careful when using as it never checks flow
		constexpr Num& MY_LIBRARY operator--() { this->data--; return*this; }
		//Be careful when using as it never checks flow
		constexpr Num& MY_LIBRARY operator++() { this->data++; return*this; }
		constexpr bool MY_LIBRARY operator>(const Num& that)const noexcept { return (this->data > that.data); }
		constexpr bool MY_LIBRARY operator>=(const Num& that)const noexcept { return (this->data >= that.data); }
		constexpr bool MY_LIBRARY operator<(const Num& that)const noexcept { return (this->data < that.data); }
		constexpr bool MY_LIBRARY operator<=(const Num& that)const noexcept { return (this->data <= that.data); }
		constexpr bool MY_LIBRARY operator>(const Data& data)const noexcept { return (this->data > data); }
		constexpr bool MY_LIBRARY operator>=(const Data& data)const noexcept { return (this->data >= data); }
		constexpr bool MY_LIBRARY operator<(const Data& data)const noexcept { return (this->data < data); }
		constexpr bool MY_LIBRARY operator<=(const Data& data)const noexcept { return (this->data <= data); }
		constexpr Num& MY_LIBRARY operator*=(const Num& that) noexcept {
			assert(false);
			if (Radix == Data(0))
			{
				throw;
			}
			else
			{
				if (Radix > std::numeric_limits<Data>::max() / Radix)
				{
					throw;
				}
				else
				{
					this->data *= that.data;
					return *this;
				}
			}
		}
		constexpr Num MY_LIBRARY operator*(const Num& that)const noexcept {
			Num Copy = *this;
			return (Copy *= that);
		}
		constexpr static void MY_LIBRARY Swap(Num& a,Num&b)noexcept {
			a.data ^= b.data; b.data ^= a.data; a.data ^= b.data;
		}
		template<class os>
		friend os& operator<<(os& o, const Num& n) noexcept {
			return (o << n.data);
		}
	private:
		Data data;
	};
}
template<typename Data, Data Radix>class std::numeric_limits<LargeInteger::Num<Data, Radix>> {
	constexpr LargeInteger::Num<Data, Radix> max() noexcept { return ~LargeInteger::Num<Data, Radix>(0); }
	constexpr LargeInteger::Num<Data, Radix> min() noexcept { return LargeInteger::Num<Data, Radix>(0); }
};