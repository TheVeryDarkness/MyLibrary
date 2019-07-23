#pragma once

#include "Shared.h"

namespace LargeInteger {
	//0 radix stands for common radix.
	template<typename Data, Data Radix>
	class Num
	{
	public:
		Num(Data init) :data(data) {}
		~Num(){}
		Data MY_LIBRARY ComplementCodeAsMinus()const {
			if (data != Data(0))
			{
				return (Radix - data);
			}
			else return Data(0);
		}
		Num MY_LIBRARY operator~() const{
			return Num((Radix - (Data(1))) - data);
		}
		bool MY_LIBRARY OverFlowInAdding(const Num& that){
			return (this->data > (~that).data);
		}


	private:
		Data data;
	};
}
