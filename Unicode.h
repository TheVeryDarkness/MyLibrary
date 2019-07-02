#pragma once

#include <limits>

class UniChar;

class UniChar
{
	using DataForm = long int;
public:
	__stdcall UniChar()noexcept {
	}

	__stdcall ~UniChar()noexcept {
	}

	bool __stdcall operator==(const UniChar& that)const noexcept {
		return (this->_Char == that._Char);
	}

	bool __stdcall operator!=(const UniChar& that)const noexcept {
		return (this->_Char != that._Char);
	}

	bool __stdcall operator<(const UniChar& that)const noexcept {
		return (this->_Char < that._Char);
	}

	template<typename OutType> explicit __stdcall UniChar(const OutType& c) noexcept {
		this->_Char = static_cast<DataForm>(
			(
			(
				c>std::numeric_limits<DataForm>::max()
				||
				c < std::numeric_limits<DataForm>::min()
				)
			?
			'?'
			:
				c
				)
			);
	}

	template<typename InType> explicit __stdcall operator InType()const noexcept {
		return static_cast<InType>(
			((
				this->_Char > std::numeric_limits<InType>::max()
				||
				this->_Char < std::numeric_limits<InType>::min()
				)
				?
				'?'
				:
				this->_Char
				));
	}

private:
	DataForm _Char = 0;
};

namespace std {
	template<>struct char_traits<UniChar>;
	template<> struct char_traits<UniChar>
	{
	public:
		using char_type =UniChar;
		using int_type =long long;
		using state_type =_Mbstatet;
		using pos_type =fpos<state_type>;
		using off_type =streamoff;
	public:
		char_traits()noexcept {
		}

		~char_traits()noexcept {
		}
		static bool eq_int_type(const int_type& a, const int_type& b) noexcept{ return a == b; }
		static int_type not_eof(const int_type& i) noexcept{ return ((i != -1) ? i : 0); }
		static constexpr int_type eof()noexcept { return static_cast<int_type>(-1); }

		static int_type __stdcall to_int_type(const char_type& c)noexcept {
			return static_cast<int_type>(c);
		}

		static char_type __stdcall to_char_type(const int_type& c) noexcept{
			return static_cast<char_type>(((c!=eof()&&true)?c:'?'));
		}

		static void assign(char_type& Arr, const char_type& c)noexcept {
			Arr = c;
		}

		static int compare(const char_type* Arr1, const char_type* Arr2, size_t sz) noexcept{
			for (size_t i = 0; i < sz; i++)
			{
				if (Arr1[i] != Arr2[i])
				{
					return (Arr1[i] < Arr2[i]);
				}
			}
			return 0;
		}

		static const char_type* find(const char_type* Arr, size_t Sz,const char_type& c) noexcept{
			for (size_t i = 0; i < Sz; i++)
			{
				if (Arr[i] == c)return &Arr[i];
			}
			return nullptr;
		}

		static void __stdcall copy(char_type* Dst, const char_type* Src, size_t Sz) noexcept{
			memcpy(Dst, Src, Sz);
		}
	};
}