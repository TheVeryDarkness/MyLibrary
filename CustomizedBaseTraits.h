#pragma once

#include <iostream>
#include "LargeInteger.h"
#include <iterator>

namespace LargeInteger {

	template<typename _Elem, typename index_type, _Elem... set>
	class BaseSet;
	template<typename T1, typename T2>class custom_istream {
	public:
		template<typename... T>custom_istream(T &... t) {
			((std::cerr << typeid(t).name()) << ' ', ...);
			std::cerr << std::endl << typeid(*this).name() << ' ' << __func__ << std::endl;
		}
		template<typename _T1, typename _T2, auto t>void store(...)noexcept {
			std::cerr
				<< typeid(_T1).name() << ' '
				<< typeid(_T2).name() << ' '
				<< typeid(t).name() << ' '
				<< typeid(*this).name() << ' '
				<< __func__ << std::endl;
		}
	};
	template<typename T1, typename T2>class custom_ostream {
		template<typename... T>custom_ostream(T &... t) {
			((std::cerr << typeid(t).name()) << ' ', ...);
			std::cerr << std::endl << typeid(*this).name() << ' ' << __func__ << std::endl;
		}
	};

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

	template<auto Radix>
	class Set {
	public:
		Set() = delete;
		~Set() = delete;
	};

	template<>class Set<2> :public Set2 { public:	Set() = delete;	~Set() = delete; };
	template<>class Set<8> :public Set8 { public:	Set() = delete;	~Set() = delete; };
	template<>class Set<10> :public Set10 { public:	Set() = delete;	~Set() = delete; };
	template<>class Set<16> :public Set16 { public:	Set() = delete;	~Set() = delete; };


	template<char...Delim>char __stdcall getline(std::istream &in, std::string &str)noexcept {
		using charset = BaseSet<char, char, Delim...>;
		static_assert(sizeof...(Delim) > 0, "Delim should be given");
		while (in.good()) {
			char tmp;
			in >> tmp;
			if (charset::exist(tmp)) {
				return tmp;
			}
			else {
				str.push_back(tmp);
			}
		}
		return '?';
	}


	template<typename _Elem, typename index_type, size_t BeginIndex, _Elem... set>
	class custom_istream<_Elem, LargeInteger::BaseSet<_Elem, index_type, BeginIndex, set...>> {
		using charset = LargeInteger::BaseSet<_Elem, index_type, BeginIndex, set...>;
	public:
		explicit custom_istream(std::basic_istream<_Elem> &i)noexcept :is(i) { }

		~custom_istream() = default;


		template<typename iter, typename Cntnr, auto radix>
		static void MY_LIB store(iter temp, Cntnr str)noexcept {
			static_assert(Math::GetPowerTimes(radix, charset::getRadix()) != 0 || radix == charset::getRadix());
			for (auto i = temp.crbegin(); i != temp.crend() && *i == charset::to_char_type(0);) {
				++i;
				temp.pop_back();
			}
			if constexpr (radix == charset::getRadix()) {
				auto i = temp.crbegin();
				while (*i == '0') {
					++i;
				}
				for (; i != temp.crend(); ++i) {
					auto c = charset::to_int_type(*i);
					if (c != '?') {
						*str = c;
						if (i + 1 != temp.crend()) {
							++str;
						}
					}
				}
				return;
			}
			else {
				for (auto i = temp.crbegin(); i != temp.crend();) {
					typename std::remove_reference<decltype(*str)>::type sum = typename std::remove_reference<decltype(*str)>::type(0);
					for (decltype(Math::GetPowerTimes(radix, charset::getRadix())) j = 0; j < Math::GetPowerTimes(radix, charset::getRadix()); j++) {
						auto c = charset::to_int_type(*i);
						if (c != typename charset::IntType('?')) {
							sum += c * static_cast<decltype(radix)>(Math::Power(static_cast<decltype(radix)>(charset::getRadix()), j));
						}
						else --j;
						++i;
						if (i == temp.crend()) {
							break;
						}
					}
					*str = sum;
					if (i != temp.crend()) {
						++str;
					}
				}
				return;
			}
		}

	private:
		std::basic_istream<_Elem> &is;
	};

	template<typename _Elem, typename index_type, index_type BeginIndex, _Elem... set>
	class custom_ostream<_Elem, LargeInteger::template BaseSet<_Elem, index_type, BeginIndex, set...>> {
	public:
		using charset = LargeInteger::BaseSet<_Elem, index_type, BeginIndex, set...>;
		explicit custom_ostream(std::basic_ostream<_Elem> &o)noexcept :os(o) { }
		~custom_ostream() = default;

		std::basic_ostream<_Elem> &MY_LIB operator()()noexcept { return os; }
		MY_LIB operator std::basic_ostream<_Elem> &() noexcept { return os; }
		MY_LIB operator const std::basic_ostream<_Elem> &() const noexcept { return os; }

		template<typename Cntnr>
		std::basic_ostream<_Elem, charset> &MY_LIB Print(Cntnr &that) {
			if constexpr (std::is_arithmetic_v<Cntnr>) {
				auto &&res = that / charset::getRadix();
				if (res != 0) {
					Print(res);
				}
				os << that % charset::getRadix();
			}
			else {
				using radix_t = decltype(Cntnr::getRadix());
				auto &&res = that.Divide(static_cast<radix_t>(charset::getRadix()));
				if (res != 0) {
					Print(res);
				}
				os << static_cast<size_t>(that.template GetValue<radix_t>());
				res.destruct();
			}
			return *this;
		}

		template<typename LL, auto radix>
		auto &MY_LIB operator<<(const LargeInteger::LargeUnsigned<LL, radix> &it) noexcept {
			using Cntnr = LargeInteger::LargeUnsigned<LL, radix>;
			if constexpr (Math::GetPowerTimes(Cntnr::getRadix(), charset::getRadix()) != 0 || Cntnr::getRadix() == charset::getRadix()) {
				std::ostream_iterator<_Elem> o(os);
				if constexpr (Cntnr::getRadix() == charset::getRadix()) {
					for (auto i = it.crbegin(); i != it.crend(); ++i) {
						auto c = charset::to_char_type(*i);
						if (c != '?') {
							++o;
							*o = c;
						}
					}
					return *this;
				}
				else {
					for (auto i = it.crbegin(); i != it.crend(); ++i) {
						auto val = *i;
						for (decltype(Math::GetPowerTimes(Cntnr::getRadix(), charset::getRadix())) j = 0; j < Math::GetPowerTimes(Cntnr::getRadix(), charset::getRadix()); j++) {
							auto c = charset::to_char_type(val % charset::getRadix());
							val /= charset::getRadix();
							if (c != '?') {
								++o;
								*o = c;
							}
							else --j;
						}
					}
					return *this;
				}
			}
			else {
				Cntnr copy = Cntnr::Copy(it);
				Print(copy);
				copy.destruct();
				return *this;
			}
		}
	private:
		std::basic_ostream<_Elem> &os;
	};

}