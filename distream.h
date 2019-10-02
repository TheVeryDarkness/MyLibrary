#pragma once
#include "CustomizedBaseTraits.h"

#ifdef _WIN32
#include <conio.h>

namespace LargeInteger {
	enum class source {
		kb
	};
	template<source s,typename charset>
	class din;

	template<typename charset>
	class din<source::kb,charset>{
	public:
		din() noexcept { }
		~din() noexcept { }

		template<typename Cntnr>
		din& operator>>(Cntnr it) {
			while (true) {
				auto&& c = _getch();
				switch (c) {
				case ' ':
				case '\n':
				case '\r':
					putch(' ');
					return *this;
				default:
					auto&& i = charset::to_int_type(c);
					if (i != '?') {
						*it = i;
						++it;
						putch(c);
					}
					else { 
						continue;
						return *this; 
					}
				}
			}
			return *this;
		}
	};
}
#endif // _WIN32
