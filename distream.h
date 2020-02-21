#pragma once
#include "CustomizedRadixCharSet.h"

#ifdef _WIN32
#include <conio.h>
#include <Windows.h>

namespace Darkness {
	inline namespace LargeInteger {
		enum class source {
			kb
		};
		template<source s, typename charset>
		class din;

		template<typename charset>
		class din<source::kb, charset> {
			bool isGood = true;
		public:
			din() noexcept { }
			~din() noexcept { }
			bool good()noexcept { return isGood; }

			template<typename Cntnr>
			din &operator>>(const Cntnr &head) {
				Cntnr it(head);
				while (true) {
					auto c = _getch();
					switch (c) {
					case '\n':
					case '\r':
						putch(' ');
						return *this;
					case 224:
						if (it - 1 != nullptr) {
							--it;
							putch('\b');
						}
						continue;
					case 77:
						if (it + 1 != nullptr) {
							++it;
							putch(charset::to_char_type(*it));
						}
						continue;
					case 27:
						isGood = false;
						return *this;
					case '\x8':
						if (it - 1 != nullptr) {
							--it;
							it.pop(it);
							putch('\b');
							putch(' ');
							putch('\b');
						}
						continue;
					default:
						auto &&i = charset::to_int_type(c);
						if (i != '?') {
							it.insert(it, i);
							++it;
							putch(c);
						}
						else {
							continue;
						}
					}
				}
				return *this;
			}
		};
	}
#endif // _WIN32

}