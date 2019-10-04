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
			MY_LIB AppositionIterator(Iterator1 a, Iterator2 b)noexcept
				:a(a), b(b),Result(c(Data(0), *(a), *(b))) {
				static_assert(std::is_same_v<Data, std::remove_cvref_t<decltype(*(b))>>, "It should be the same type");
			}
			MY_LIB ~AppositionIterator()noexcept {}
			//Notice:
			//	this function move the iterator to its next place
			void MY_LIB operator++() noexcept {
				++a;
				++b;

				Result = c(Result.second, *(a), *(b));
			}
			const Data& operator*()const noexcept {
				return Result.first;
			}
			//return true if the iterator is still working
			bool MY_LIB operator!()const noexcept {
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
			MY_LIB LineIterator(Data a, Iterator b)noexcept :a(a), b(b), c(), Result(c(Data(0), a, *b)) {
				static_assert(std::is_same<Data, std::remove_cvref_t<decltype(*b)>>::value, "It should be the same type");
			}
			MY_LIB ~LineIterator()noexcept {}
			//Notice:
			//	this function move the iterator b to its next place
			void MY_LIB operator++() noexcept {
				++b;
				Result = c(Result.second, a, *b);
			}
			constexpr void MY_LIB operator+=(size_t sz) noexcept {
				for (size_t i = 0; i < sz && b != nullptr; i++)
				{
					++b;
				}
				Result = c(Result.second, a, *b);
			}
			constexpr LineIterator MY_LIB operator+(size_t sz) const noexcept {
				LineIterator it(*this);
				it.b = it.b + sz;
				return it;
			}
			const Data& operator*()const noexcept {
				return Result.first;
			}
			//return true if the iterator is still working
			bool MY_LIB operator!()const noexcept {
				return (b == nullptr && Result.first() == 0 && Result.second() == 0);
			}
			constexpr bool MY_LIB operator==(const LineIterator& that)const noexcept {
				return (this->a == that.a) && (this->b == that.b);
			}
			constexpr bool MY_LIB operator!=(const LineIterator& that)const noexcept {
				return (this->a != that.a) || (this->b != that.b);
			}
			constexpr bool MY_LIB operator==(nullptr_t null)const noexcept {
				return (this->b == null) && (Result.second == 0);
			}
			constexpr bool MY_LIB operator!=(nullptr_t null)const noexcept {
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
			void MY_LIB operator++() noexcept {
				Result = c(Result.second);
			}
			void MY_LIB operator+=(size_t sz)noexcept {
				for (size_t i = 0; i < sz; i++)
				{
					++*this;
				}
			}
			LayerIterator MY_LIB operator+(size_t sz)const noexcept {
				LayerIterator it(*this);
				it += sz;
				return it;
			}
			//if this iterator doesn't work,
			//it equals nullptr
			bool MY_LIB operator==(std::nullptr_t)const noexcept {
				return !*this;
			}
			bool MY_LIB operator!()const noexcept {
				return (Result.second == Data(0) && Result.first == Data(0));
			}

		private:
			//Remained, Quotient
			std::pair<Data, Data> Result;
			ComputeFunction c;
		};


		template<typename Compute, typename Iterator1, typename Iterator2>
		static constexpr INLINED void MY_LIB AppositionComputeTo(Iterator1 a, Iterator2 b)noexcept {
			static_assert(std::is_same_v<
				typename std::remove_cvref_t<decltype(*a)>, 
				typename std::remove_cvref_t<decltype(*b)>
			>,
				"They should have the same type.");
			using Data=std::remove_cvref_t<decltype(*a)>;
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
		static constexpr INLINED void MY_LIB AddTo(Iterator1 a, Iterator2 b)noexcept {
			static_assert(std::is_same_v<
				typename std::remove_cvref_t<decltype(*a)>,
				typename std::remove_cvref_t<decltype(*b)>
			>, "They should have the same type.");
			return AppositionComputeTo<typename _Traits::Add, Iterator1, Iterator2>(a, b);
		}
		
		template<typename Iterator1, typename Iterator2>
		static constexpr INLINED void MY_LIB SubtractFrom(Iterator1 a, Iterator2 b)noexcept {
			return AppositionComputeTo<typename _Traits::SubtractFrom, Iterator1, Iterator2>(a, b);
		}

		template<typename Iterator, typename Data>
		static INLINED void MY_LIB MultiplyTo(Data a, Iterator b) noexcept {
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
		static INLINED Compare MY_LIB CompareTo(const Iterator1& a, const Iterator2& b) noexcept {
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
		static INLINED auto MY_LIB _CompareTo(const Iterator1& a, const Iterator2& b) noexcept {
			using Data=typename std::remove_cvref<decltype(*a)>::type;
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
								return std::pair<Data, Compare>(Data(*_a / (*_b)), Compare::Larger);
							}
							return std::pair<Data, Compare>(Data(*_a / (*_b + Data(1))), Compare::Larger);
						}
						return std::pair<Data, Compare>(1, Compare::Larger);
					}
					else if (temp == Compare::Smaller) {
						if (*_a < *_b) {
							if (!HasChanged && ((*_a) != 0))
							{
								return std::pair<Data, Compare>(Data(*_b / (*_a)), Compare::Smaller);
							}
							return std::pair<Data, Compare>(Data(*_b / (*_a + Data(1))), Compare::Smaller);
						}
						return std::pair<Data, Compare>((1), Compare::Smaller);
					}
					else {
						return std::pair<Data, Compare>((1), Compare::Equal);
					}
				}
			}
		}
		template<typename Accumulation, typename Recursion, typename Iterator1, typename Iterator2, typename Data>
		static INLINED void MY_LIB __DivideInto(Iterator1 _a, Iterator2 _b, Recursion Move, Accumulation Accum)noexcept {
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
		static INLINED void MY_LIB DivideInto(Linear& Res, Iterator1 a, Iterator2 b) noexcept {
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
		static INLINED void MY_LIB DivideInto(Iterator1 a, Iterator2 b) {
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
			MY_LIB Add()noexcept {}
			MY_LIB ~Add()noexcept {}
			std::pair<Data, Data> MY_LIB operator()(Data Carry, const Data& a, const Data& b)noexcept {
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
			MY_LIB SubtractFrom()noexcept {}
			MY_LIB ~SubtractFrom()noexcept {}
			std::pair<Data, Data> MY_LIB operator()(Data Carry, const Data& a, const Data& b)noexcept {
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
			MY_LIB Multiply()noexcept {}
			MY_LIB ~Multiply()noexcept {}
			std::pair<Data, Data> MY_LIB operator()(Data Carry, Data a, Data b)noexcept {
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
			using radix_t =decltype(radix);
			MY_LIB Divide()noexcept{}
			MY_LIB ~Divide()noexcept{}

			std::pair<radix_t, Data> operator()(const Data& that) noexcept{
				using resT=std::pair<radix_t, Data>;
				static_assert(std::is_same_v<Depack_t<Data>, Data>, "The type must not be num!");
				if constexpr(radix == 0) {
					if constexpr (sizeof(radix) >= sizeof(Data)) {
						return resT(that, 0);
					}
					else return resT(static_cast<radix_t>(that), that >> (LargeInteger::BitsPerByte * sizeof(radix)));
				}
				else return resT(static_cast<radix_t>(that % radix), that / radix);
			}
		};


	private:

	};
}