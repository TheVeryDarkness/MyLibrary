#pragma once

#include "Shared.h"

namespace LargeInteger{
	enum class Compare :short
	{
		Larger = 0x1, Equal = 0x0, Smaller = -0x1
	};



	//The _Traits must give these functions:
	//	Iterator GetNext(Iterator);
	//	Data& GetData(Iterator);
	//	void Add(Data&, bool, Data, Data);
	//	void Subtract(Data&, bool, Data, Data);
	//	void assign(Linear*, size_t);
	//	void InsertAfter(Iterator*, Data);//However, it doen't need to insert an element after it
	//When an element doesn't a next element, GetNext(Iterator) should return NullIterator.
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
				:a(a), b(b), c(), Result(c(Data(0), _Traits::GetData(a), _Traits::GetData(b))) {
				static_assert(std::is_same<Data, std::remove_reference<decltype(_Traits::GetData(b))>::type>::value, "It should be the same type");
			}
			MY_LIBRARY ~AppositionIterator()noexcept {}
			//Notice:
			//	this function move the iterator to its next place
			void MY_LIBRARY operator++() noexcept {
				a = _Traits::GetNext(a);
				b = _Traits::GetNext(b);

				Result = c(Result.second, _Traits::GetData(a), _Traits::GetData(b));
			}
			const Data& operator*()const noexcept {
				return Result.first;
			}
			//return true if the iterator is still working
			MY_LIBRARY operator bool()const noexcept {
				return !(
					a == _Traits::NullIterator
					&&
					b == _Traits::NullIterator
					&&
					Result.first == Data(0)
					&&
					Result.second == Data(0)
					);
			}
			//result;overflow
			std::pair<Data, Data> Result;
			Iterator1 a;
			Iterator2 b;
		private:
			ComputeFunction c;
		};

		template<class ComputeFunction, typename Iterator, typename Data>
		class LineIterator
		{
		public:
			MY_LIBRARY LineIterator(Data a, Iterator b)noexcept :a(a), b(b), c(), Result(c(Data(0), a, *b)) {
				static_assert(std::is_same<Data, std::remove_reference<decltype(*b)>::type>::value, "It should be the same type");
			}
			MY_LIBRARY ~LineIterator()noexcept {}
			//Notice:
			//	this function move the iterator b to its next place
			void MY_LIBRARY operator++() noexcept {
				b = _Traits::GetNext(b);

				Result = c(Result.second, a, _Traits::GetData(b));
			}
			const Data& operator*()const noexcept {
				return Result.first;
			}
			//return true if the iterator is still working
			MY_LIBRARY operator bool()const noexcept {
				return !(b == nullptr && Result.first == Data(0) && Result.second == Data(0));
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
			LayerIterator(Val val) noexcept :c(), Result(c(val)) {}
			~LayerIterator()noexcept {}
			const Data& operator*()const noexcept {
				return Result.first;
			}
			void MY_LIBRARY operator++() noexcept {
				Result = c(Result);
			}

		private:
			//Remained, Quotient
			std::pair<Data, Data> Result;
			ComputeFunction c;
		};


		template<typename Compute, typename Iterator1, typename Iterator2, typename Data>
		static constexpr INLINED void MY_LIBRARY AppositionComputeTo(Iterator1 a, Iterator2 b)noexcept {
			Data Carry = Data(0);
			AppositionIterator<Compute, Iterator1, Iterator2, Data> compute(a, b);
			while (true)
			{
				//This element
				_Traits::GetData(compute.b) = compute.Result.first;
				if (
					_Traits::GetNext(compute.b) == _Traits::NullIterator
					)
				{
					if (_Traits::GetNext(compute.a) == _Traits::NullIterator)
					{
						if (compute.Result.second != Data(0))
						{
							_Traits::InsertAfter(compute.b, compute.Result.second);
						}
						break;
					}
					else
					{
						_Traits::InsertAfter(compute.b);
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
			static_assert(std::is_same<typename std::remove_cvref<decltype(*a)>::type, typename std::remove_cvref<decltype(*b)>::type>::value);
			return AppositionComputeTo<typename _Traits::Add, Iterator1, Iterator2, decltype(*a)>(a, b);
		}
		
		template<typename Iterator1, typename Iterator2>
		static constexpr INLINED void MY_LIBRARY SubtractFrom(Iterator1 a, Iterator2 b)noexcept {
			return AppositionComputeTo<typename _Traits::SubtractFrom, Iterator1, Iterator2, decltype(*a)>(a, b);
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
			static_assert(std::is_same<std::remove_cvref<decltype(*a)>::type, std::remove_cvref<decltype(*b)>::type>::value);
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
		static INLINED std::pair<typename std::remove_cvref<decltype(*a)>::type, Compare> MY_LIBRARY _CompareTo(const Iterator1& a, const Iterator2& b) noexcept {
			using Data=typename std::remove_cvref<decltype(*a)>::type;
			static_assert(std::is_same<std::remove_cvref<decltype(*a)>::type, std::remove_cvref<decltype(*b)>::type>);
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
					if ((_a + 1 == _Traits::NullIterator) && (_b + 1 == _Traits::NullIterator))
					{
						break;
					}
				}
				{
					if (temp == Compare::Larger) {
						if (*_a > *_b) {
							return std::pair<Data, Compare>(Data(*_a / (*_b + Data(1))), Compare::Larger);
						}
						return std::pair<Data, Compare>(Data(1), Compare::Larger);
					}
					else if (temp == Compare::Smaller) {
						if (_Traits::GetData(_a) < (_Traits::GetData(_b))) {
							return std::pair<Data, Compare>(Data(_Traits::GetData(_b) / (_Traits::GetData(_a) + Data(1))), Compare::Smaller);
						}
						return std::pair<Data, Compare>(Data(1), Compare::Smaller);
					}
					else {
						return std::pair<Data, Compare>(Data(1), Compare::Equal);
					}
				}
			}
		}
		template<typename Accumulation, typename Recursion, typename Iterator1, typename Iterator2, typename Data>
		static INLINED void MY_LIBRARY __DivideInto(Iterator1 _a, Iterator2 _b, Recursion Move, Accumulation Accum)noexcept {
			{
				switch (CompareTo<Iterator1, Iterator2, Data>(_a, _b))
				{
				case Compare::Larger:
					return;
				case Compare::Equal:
					Accum(_a, _b, Data(1));
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
				auto [res, cmpr] = _CompareTo<Iterator, Data>(_b, _a);
				if (cmpr == Compare::Smaller)
				{
					return;
				}
				else {
					if (cmpr == Compare::Equal)
					{
						res = Data(1);
					}
					Accum(_a, _b, res);
					//return;
				}
			}
		}
		template<typename Linear, typename Iterator1, typename Iterator2>
		static INLINED void MY_LIBRARY DivideInto(Linear& Res, Iterator1 a, Iterator2 b) noexcept {
			static_assert(std::is_same< std::remove_cvref<decltype(*a)>::type, std::remove_cvref<decltype(*b)>::type>::value);
			using Data=std::remove_cvref<decltype(*a)>::type;
			//Regarding of the compatibility, we didn't use any majorization.
			auto func1 = [&Res](const Iterator1& a, const Iterator2& b, Data times)->void {
				for (Data i = Data(0); i < times; ++i) {
					AppositionComputeTo<typename _Traits::SubtractFrom, Iterator1, Iterator2, Data>(a, b);
				}
				Res += times;
			};
			auto func2 = [&Res]()->void {Res <<= 1; };
			__DivideInto<decltype(func1), decltype(func2), Iterator1, Iterator2, Data>(a, b, func2, func1);
		}
		template<typename Iterator1, typename Iterator2>
		static INLINED void MY_LIBRARY DivideInto(Iterator1 a, Iterator2 b) {
			static_assert(std::is_same< std::remove_cvref<decltype(*a)>::type, std::remove_cvref<decltype(*b)>::type>::value);
			using Data=std::remove_cvref<decltype(*a)>::type;
			//Regarding of the compatibility, we didn't use any majorization.
			auto func = [](const Iterator1& a, const Iterator2& b, Data times)->void {
				for (Data i = Data(0); i < times; ++i) {
					AppositionComputeTo<typename _Traits::SubtractFrom, Iterator1, Iterator2, Data>(a, b);
				}
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

		class Divide
		{
		public:
			MY_LIBRARY Divide()noexcept{}
			MY_LIBRARY ~Divide()noexcept{}

		private:

		};


	private:

	};
}