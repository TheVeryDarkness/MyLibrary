#pragma once

#include "Shared.h"
#include "Statistics.h"
#ifdef _DEBUG
#include "Bytes.h"
#endif // _DEBUG
#include "_Bytes.h"
#include <cassert>
#include <limits>
#include <type_traits>

namespace LargeInteger {
	//0 radix stands for common radix.
	template<typename Data, Data Radix>
	class Num
	{
	public:
		constexpr static inline Data getRadix()noexcept {
			return Radix;
		}
		template<Data _Radix>
		constexpr MY_LIBRARY Num(const Num<Data, _Radix>& that) noexcept{
			if constexpr(Radix >= _Radix)
			{
				this->data = that();
			}
			else
			{
				this->data = that() % Radix;
				static_assert(Radix >= _Radix, "Target base should be bigger that origin.");
			}
		}
		constexpr const Data& MY_LIBRARY operator()()const noexcept{ return this->data; }
		constexpr MY_LIBRARY Num(Data init)noexcept
			:data((Radix == Data(0)) ? (init) : (init % Radix)) {
			ASSERT(init << " >= " << Radix, init < Radix);
			static_assert(std::is_integral<Data>::value, "Integral required");
			static_assert(Data(-1) > Data(0), "Unsigned type required");
		}
		explicit constexpr MY_LIBRARY operator const Data&()const noexcept{
			return data;
		}
		explicit constexpr MY_LIBRARY operator Data&()noexcept{
			return data;
		}
		MY_LIBRARY ~Num()noexcept {

		}
		constexpr Num MY_LIBRARY ComplementCodeAsMinus()const noexcept{
			if constexpr (Radix != Data(0)) {
				if (data != Data(0))
				{
					return Num(Radix - data);
				}
				else return Num(0);
			}
			else {
				return Num(~this->data + Data(1));
			}
		}
		constexpr Num MY_LIBRARY operator~() const noexcept {
			if constexpr (Radix == Data(0))
			{
				return Num(~data);
			}
			else return Num((Radix - (Data(1))) - data);
		}
		constexpr bool MY_LIBRARY OverFlowInAdding(const Num& that)const noexcept { return (this->data > (~that).data); }
		constexpr bool MY_LIBRARY UnderFlowInSubtracting(const Num& that)const noexcept { return (this->data < that.data); }
		constexpr bool MY_LIBRARY operator!()const noexcept {
			return (this->data == 0);
		}
		constexpr bool MY_LIBRARY operator==(const Num& that)const noexcept { return (this->data == that.data); }
		constexpr bool MY_LIBRARY operator!=(const Num& that)const noexcept { return (this->data != that.data); }
		constexpr bool MY_LIBRARY operator==(const Data& data)const noexcept { return (this->data == data); }
		constexpr Num& MY_LIBRARY operator=(const Num& that)noexcept { this->data = that.data; return*this; }
		constexpr Num& MY_LIBRARY operator+=(const Num& that) noexcept {
			if (OverFlowInAdding(that))
			{
				return (*this -= that.ComplementCodeAsMinus());
			}
			else {
				this->data += that.data;
				return *this;
			}
		}
		constexpr Num MY_LIBRARY operator+(const Num& that)const noexcept {
			Num Copy = *this;
			return (Copy += that);
		}
		constexpr Num& MY_LIBRARY operator-=(const Num& that) noexcept {
			if (this->UnderFlowInSubtracting(that))
			{
				return (*this += that.ComplementCodeAsMinus());
			}
			else {
				this->data -= that.data;
				return *this;
			}
		}
		constexpr Num MY_LIBRARY operator-(const Num& that)const noexcept {
			Num Copy = *this;
			return (Copy -= that);
		}
		constexpr Num& MY_LIBRARY operator/=(const Num& that)noexcept {
			this->data /= that.data;
			return *this;
		}
		constexpr Num& MY_LIBRARY operator%=(const Num& that) noexcept {
			this->data %= that.data;
			return *this;
		}
		constexpr Num MY_LIBRARY operator/(const Num& that) const noexcept {
			Num Copy(*this);
			return (Copy /= that);
		}
		constexpr Num MY_LIBRARY operator%(const Num& that) const noexcept {
			Num Copy(*this);
			return (Copy %= that);
		}
		//Be careful when using as it never checks flow
		constexpr Num& MY_LIBRARY operator--() { this->data--; return*this; }
		//Be careful when using as it never checks flow
		constexpr Num& MY_LIBRARY operator++() { this->data++; return*this; }
		constexpr bool MY_LIBRARY operator>(const Num& that)const noexcept { return (this->data > that.data); }
		constexpr bool MY_LIBRARY operator>=(const Num& that)const noexcept { return (this->data >= that.data); }
		constexpr bool MY_LIBRARY operator<(const Num& that)const noexcept { return (this->data < that.data); }
		constexpr bool MY_LIBRARY operator<=(const Num& that)const noexcept { return (this->data <= that.data); }
		constexpr bool MY_LIBRARY operator>(const Data& data)const noexcept { return (this->data > data); }
		constexpr bool MY_LIBRARY operator>=(const Data& data)const noexcept { return (this->data >= data); }
		constexpr bool MY_LIBRARY operator<(const Data& data)const noexcept { return (this->data < data); }
		constexpr bool MY_LIBRARY operator<=(const Data& data)const noexcept { return (this->data <= data); }
		constexpr Num& MY_LIBRARY operator*=(const Num& that) noexcept {
			assert(false);
			using LargeInteger::_Bytes;
			if constexpr(Radix == Data(0))
			{
				_Bytes<sizeof(Data) * 2> This = _Bytes<sizeof(Data) * 2>::Make_s(*this()), That = _Bytes<sizeof(Data) * 2>::Make_s(that());
				This *= that;
				this->data = Data(This);
				return *this;
			}
			else
			{
				if constexpr(Radix > std::numeric_limits<Data>::max() / Radix)
				{
					_Bytes<sizeof(Data) * 2> This = _Bytes<sizeof(Data) * 2>::Make_s(this->data);
					This *= _Bytes<sizeof(Data) * 2>::Make_s(that.data);
					this->data = Data(This % _Bytes<sizeof(Data) * 2>::Make_s(Radix));
					return *this;
				}
				else
				{
					this->data *= that.data;
					this->data %= Radix;
					return *this;
				}
			}
		}
		constexpr Num MY_LIBRARY operator*(const Num& that)const noexcept {
			Num Copy = *this;
			return (Copy *= that);
		}
		class Multiply
		{
		public:
			Multiply() = default;
			~Multiply() = default;

			std::pair<Num, Num> MY_LIBRARY operator()(Num Carry, Num a, Num b)const noexcept {
				using LargeInteger::_Bytes;
				if constexpr (Radix == Data(0))
				{
					_Bytes<sizeof(Data) * 2> This(a());
					This *= _Bytes<sizeof(Data) * 2>::Make_s(b());
					This += _Bytes<sizeof(Data) * 2>::Make_s(Carry());
					return std::pair<Num, Num>(Num(Data(This)), Num(Data(This >> LargeInteger::BitsPerByte * sizeof(Num))));
				}
				else
				{
					if constexpr (Radix > std::numeric_limits<Data>::max() / Radix)
					{
						_Bytes<GetMinLength(Radix) * 2> This = _Bytes<GetMinLength(Radix) * 2>::Make_s(a());
						This *= _Bytes<GetMinLength(Radix) * 2>::Make_s(b());
						This += _Bytes<GetMinLength(Radix) * 2>::Make_s(Carry());
						_Bytes<GetMinLength(Radix) * 2> radix = _Bytes<GetMinLength(Radix) * 2>::Make_s(Radix);
						_Bytes<GetMinLength(Radix) * 2> Res = This.Divide(radix);
						return std::pair<Num, Num>(
							Num(Data(This)),
							Num(Data(Res))
							);
					}
					else
					{
						a.data *= b.data;
						a.data += Carry.data;
						return std::pair<Num, Num>(Num(a() % Radix), Num(a() / Radix));
					}
				}
			}

		private:

		};

		constexpr static void MY_LIBRARY Swap(Num& a,Num&b)noexcept {
			a.data ^= b.data; b.data ^= a.data; a.data ^= b.data;
		}
		template<class os>
		friend os& operator<<(os& o, const Num& n) noexcept {
			if constexpr (sizeof(Data) <= 1)
			{
				return (o << (unsigned short)n.data);
			}
			else return (o << n.data);
		}
	private:
		Data data;
	};

}
template<typename Data, Data Radix>class std::numeric_limits<LargeInteger::Num<Data, Radix>> {
public:
	static constexpr Data max() noexcept { return (~LargeInteger::Num<Data, Radix>(0))(); }
	static constexpr Data min() noexcept { return (LargeInteger::Num<Data, Radix>(0))(); }
};
template<auto Radix>
class Depack<LargeInteger::Num<decltype(Radix), Radix>>
{
public:
	Depack() = delete;
	~Depack() = delete;
	using TRUE_TYPE=decltype(Radix);
private:

};