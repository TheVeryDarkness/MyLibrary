#pragma once

#include "Shared.h"
#include <ctime>
#include <forward_list>

namespace Algorithm {
	template<typename Gene, class Fitness>
	class GA {
	public:
		constexpr INLINED MY_LIBRARY GA() noexcept { 
			time_t t;
			time(&t);
			srand(static_cast<unsigned>(t));
			srand(rand());
		}
		INLINED MY_LIBRARY ~GA() noexcept { }
		constexpr INLINED void MY_LIBRARY func() noexcept {

			return;
		}
		constexpr INLINED void MY_LIBRARY revolute() noexcept {
			Fitness f;
			return;
		}
	private:
		std::forward_list<GA> Group;
	};

}