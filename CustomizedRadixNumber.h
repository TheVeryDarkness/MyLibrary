#pragma once

#include "Shared.h"
#include <type_traits>

namespace LargeInteger {
	//0 radix stands for common radix.
	template<typename Data, Data Radix>
	class Num
	{
	public:
		explicit constexpr MY_LIBRARY Num(Data init) :data(data) {
			static_assert(std::is_integral<Data>::value, "Integral required");
		}
		constexpr MY_LIBRARY operator Data()const {
			return data;
		}
		MY_LIBRARY ~Num() {}
		Data MY_LIBRARY ComplementCodeAsMinus()const {
			if constexpr (data != Data(0))
			{
				return (Radix - data);
			}
			else return Data(0);
		}
		Num MY_LIBRARY operator~() const {
			if constexpr (Radix == Data(0))
			{
				return Num(~data);
			}
			else return Num((Radix - (Data(1))) - data);
		}
		bool MY_LIBRARY OverFlowInAdding(const Num& that)const { return (this->data > (~that).data); }
		bool MY_LIBRARY UnderFlowInSubtracting(const Num& that)const { return (this->data < that.data); }
		bool MY_LIBRARY operator==(const Num& that)const { return (this->data == that.data); }
		Num& MY_LIBRARY operator+=(const Num& that) {
			if (OverFlowInAdding(that))
			{
				return (*this -= that.ComplementCodeAsMinus);
			}
			else return (this->data += that.data);
		}
		Num MY_LIBRARY operator+(const Num& that)const {
			Num Copy = *this;
			return (Copy += that);
		}
		Num& MY_LIBRARY operator-=(const Num& that) {
			if (this->UnderFlowInSubtracting(that))
			{
				return (*this += that.ComplementCodeAsMinus());
			}
			else {
				this->data -= that.data;
				return *this;
			}
		}
		Num MY_LIBRARY operator-(const Num& that)const {
			Num Copy = *this;
			return (Copy -= that);
		}

	private:
		Data data;
	};


}
#undef max
#undef min
template<typename Data, Data Radix>class std::numeric_limits<LargeInteger::Num<Data, Radix>> {
	constexpr LargeInteger::Num<Data, Radix> max() { return ~Num<Data, Radix>(0); }
	constexpr LargeInteger::Num<Data, Radix> min() { return Num<Data, Radix>(0); }
};
