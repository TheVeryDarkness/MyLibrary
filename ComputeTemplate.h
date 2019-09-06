#pragma once

#include "Shared.h"
#include "_Bytes.h"
#include <type_traits>

namespace LargeInteger{
	enum class Compare :signed char
	{
		Larger = 0x1, Equal = 0x0, Smaller = -0x1
	};



	//The _Traits must give these functions:
	//	std::pair<Data, Data> Add(const Data& Carry, const Data& a, const Data& b);
	//	std::pair<Data, Data> SubtractFrom(Data Carry, const Data& a, const Data& b);
	//	std::pair<Data, Data> Multiply(Data Carry, Data a, Data b);
	//	std::pair<Data, Data> Divide<radix>(const Data& that);
	//	void insert(Iterator, Data);//However, it doen't need to insert an element after it
	//When an element doesn't a next element, GetNext(Iterator) should return a Iterator that equals to nullptr.
	//
	//The _Traits must give these definition:
	//	NullIterator;
	//NullIterator must have 0 data, and not have an next element.
	template<typename _Traits>
	class LongCmpt {
	public:
		template<class ComputeFunction, typename Iterator1, typename Iterator2, typename Data>
		class AppositionIterator
		{
		public:
			MY_LIBRARY AppositionIterator(Iterator1 a, Iterator2 b)noexcept
				:a(a), b(b),Result(c(Data(0), *(a), *(b))) {
				static_assert(std::is_same_v<Data, std::remove_cvref_t<decltype(*(b))>>, "It should be the same type");
			}
			MY_LIBRARY ~AppositionIterator()noexcept {}
			//Notice:
			//	this function move the iterator to its next place
			void MY_LIBRARY operator++() noexcept {
				++a;
				++b;

				Result = c(Result.second, *(a), *(b));
			}
			const Data& operator*()const noexcept {
				return Result.first;
			}
			//return true if the iterator is still working
			bool MY_LIBRARY operator!()const noexcept {
				return (
					a == nullptr
					&&
					b == nullptr
					&&
					Result.first == 0
					&&
					Result.second == 0
					);
			}
			//result;overflow
			std::pair<Data, Data> Result;
			Iterator1 a;
			Iterator2 b;
		private:
			ComputeFunction c = ComputeFunction();
		};

		template<class ComputeFunction, typename Iterator, typename Data>
		class LineIterator
		{
		public:
			MY_LIBRARY LineIterator(Data a, Iterator b)noexcept :a(a), b(b), c(), Result(c(Data(0), a, *b)) {
				static_assert(std::is_same<Data, std::remove_cvref_t<decltype(*b)>>::value, "It should be the same type");
			}
			MY_LIBRARY ~LineIterator()noexcept {}
			//Notice:
			//	this function move the iterator b to its next place
			void MY_LIBRARY operator++() noexcept {
				++b;
				Result = c(Result.second, a, *b);
			}
			constexpr void MY_LIBRARY operator+=(size_t sz) noexcept {
				for (size_t i = 0; i < sz && b != nullptr; i++)
				{
					++b;
				}
				Result = c(Result.second, a, *b);
			}
			constexpr LineIterator MY_LIBRARY operator+(size_t sz) const noexcept {
				LineIterator it(*this);
				it.b = it.b + sz;
				return it;
			}
			const Data& operator*()const noexcept {
				return Result.first;
			}
			//return true if the iterator is still working
			bool MY_LIBRARY operator!()const noexcept {
				return (b == nullptr && Result.first() == 0 && Result.second() == 0);
			}
			constexpr bool MY_LIBRARY operator==(const LineIterator& that)const noexcept {
				return (this->a == that.a) && (this->b == that.b);
			}
			constexpr bool MY_LIBRARY operator!=(const LineIterator& that)const noexcept {
				return (this->a != that.a) || (this->b != that.b);
			}
			constexpr bool MY_LIBRARY operator==(nullptr_t null)const noexcept {
				return (this->b == null) && (Result.second == 0);
			}
			constexpr bool MY_LIBRARY operator!=(nullptr_t null)const noexcept {
				return !(*this == null);
			}
			//result;overflow
			std::pair<Data, Data> Result;
			Data a;
			Iterator b;
			ComputeFunction c;
		};


		template<class ComputeFunction, typename Data>
		class LayerIterator
		{
		public:
			template<typename Val>
			LayerIterator(Val val) noexcept :c(), Result(c(val)) { }
			~LayerIterator()noexcept {}
			const Data& operator*()const noexcept {
				return Result.first;
			}
			void MY_LIBRARY operator++() noexcept {
				Result = c(Result.second);
			}
			void MY_LIBRARY operator+=(size_t sz)noexcept {
				for (size_t i = 0; i < sz; i++)
				{
					++*this;
				}
			}
			LayerIterator MY_LIBRARY operator+(size_t sz)const noexcept {
				LayerIterator it(*this);
				it += sz;
				return it;
			}
			bool MY_LIBRARY operator==(std::nullptr_t nul)const noexcept {
				return !*this;
			}
			bool MY_LIBRARY operator!()const noexcept {
				return (Result.second == Data(0) && Result.first == Data(0));
			}

		private:
			//Remained, Quotient
			std::pair<Data, Data> Result;
			ComputeFunction c;
		};


		template<typename Compute, typename Iterator1, typename Iterator2>
		static constexpr INLINED void MY_LIBRARY AppositionComputeTo(Iterator1 a, Iterator2 b)noexcept {
			static_assert(std::is_same_v<
				typename std::remove_cvref_t<decltype(*a)>, 
				typename std::remove_cvref_t<decltype(*b)>
			>,
				"They should have the same type.");
			using Data=std::remove_cvref_t<decltype(*a)>;
			Data Carry = Data(0);
			AppositionIterator<Compute, Iterator1, Iterator2, Data> compute(a, b);
			while (true)
			{
				//This element
				*(compute.b) = compute.Result.first;
				if (
					(compute.b + 1) == nullptr
					)
				{
					if ((compute.a + 1) == nullptr)
					{
						if (compute.Result.second != 0)
						{
							compute.b.insert(compute.b, compute.Result.second);
						}
						break;
					}
					else
					{
						compute.b.insert(compute.b, Data(0));
					}
				}
				++compute;
				if (!compute)
				{
					break;
				}
			}
		}

		template<typename Iterator1, typename Iterator2>
		static constexpr INLINED void MY_LIBRARY AddTo(Iterator1 a, Iterator2 b)noexcept {
			static_assert(std::is_same_v<
				typename std::remove_cvref_t<decltype(*a)>,
				typename std::remove_cvref_t<decltype(*b)>
			>);
			return AppositionComputeTo<typename _Traits::Add, Iterator1, Iterator2>(a, b);
		}
		
		template<typename Iterator1, typename Iterator2>
		static constexpr INLINED void MY_LIBRARY SubtractFrom(Iterator1 a, Iterator2 b)noexcept {
			return AppositionComputeTo<typename _Traits::SubtractFrom, Iterator1, Iterator2>(a, b);
		}

		template<typename Iterator, typename Data>
		static INLINED void MY_LIBRARY MultiplyTo(Data a, Iterator b) noexcept {
			Data Carry = Data(0);
			LineIterator<typename _Traits::Multiply, Iterator, Data> mul(a, b);
			while (true)
			{
				//This element
				*(mul.b) = mul.Result.first;
				if (mul.b + 1 == nullptr)
				{
					if (mul.Result.second != Data(0))
					{
						mul.b.insert(mul.b, mul.Result.second);
					}
					break;
				}
				++mul;
				if (!mul)
				{
					break;
				}
			}
		}


		//Compare a to b.
		template<typename Iterator1, typename Iterator2>
		static INLINED Compare MY_LIBRARY CompareTo(const Iterator1& a, const Iterator2& b) noexcept {
			static_assert(
				std::is_same_v<
				std::remove_cvref_t<decltype(*a)>, 
				std::remove_cvref_t<decltype(*b)>
				>,
				"They should have the same type");
			{
				Compare temp = Compare::Equal;
				Iterator1 _a = a;
				Iterator2 _b = b;
				for (;
					;
					++_a, ++_b
					) {
					if (*_a > *_b)
					{
						temp = Compare::Larger;
					}
					else if (*_a < *_b)
					{
						temp = Compare::Smaller;
					}
					if ((_a + 1 == nullptr) && (_b + 1 == nullptr))
					{
						break;
					}
				}
				{
					return temp;
				}
			}
		}
		//Extension for Compare()
		template<typename Iterator1, typename Iterator2>
		static INLINED auto MY_LIBRARY _CompareTo(const Iterator1& a, const Iterator2& b) noexcept {
			using Data=typename std::remove_cvref<decltype(*a)>::type;
			using TRUE_TYPE=typename Depack<Data>::TRUE_TYPE;
			static_assert(std::is_same_v<std::remove_cvref_t<decltype(*a)>, std::remove_cvref_t<decltype(*b)>>);
			{
				bool HasChanged = false;
				Compare temp = Compare::Equal;
				Iterator1 _a = a;
				Iterator2 _b = b;
				for (;
					;
					++_a, ++_b, HasChanged = true
					) {
					if (*_a > *_b)
					{
						temp = Compare::Larger;
					}
					else if (*_a < *_b)
					{
						temp = Compare::Smaller;
					}
					if ((_a + 1 == nullptr) && (_b + 1 == nullptr))
					{
						break;
					}
				}
				{
					if (temp == Compare::Larger) {
						if (*_a > *_b) {
							if (!HasChanged && ((*_a) != 0))
							{
								return std::pair<TRUE_TYPE, Compare>(TRUE_TYPE(*_a / (*_b)), Compare::Larger);
							}
							return std::pair<TRUE_TYPE, Compare>(TRUE_TYPE(*_a / (*_b + Data(1))), Compare::Larger);
						}
						return std::pair<TRUE_TYPE, Compare>(1, Compare::Larger);
					}
					else if (temp == Compare::Smaller) {
						if (*_a < (*_b)) {
							if (!HasChanged && ((*_a) != 0))
							{
								return std::pair<TRUE_TYPE, Compare>(TRUE_TYPE(*_b / (*_a)), Compare::Smaller);
							}
							return std::pair<TRUE_TYPE, Compare>(TRUE_TYPE(*_b / (*_a + Data(1))), Compare::Smaller);
						}
						return std::pair<TRUE_TYPE, Compare>((1), Compare::Smaller);
					}
					else {
						return std::pair<TRUE_TYPE, Compare>((1), Compare::Equal);
					}
				}
			}
		}
		template<typename Accumulation, typename Recursion, typename Iterator1, typename Iterator2, typename Data>
		static INLINED void MY_LIBRARY __DivideInto(Iterator1 _a, Iterator2 _b, Recursion Move, Accumulation Accum)noexcept {
			{
				switch (CompareTo(_a, _b))
				{
				case Compare::Larger:
					return;
				case Compare::Equal:
					Accum(_a, _b, 1);
					return;
				case Compare::Smaller:
					__DivideInto<Accumulation, Recursion, Iterator1, Iterator2, Data>(_a, _b + 1, Move, Accum);
					Move();
					break;
				default:
					break;
				}
			}
			while (true)
			{
				auto [res, cmpr] = _CompareTo(_b, _a);
				if (cmpr == Compare::Smaller)
				{
					return;
				}
				else {
					if (cmpr == Compare::Equal)
					{
						res = 1;
					}
					Accum(_a, _b, res);
					//return;
				}
			}
		}
		template<typename Simplify, typename Linear, typename Iterator1, typename Iterator2>
		static INLINED void MY_LIBRARY DivideInto(Linear& Res, Iterator1 a, Iterator2 b) noexcept {
			static_assert(std::is_same< std::remove_cvref<decltype(*a)>::type, std::remove_cvref<decltype(*b)>::type>::value);
			using Data=typename std::remove_cvref<decltype(*a)>::type;
			//Regarding of the compatibility, we didn't use any majorization.
			auto func1 = [&Res](const Iterator1& a, const Iterator2& b, Data times)->void {
				LineIterator<_Traits::Multiply, Iterator1, decltype(times)> temp(times, a);
				SubtractFrom(temp, b);
				Simplify s(b);
				Res += times;
			};
			auto func2 = [&Res]()->void {Res <<= 1; };
			__DivideInto<decltype(func1), decltype(func2), Iterator1, Iterator2, Data>(a, b, func2, func1);
		}
		template<typename Simplify, typename Iterator1, typename Iterator2>
		static INLINED void MY_LIBRARY DivideInto(Iterator1 a, Iterator2 b) {
			static_assert(std::is_same< std::remove_cvref<decltype(*a)>::type, std::remove_cvref<decltype(*b)>::type>::value);
			using Data=typename std::remove_cvref<decltype(*a)>::type;
			//Regarding of the compatibility, we didn't use any majorization.
			auto func = [](const Iterator1& a, const Iterator2& b, Data times)->void {
				LineIterator<_Traits::Multiply, Iterator1, decltype(times)> temp(times, a);
				SubtractFrom(temp, b);
				Simplify s(b);
			};
			auto null = []()->void {};
			__DivideInto<decltype(func), decltype(null), Iterator1, Iterator2, Data>(a, b, null, func);
		}
	};

	template<typename Data>
	class StdCmptTraits
	{
	public:
		StdCmptTraits() = delete;
		~StdCmptTraits() = delete;
		class Add
		{
		public:
			MY_LIBRARY Add()noexcept {}
			MY_LIBRARY ~Add()noexcept {}
			std::pair<Data, Data> MY_LIBRARY operator()(Data Carry, const Data& a, const Data& b)noexcept {
				return std::pair<Data, Data>(
					a + b + Carry,
					Data(
					(Carry > 0) ?
						(((a > Data(~Data(b + 1))) || (b > Data(~Data(1)))) ? 1 : 0)
						:
						((a > Data(~b)) ? 1 : 0)
					));
			}
		};

		class SubtractFrom
		{
		public:
			MY_LIBRARY SubtractFrom()noexcept {}
			MY_LIBRARY ~SubtractFrom()noexcept {}
			std::pair<Data, Data> MY_LIBRARY operator()(Data Carry, const Data& a, const Data& b)noexcept {
				return std::pair<Data, Data>(
					b - a - Carry,
					Data(
					(Carry > 0) ?
						((b <= a) ? 1 : 0)
						:
						((b < a) ? 1 : 0))
					);
			}
		};

		class Multiply
		{
		public:
			MY_LIBRARY Multiply()noexcept {}
			MY_LIBRARY ~Multiply()noexcept {}
			std::pair<Data, Data> MY_LIBRARY operator()(Data Carry, Data a, Data b)noexcept {
				if (a > b)
				{
					Data temp = b;
					b = a;
					a = temp;
				}
				assert(a <= b);
				if (a == Data(0))
				{
					return std::pair<Data, Data>(Carry, Data(0));
				}
				Data Res = b;
				if (Res > Data(~Carry)) { Carry = Data(1); }
				else Carry = Data(0);
				for (Data i = Data(1); i < a; ++i)
				{
					if (Res > Data(~b))
					{
						Carry += Data(1);
					}
					Res += b;
				}
				return std::pair<Data, Data>(Res, Carry);
			}
		};

		template<auto radix>
		class Divide
		{
		public:
			MY_LIBRARY Divide()noexcept{}
			MY_LIBRARY ~Divide()noexcept{}

			std::pair<Data, Data> operator()(const Data& that) noexcept{
				static_assert(std::is_same_v<Depack<Data>::TRUE_TYPE, Data>, "The type must not be num!");
				return std::pair<Data, Data>(that % radix, that / radix);
			}
		};


	private:

	};

	template<auto Radix>
	class Multiply {
	public:
		using Num=decltype(Radix);
		using Data=Num;
		Multiply() = default;
		~Multiply() = default;

		std::pair<Num, Num> MY_LIBRARY operator()(Num Carry, Num a, Num b)const noexcept {
			using LargeInteger::_Bytes;
			if constexpr (Radix == Data(0)) {
				_Bytes<sizeof(Data) * 2> This(a);
				This *= _Bytes<sizeof(Data) * 2>::Make_s(b);
				This += _Bytes<sizeof(Data) * 2>::Make_s(Carry);
				return std::pair<Num, Num>(Num(Data(This)), Num(Data(This >> LargeInteger::BitsPerByte * sizeof(Num))));
			}
			else {
				if constexpr (Radix > std::numeric_limits<Data>::max() / Radix) {
					_Bytes<GetMinLength(Radix) * 2> This = _Bytes<GetMinLength(Radix) * 2>::Make_s(a);
					This *= _Bytes<GetMinLength(Radix) * 2>::Make_s(b);
					This += _Bytes<GetMinLength(Radix) * 2>::Make_s(Carry);
					_Bytes<GetMinLength(Radix) * 2> radix = _Bytes<GetMinLength(Radix) * 2>::Make_s(Radix);
					_Bytes<GetMinLength(Radix) * 2> Res = This.Divide(radix);
					return std::pair<Num, Num>(
						Num(Data(This)),
						Num(Data(Res))
						);
				}
				else {
					a.data *= b.data;
					a.data += Carry.data;
					return std::pair<Num, Num>(Num(a % Radix), Num(a / Radix));
				}
			}
		}

	private:

	};
}