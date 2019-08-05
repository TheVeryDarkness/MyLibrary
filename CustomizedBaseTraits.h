#pragma once

#include <istream>
#include <iterator>

namespace LargeInteger {

	template<typename char_type, typename int_type, size_t BeginIndex, char_type... set>
	class BaseSet;

	template<typename char_type, typename int_type, size_t BeginIndex>
	class BaseSet<char_type, int_type, BeginIndex>
	{
	public:
		MY_LIBRARY BaseSet() = delete;
		MY_LIBRARY ~BaseSet() = delete;
		using CharType=char_type;
		using IntType=int_type;
		constexpr static int_type MY_LIBRARY to_int_type(char_type Char)noexcept {
			return int_type('?');
		}
		constexpr static char_type MY_LIBRARY to_char_type(int_type Int)noexcept {
			return char_type('?');
		}
		constexpr static int_type MY_LIBRARY getRadix()noexcept {
			return BeginIndex;
		}
	private:

	};

	//Mind that '?' is reserved for unknown input
	template<typename char_type, typename int_type, size_t BeginIndex, char_type Head, char_type... Remained>
	class BaseSet<char_type, int_type, BeginIndex, Head, Remained...> :public BaseSet<char_type, int_type, BeginIndex + 1, Remained...>
	{
	public:
		MY_LIBRARY BaseSet() = delete;
		MY_LIBRARY ~BaseSet() = delete;
		using CharType=char_type;
		using IntType=int_type;
		constexpr static int_type MY_LIBRARY to_int_type(char_type Char)noexcept {
			if (Char == Head)
			{
				return int_type(BeginIndex);
			}
			else
			{
				return BaseSet<char_type, int_type, BeginIndex + 1, Remained...>::to_int_type(Char);
			}
		}
		constexpr static char_type MY_LIBRARY to_char_type(int_type Int)noexcept {
			if (Int == BeginIndex)
			{
				return char_type(Head);
			}
			else
			{
				return BaseSet<char_type, int_type, BeginIndex + 1, Remained>::to_char_type(Int);
			}
		}
		constexpr static int_type MY_LIBRARY getRadix()noexcept {
			return BaseSet<char_type, int_type, BeginIndex + 1, Remained...>::getRadix();
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

	template<typename _Elem, typename char_type, typename int_type, size_t BeginIndex, char_type... set>
	class std::basic_istream<_Elem, BaseSet<char_type, int_type, BeginIndex, set...>>
	{
		using charset=LargeInteger::BaseSet<char_type, int_type, BeginIndex, set...>;
	public:
		basic_istream(std::basic_istream<_Elem>& i)noexcept :is(i) {}

		~basic_istream() = default;


		template<typename Iter>
		static void MY_LIBRARY store(const std::basic_string<_Elem>& temp, Iter str)noexcept {
			static_assert(GetPowerTimes(Iter::getRadix(), charset::getRadix()) != 0 || Iter::getRadix() == charset::getRadix());
			if constexpr (Iter::getRadix() == charset::getRadix())
			{
				for (auto i = temp.crbegin(); i != temp.crend(); ++i) {
					auto c = charset::to_int_type(*i);
					if (c != '?')
					{
						++str;
						*str = c;
					}
				}
				return;
			}
			else
			{
				for (auto i = temp.crbegin(); i != temp.crend();) {
					typename std::remove_reference<decltype(*str)>::type sum = std::remove_reference<decltype(*str)>::type(0);
					for (decltype(GetPowerTimes(Iter::getRadix(), charset::getRadix())) j = 0; j < GetPowerTimes(Iter::getRadix(), charset::getRadix()); j++) {
						auto c = charset::to_int_type(*i);
						if (c != charset::IntType('?'))
						{
							sum += c * static_cast<decltype(Iter::getRadix())>(Power(static_cast<decltype(Iter::getRadix())>(charset::getRadix()), j));
						}
						else --j;
						i++;
						if (i == temp.crend())
						{
							break;
						}
					}
					if (sum != static_cast<decltype(sum)>(0))
					{
						++str;
						*str = sum;
					}
				}
				return;
			}
		}

	private:
		std::basic_istream<_Elem>& is;
	};
};