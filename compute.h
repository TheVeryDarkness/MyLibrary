#pragma once

#include <list>
#include <forward_list>

#include "Statistics.h"
#include "Shared.h"

namespace LL {
	template<typename Data>
	class UIntList
	{
	public:
		UIntList(const std::initializer_list<Data>& list) :data(new std::forward_list<Data>(list)) {
			if (!CheckUnsigned<Data, -1>()) { PtrDelete(data); throw std::exception("Unsigned interger type is needed."); }
		}
		~UIntList() { PtrDelete(data); }

	private:
		std::forward_list<Data>* data = nullptr;
	};
}