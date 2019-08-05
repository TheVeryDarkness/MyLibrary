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

		static int_type MY_LIBRARY to_int_type(char_type Char)noexcept {
			return int_type('?');
		}
		static char_type MY_LIBRARY to_char_type(int_type Int)noexcept {
			return char_type('?');
		}
		static size_t MY_LIBRARY getRadix()noexcept {
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
		static int_type MY_LIBRARY to_int_type(char_type Char)noexcept {
			if constexpr (Char == Head)
			{
				return int_type(BeginIndex);
			}
			else
			{
				return BaseSet<char_type, int_type, BeginIndex + 1, Remained>::to_int_type(Char);
			}
		}
		static char_type MY_LIBRARY to_char_type(int_type Int)noexcept {
			if constexpr (Int == BeginIndex)
			{
				return char_type(Head);
			}
			else
			{
				return BaseSet<char_type, int_type, BeginIndex + 1, Remained>::to_char_type(Int);
			}
		}
		static size_t MY_LIBRARY getRadix()noexcept {
			return BaseSet<char_type, int_type, BeginIndex + 1, Remained...>::getRadix();
		}
	private:

	};

	typedef BaseSet < char, unsigned char, 0, '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'> Set16;
	typedef BaseSet < char, unsigned char, 0, '0', '1', '2', '3', '4', '5', '6', '7', '8'> Set8;
	typedef BaseSet < char, unsigned char, 0, '0', '1'> Set2;
	typedef BaseSet < char, unsigned char, 0, '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'> Set10;

	template<typename _Elem, typename charset>
	class OddStream
	{
	public:
		OddStream(std::basic_istream<_Elem> i)noexcept :i(i) {}

		~OddStream() = default;


		template<typename Iter>
		OddStream& MY_LIBRARY operator>>(Iter str)noexcept {
			std::basic_string<_Elem> temp;
			is >> temp;
			for (auto& i : temp)
			{
				str = charset::to_int_type(i);
				++str;
			}
			return *this;
		}

	private:
		std::basic_istream<_Elem> is;
	};
};