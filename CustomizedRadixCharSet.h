#pragma once
#include "Shared.h"
#include <istream>

namespace Darkness {
	inline namespace LargeInteger {
		template<typename char_type, typename int_type, char_type... set>
		class BaseSet;

		//Mind that '?' is reserved for unknown input
		template<typename char_type, typename int_type, char_type... set>
		class BaseSet {
		public:
			MY_LIB BaseSet() = delete;
			MY_LIB ~BaseSet() = delete;
			constexpr static int_type MY_LIB getRadix()noexcept {
				return sizeof...(set);
			}
			constexpr static int_type MY_LIB to_int_type(char_type Char)noexcept {
				size_t Index = static_cast<size_t>(-1);
				return int_type((((Index += 1), (Char == set)) || ...) ? Index : '?');
			}
			constexpr static char_type MY_LIB to_char_type(int_type Int)noexcept {
				constexpr char_type arr[getRadix()] = { set... };
				return (Int < getRadix()) ? arr[Int] : char_type('?');
			}
			constexpr static bool MY_LIB exist(char_type c)noexcept {
				return ((c == set) || ...);
			}
			static_assert(!exist('?'), "'?' is reserved.");
		private:

		};


		typedef BaseSet<char, unsigned char, '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'> Set16;
		typedef BaseSet<char, unsigned char, '0', '1', '2', '3', '4', '5', '6', '7'> Set8;
		typedef BaseSet<char, unsigned char, '0', '1'> Set2;
		typedef BaseSet<char, unsigned char, '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'> Set10;


		template<char...Delim>char MY_LIB getline(std::istream &in, std::string &str)noexcept {
			using charset = BaseSet<char, char, Delim...>;
			static_assert(sizeof...(Delim) > 0, "Delim should be given");
			while (in.good()) {
				char tmp = static_cast<char>(in.peek());
				if (!in || tmp == -1)tmp = 0;
				if (charset::exist(tmp)) {
					return tmp;
				}
				else {
					str.push_back(tmp);
					in.ignore();
				}
			}
			return char('\0');
		}
		template<char...Delim>char MY_LIB ignore_if(std::istream &in)noexcept {
			using charset = BaseSet<char, char, Delim...>;
			static_assert(sizeof...(Delim) > 0, "Delim should be given");
			while (true) {
				char tmp = static_cast<char>(in.peek());
				if (!in) { tmp = 0; return tmp; }
				if (charset::exist(tmp)) in.ignore(); else return tmp;
			}
		}

		template<char...Delim>char MY_LIB ignore_if_not(std::istream &in)noexcept {
			using charset = BaseSet<char, char, Delim...>;
			static_assert(sizeof...(Delim) > 0, "Delim should be given");
			while (true) {
				char tmp = static_cast<char>(in.peek());
				if (!in) { tmp = 0; return tmp; }
				if (!charset::exist(tmp)) in.ignore(); else return tmp;
			}
		}

		template<char...Delim>const char *MY_LIB find(const char *s)noexcept {
			using charset = BaseSet<char, char, Delim...>;
			const char *end = s;
			for (; !charset::exist(*end); ++end);
			return end > s ? end : s;
		}

		template<size_t Radix, typename char_type = char, typename int_type = unsigned char>
		class Set {
		public:
			static_assert(Radix <= 36, "Unsupported radix.");
			Set() = delete;
			~Set() = delete;
			static constexpr bool __stdcall exist(char c) noexcept {
				if constexpr (Radix <= 10) {
					return ('0' <= c) && (c < '0' + Radix);
				}
				else {
					return ('0' <= c && c < '0' + Radix) || ('a' <= c && c < 'a' + Radix - 10);
				}
			}
			static constexpr int_type __stdcall to_int_type(char_type c) noexcept {
				if (exist(c)) {
					if constexpr (Radix <= 10) {
						return ('0' <= c) && (c < '0' + Radix);
					}
					else {
						return ('0' <= c && c < '0' + Radix) || ('a' <= c && c < 'a' + Radix - 10);
					}
				}
				else return static_cast<int_type>(-1);
			}
			static constexpr char_type __stdcall to_char_type(int_type i) noexcept {
				if (i < Radix) {
					if constexpr (Radix <= 10) {
						return '0' + i;
					}
					else {
						return i < 10 ? ('0' + i) : ('a' + i - 10);
					}
				}
				else return static_cast<char_type>(-1);
			}
		};

		class LowerCharSet {
		public:
			using char_type = char;
			using int_type = unsigned char;
			LowerCharSet() = delete;
			~LowerCharSet() = delete;
			static constexpr bool __stdcall exist(char c) noexcept {
				return 'a' <= c && c <= 'z';
			}
		};

		class HigherCharSet {
		public:
			using char_type = char;
			using int_type = unsigned char;
			HigherCharSet() = delete;
			~HigherCharSet() = delete;
			static constexpr bool __stdcall exist(char c) noexcept {
				return 'A' <= c && c <= 'Z';
			}
		};

		template<typename ...CharSet>inline constexpr bool exist(char c)noexcept {
			return (CharSet::exist(c) || ...);
		}
	}
}