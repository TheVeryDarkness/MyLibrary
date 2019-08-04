#pragma once

#include <istream>

namespace LargeInteger {

	//Mind that '?' is reserved for unknown input
	template<typename _Elem, size_t BeginIndex, _Elem Head, _Elem... Remained>
	class OddTraits :OddTraits<_Elem, BeginIndex + 1, Remained>
	{
	public:
		MY_LIBRARY OddTraits() = default;
		MY_LIBRARY ~OddTraits() = default;
		using char_type=typename OddTraits<_Elem, BeginIndex + 1, Remained>::char_type;
		using int_type=typename OddTraits<_Elem, BeginIndex + 1, Remained>::int_type;
		static int_type MY_LIBRARY to_int_type(char_type Char)noexcept {
			if constexpr (Char == Head)
			{
				return int_type(BeginIndex);
			}
			else
			{
				return OddTraits<_Elem, BeginIndex + 1, Remained>::to_int_type(Char);
			}
		}
		static char_type MY_LIBRARY to_char_type(int_type Int)noexcept {
			if constexpr (Int == BeginIndex)
			{
				return char_type(Head);
			}
			else
			{
				return OddTraits<_Elem, BeginIndex + 1, Remained>::to_char_type(Int);
			}
		}
	private:

	};


	template<typename _Elem,size_t BeginIndex, _Elem Head>
	class OddTraits<_Elem, BeginIndex, Head> :std::char_traits<_Elem>
	{
	public:
		using char_type=typename std::char_traits<_Elem>::char_type;
		using int_type=typename std::char_traits<_Elem>::int_type;
		MY_LIBRARY OddTraits() = default;
		MY_LIBRARY ~OddTraits() = default;

		static int_type MY_LIBRARY to_int_type(_Elem Char)noexcept {
			if constexpr (Char == Head)
			{
				return BeginIndex;
			}
			else
			{
				return int_type('?');
			}
		}
		static char_type MY_LIBRARY to_char_type(int_type Int)noexcept {
			if constexpr (Int == BeginIndex)
			{
				return char_type(Head);
			}
			else
			{
				return char_type('?');
			}
		}
	private:

	};
	
	template<typename _Elem>
	class OddStream
	{
	public:
		OddStream(std::basic_istream<_Elem> i)noexcept:i(i){}

		~OddStream() = default;

		template<typename target>
		OddStream& MY_LIBRARY operator>>()noexcept {

		}


	private:
		std::basic_istream<_Elem> i;
	};

}