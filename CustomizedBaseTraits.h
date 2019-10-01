#pragma once

#include <iostream>

namespace LargeInteger {

	template<typename _Elem, typename index_type, index_type BeginIndex, _Elem... set>
	class BaseSet;

	template<typename _Elem, typename index_type, index_type BeginIndex>
	class BaseSet<_Elem, index_type, BeginIndex>
	{
	public:
		MY_LIB BaseSet() = delete;
		MY_LIB ~BaseSet() = delete;
		using CharType=_Elem;
		using IntType=index_type;
		constexpr static index_type MY_LIB to_int_type(_Elem Char)noexcept {
			return index_type('?');
		}
		constexpr static _Elem MY_LIB to_char_type(index_type Int)noexcept {
			return _Elem('?');
		}
		constexpr static index_type MY_LIB getRadix()noexcept {
			return BeginIndex;
		}
	private:

	};

	//Mind that '?' is reserved for unknown input
	template<typename _Elem, typename index_type, index_type BeginIndex, _Elem Head, _Elem... Remained>
	class BaseSet<_Elem, index_type, BeginIndex, Head, Remained...> :public BaseSet<_Elem, index_type, BeginIndex + 1, Remained...>
	{
	public:
		MY_LIB BaseSet() = delete;
		MY_LIB ~BaseSet() = delete;
		using CharType=_Elem;
		using IntType=index_type;
		using char_type=_Elem;
		using int_type=index_type;
		constexpr static index_type MY_LIB to_int_type(char_type Char)noexcept {
			if (Char == Head)
			{
				return index_type(BeginIndex);
			}
			else
			{
				return BaseSet<char_type, index_type, BeginIndex + 1, Remained...>::to_int_type(Char);
			}
		}
		constexpr static char_type MY_LIB to_char_type(int_type Int)noexcept {
			if (Int == BeginIndex)
			{
				return char_type(Head);
			}
			else
			{
				return BaseSet<char_type, index_type, BeginIndex + 1, Remained...>::to_char_type(Int);
			}
		}
		constexpr static int_type MY_LIB getRadix()noexcept {
			return BaseSet<char_type, index_type, BeginIndex + 1, Remained...>::getRadix();
		}
	private:

	};


	typedef BaseSet < char, unsigned char, 0, '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'> Set16;
	typedef BaseSet < char, unsigned char, 0, '0', '1', '2', '3', '4', '5', '6', '7', '8'> Set8;
	typedef BaseSet < char, unsigned char, 0, '0', '1'> Set2;
	typedef BaseSet < char, unsigned char, 0, '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'> Set10;

	template<auto Radix>
	class Set
	{
	public:
		Set() = delete;
		~Set() = delete;
	};

	template<>class Set<2> :public Set2 { public:	Set() = delete;	~Set() = delete; };
	template<>class Set<8> :public Set8{ public:	Set() = delete;	~Set() = delete; };
	template<>class Set<10> :public Set10{ public:	Set() = delete;	~Set() = delete; };
	template<>class Set<16> :public Set16{ public:	Set() = delete;	~Set() = delete; };

	template<typename _Elem, typename index_type, size_t BeginIndex, _Elem... set>
	class std::basic_istream<_Elem, BaseSet<_Elem, index_type, BeginIndex, set...>>
	{
		using charset=LargeInteger::BaseSet<_Elem, index_type, BeginIndex, set...>;
	public:
		basic_istream(std::basic_istream<_Elem>& i)noexcept :is(i) {}

		~basic_istream() = default;


		template<typename iter, typename Cntnr, auto radix>
		static void MY_LIB store(iter temp, Cntnr str)noexcept {
			static_assert(
				std::is_same_v<
				decltype(std::remove_cvref_t<*temp>),
				decltype(std::remove_cvref_t<*str>)
				>);
			static_assert(GetPowerTimes(radix, charset::getRadix()) != 0 || radix == charset::getRadix());
			if constexpr (radix == charset::getRadix())
			{
				for (auto i = temp.crbegin(); i != temp.crend(); ++i) {
					auto c = charset::to_int_type(*i);
					if (c != '?')
					{
						*str = c;
						if (i + 1 != temp.crend())
						{
							++str;
						}
					}
				}
				return;
			}
			else
			{
				for (auto i = temp.crbegin(); i != temp.crend();) {
					typename std::remove_reference<decltype(*str)>::type sum = std::remove_reference<decltype(*str)>::type(0);
					for (decltype(GetPowerTimes(radix, charset::getRadix())) j = 0; j < GetPowerTimes(radix, charset::getRadix()); j++) {
						auto c = charset::to_int_type(*i);
						if (c != charset::IntType('?'))
						{
							sum += c * static_cast<decltype(radix)>(Power(static_cast<decltype(radix)>(charset::getRadix()), j));
						}
						else --j;
						++i;
						if (i == temp.crend())
						{
							break;
						}
					}
					if (sum != static_cast<decltype(sum)>(0))
					{
						*str = sum;
						if (i != temp.crend())
						{
							++str;
						}
					}
				}
				return;
			}
		}

	private:
		std::basic_istream<_Elem>& is;
	};
	
	template<typename _Elem, typename index_type, index_type BeginIndex, _Elem... set>
	class std::basic_ostream<_Elem, BaseSet<_Elem, index_type, BeginIndex, set...>>
	{
		using charset=LargeInteger::BaseSet<_Elem, index_type, BeginIndex, set...>;
	public:
		basic_ostream(std::basic_ostream<_Elem>& o)noexcept :os(o) {}

		~basic_ostream() = default;

		MY_LIB operator std::basic_ostream<_Elem>& () noexcept { return os; }
		MY_LIB operator const std::basic_ostream<_Elem>& () const noexcept { return os; }

		template<typename Cntnr>
		std::basic_ostream<_Elem, charset> &MY_LIB Print(Cntnr &that) {
			if constexpr(std::is_arithmetic_v<Cntnr>) {
				auto &&res = that / charset::getRadix();
				if (res != 0) {
					Print(res);
				}
				os << that % charset::getRadix();
			}
			else {
				auto &&res = that.Divide(charset::getRadix());
				if (res != 0) {
					Print(res);
				}
				os << that.GetValue<size_t>();
				res.destruct();
			}
			return *this;
		}

		template<typename Cntnr>
		auto &MY_LIB operator<<(const Cntnr& it) {
			if constexpr (GetPowerTimes(Cntnr::getRadix(), charset::getRadix()) != 0 || Cntnr::getRadix() == charset::getRadix()) {
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
						for (decltype(GetPowerTimes(Cntnr::getRadix(), charset::getRadix())) j = 0; j < GetPowerTimes(Cntnr::getRadix(), charset::getRadix()); j++) {
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
		std::basic_ostream<_Elem>& os;
	};
}