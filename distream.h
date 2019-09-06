#pragma once
#include "CustomizedBaseTraits.h"

#ifdef _WIN32
#include <conio.h>

namespace LargeInteger {
	enum source {
		kb
	};
	template<source s,typename charset>
	class din {
	public:
		din() noexcept { }
		~din() noexcept { }

		template<typename Iter>
		din& operator>>(Iter it) {
			while (true) {
				auto&& c = _getch();
				switch (c) {
				case '\n':
				case '\r':
					return *this;
				default:
					break;
				}
				auto&& i = charset::to_int_type(c);
				if (i != '?') {
					*it = i;
					++it;
				}
			}
		}
	};
}
#endif // _WIN32