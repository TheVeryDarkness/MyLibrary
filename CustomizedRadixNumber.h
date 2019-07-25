#pragma once

#include "Shared.h"
#include <type_traits>

namespace LargeInteger {
	//0 radix stands for common radix.
	template<typename Data, Data Radix>
	class Num
	{
	public:
		constexpr const Data& operator()()const noexcept{ return this->data; }
		constexpr MY_LIBRARY Num(Data init)noexcept :data(init) {
			static_assert(std::is_integral<Data>::value, "Integral required");
		}
		explicit constexpr MY_LIBRARY operator const Data&()const noexcept{
			return data;
		}
		explicit constexpr MY_LIBRARY operator Data&()noexcept{
			return data;
		}
		MY_LIBRARY ~Num()noexcept {}
		Num MY_LIBRARY ComplementCodeAsMinus()const noexcept{
			 (Radix == Data(9)){
			if (data != Data(0))
			{
				return Num(Radix - data);
			}
			else return Num(0);
			}
		}
		Num MY_LIBRARY operator~() const noexcept {
			if constexpr (Radix == Data(0))
			{
				return Num(~data);
			}
			else return Num((Radix - (Data(1))) - data);
		}
		bool MY_LIBRARY OverFlowInAdding(const Num& that)const noexcept { return (this->data > (~that).data); }
		bool MY_LIBRARY UnderFlowInSubtracting(const Num& that)const noexcept { return (this->data < that.data); }
		bool MY_LIBRARY operator!()const noexcept {
			return (this->data == 0);
		}
		bool MY_LIBRARY operator==(const Num& that)const noexcept { return (this->data == that.data); }
		bool MY_LIBRARY operator!=(const Num& that)const noexcept { return (this->data != that.data); }
		bool MY_LIBRARY operator==(const Data& data)const noexcept { return (this->data == data); }
		Num& MY_LIBRARY operator=(const Num& that)noexcept { this->data = that.data; return*this; }
		Num& MY_LIBRARY operator+=(const Num& that) noexcept {
			if (OverFlowInAdding(that))
			{
				return (*this -= that.ComplementCodeAsMinus());
			}
			else {
				this->data += that.data;
				return *this;
			}
		}
		Num MY_LIBRARY operator+(const Num& that)const noexcept {
			Num Copy = *this;
			return (Copy += that);
		}
		Num& MY_LIBRARY operator-=(const Num& that) noexcept {
			if (this->UnderFlowInSubtracting(that))
			{
				return (*this += that.ComplementCodeAsMinus());
			}
			else {
				this->data -= that.data;
				return *this;
			}
		}
		Num MY_LIBRARY operator-(const Num& that)const noexcept {
			Num Copy = *this;
			return (Copy -= that);
		}
		Num& MY_LIBRARY operator/=(const Num& that)noexcept {
			this->data /= that.data;
			return *this;
		}
		Num& MY_LIBRARY operator%=(const Num& that) noexcept {
			this->data %= that.data;
			return *this;
		}
		Num MY_LIBRARY operator/(const Num& that) noexcept {
			Num Copy(*this);
			return (Copy /= that);
		}
		Num MY_LIBRARY operator%(const Num& that) noexcept {
			Num Copy(*this);
			return (Copy %= that);
		}
		//Be careful when using as it never checks flow
		Num& MY_LIBRARY operator--() { this->data--; return*this; }
		//Be careful when using as it never checks flow
		Num& MY_LIBRARY operator++() { this->data++; return*this; }
		bool MY_LIBRARY operator>(const Num& that)const noexcept { return (this->data > that.data); }
		bool MY_LIBRARY operator>=(const Num& that)const noexcept { return (this->data >= that.data); }
		bool MY_LIBRARY operator<(const Num& that)const noexcept { return (this->data < that.data); }
		bool MY_LIBRARY operator<=(const Num& that)const noexcept { return (this->data <= that.data); }
		bool MY_LIBRARY operator>(const Data& data)const noexcept { return (this->data > data); }
		bool MY_LIBRARY operator>=(const Data& data)const noexcept { return (this->data >= data); }
		bool MY_LIBRARY operator<(const Data& data)const noexcept { return (this->data < data); }
		bool MY_LIBRARY operator<=(const Data& data)const noexcept { return (this->data <= data); }
		Num& MY_LIBRARY operator*=(const Num& that) noexcept {
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
					this->data %= Radix;
					return *this;
				}
			}
		}
		Num MY_LIBRARY operator*(const Num& that)const noexcept {
			Num Copy = *this;
			return (Copy *= that);
		}
		template<class os>
		friend os& operator<<(os& o, const Num& n) noexcept {
			return (o << n.data);
		}
	private:
		Data data;
	};
}
#undef max
#undef min
template<typename Data, Data Radix>class std::numeric_limits<LargeInteger::Num<Data, Radix>> {
	constexpr LargeInteger::Num<Data, Radix> max() noexcept { return ~Num<Data, Radix>(0); }
	constexpr LargeInteger::Num<Data, Radix> min() noexcept { return Num<Data, Radix>(0); }
};

