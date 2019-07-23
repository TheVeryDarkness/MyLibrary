#pragma once

#include "Shared.h"
#include <type_traits>

namespace LargeInteger {
	//0 radix stands for common radix.
	template<typename Data, Data Radix>
	class Num
	{
	public:
		constexpr MY_LIBRARY Num(Data init) :data(data) {
			static_assert(std::is_integral<Data>::value,"Integral required");
		}
		MY_LIBRARY ~Num(){}
		Data MY_LIBRARY ComplementCodeAsMinus()const {
			if constexpr(data != Data(0))
			{
				return (Radix - data);
			}
			else return Data(0);
		}
		Num MY_LIBRARY operator~() const{
			if constexpr(Radix==Data(0))
			{
				return Num(~data);
			}
			else return Num((Radix - (Data(1))) - data);
		}
		bool MY_LIBRARY OverFlowInAdding(const Num& that){return (this->data > (~that).data);}
		bool MY_LIBRARY operator==(const Num& that)const {return (this->data == that.data);}
		Num& MY_LIBRARY operator+=(const Num& that) {
			if (OverFlowInAdding(that))
			{

			}
			else return (this->data += that.data);
		}

	private:
		Data data;
	};
}
