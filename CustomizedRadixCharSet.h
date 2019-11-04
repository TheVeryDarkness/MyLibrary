#pragma once
#include "Shared.h"

namespace LargeInteger{
	template<typename _Elem, typename index_type, _Elem... set>
	class BaseSet;

	//Mind that '?' is reserved for unknown input
	template<typename _Elem, typename index_type, _Elem... set>
	class BaseSet {
	public:
		MY_LIB BaseSet() = delete;
		MY_LIB ~BaseSet() = delete;
		using CharType = _Elem;
		using IntType = index_type;
		using char_type = _Elem;
		using int_type = index_type;
		constexpr static int_type MY_LIB getRadix()noexcept {
			return sizeof...(set);
		}
		constexpr static _Elem arr[getRadix()] = { set... };
		constexpr static index_type MY_LIB to_int_type(char_type Char)noexcept {
			size_t Index = static_cast<size_t>(-1);
			return index_type((((Index += 1), (Char == set)) || ...) ? Index : '?');
		}
		constexpr static char_type MY_LIB to_char_type(int_type Int)noexcept {
			return (Int < getRadix()) ? arr[Int] : char_type('?');
		}
		constexpr static bool MY_LIB exist(char_type c)noexcept {
			return ((c == set) || ...);
		}
		static_assert(!exist('?'), "'?' is reserved.");
	private:

	};


	typedef BaseSet < char, unsigned char, '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'> Set16;
	typedef BaseSet < char, unsigned char, '0', '1', '2', '3', '4', '5', '6', '7'> Set8;
	typedef BaseSet < char, unsigned char, '0', '1'> Set2;
	typedef BaseSet < char, unsigned char, '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'> Set10;

	template<size_t Radix>
	class Set {
	public:
		Set() = delete;
		~Set() = delete;
	};

template<>class Set<2> { public:	using super = Set2; };
template<>class Set<8> { public:	using super = Set8; };
template<>class Set<10> { public:	using super = Set10; };
template<>class Set<16> { public:	using super = Set16; };


	template<char...Delim>char __stdcall getline(std::istream &in, std::string &str)noexcept {
		using charset = BaseSet<char, char, Delim...>;
		static_assert(sizeof...(Delim) > 0, "Delim should be given");
		while (in.good()) {
			char tmp = static_cast<char>(in.get());
			if (!in)tmp = 0;
			if (charset::exist(tmp)) {
				return tmp;
			}
			else {
				str.push_back(tmp);
			}
		}
		return char('\0');
	}	
	template<char...Delim>void __stdcall ignore_if(std::istream &in)noexcept {
		using charset = BaseSet<char, char, Delim...>;
		static_assert(sizeof...(Delim) > 0, "Delim should be given");
		while (true) {
			char tmp = static_cast<char>(in.peek());
			if (!in) { tmp = 0; return; }
			if (charset::exist(tmp)) in.ignore(); else return;
		}
	}
}