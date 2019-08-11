#pragma once

#include "Shared.h"
#include "Bytes.h"
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
			assert(init < Radix);
			static_assert(std::is_integral<Data>::value, "Integral required");
			static_assert(Data(-1) > Data(0), "Unsigned type required");
		}
		explicit constexpr MY_LIBRARY operator const Data&()const noexcept{
			return data;
		}
		explicit constexpr MY_LIBRARY operator Data&()noexcept{
			return data;
		}
		MY_LIBRARY ~Num()noexcept {}
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
		constexpr Num MY_LIBRARY operator/(const Num& that) noexcept {
			Num Copy(*this);
			return (Copy /= that);
		}
		constexpr Num MY_LIBRARY operator%(const Num& that) noexcept {
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
			if constexpr(Radix == Data(0))
			{
				Bytes<sizeof(Data) * 2> This = *this(), That = that();
				This *= that;
				this->data = Data(This);
				return *this;
			}
			else
			{
				if constexpr(Radix > std::numeric_limits<Data>::max() / Radix)
				{
					Bytes<sizeof(Data) * 2> This = Bytes<sizeof(Data) * 2>(this->data);
					This *= Bytes<sizeof(Data) * 2>(that.data);
					this->data = Data(This % Bytes<sizeof(Data) * 2>(Radix));
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
				if constexpr (Radix == Data(0))
				{
					Bytes<sizeof(Num) * 2> This(a());
					This *= Bytes<sizeof(Num) * 2>(b());
					This += Bytes<sizeof(Num) * 2>(Carry());
					return std::pair<Num, Num>(Num(Data(This)), Num(Data(This >> BitsPerByte * sizeof(Num))));
				}
				else
				{
					if constexpr (Radix > std::numeric_limits<Data>::max() / Radix)
					{
						Bytes<GetMinLength(Radix) * 2> This(a());
						This *= Bytes<GetMinLength(Radix) * 2>(b());
						This += Bytes<GetMinLength(Radix) * 2>(Carry());
						Bytes<GetMinLength(Radix) * 2> Res(0);
						Bytes<GetMinLength(Radix) * 2> radix(Radix);
						LongCmpt<BytesTraits<GetMinLength(Radix) * 2>>::DivideInto<Bytes<GetMinLength(Radix) * 2>, BytesIterator<GetMinLength(Radix) * 2>, value_type>(
							Res,
							BytesIterator<GetMinLength(Radix) * 2>(&radix, 0),
							BytesIterator<GetMinLength(Radix) * 2>(&This, 0)
							);
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