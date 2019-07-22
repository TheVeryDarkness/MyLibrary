#pragma once
#include "Shared.h"

namespace LargeInteger {
	template<typename Data, Data Radix>
	class Num
	{
	public:
		Num(Data init) :data(data) {}
		~Num(){}
		Data MY_LIBRARY ComplementCode()const { return (Radix - data); }
		void MY_LIBRARY ADD(const Num& that){
			throw;
		}

	private:
		Data data;
	};
}
