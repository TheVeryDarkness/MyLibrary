#pragma once

#include "Shared.h"
#include "end_ptr.h"
#include <cassert>
#include <ostream>
#include <type_traits>

//Hey guy,
//	if you were here to edit my source code below,
//	notice that I now follow a non-standard standard.
//	It's set up by myself,
//	after a lot of struggle.
//
//	1.An iterator equals nullptr when it is going to be out of its working range,
//	though it may not result in crash.
//	2.end_ptr is used for suffiency.
//	3.Always remember that each iterator below represents a kind of linear list,
//	difference among them is just how they are induced.



namespace Darkness {
	inline namespace LargeInteger {
		template<typename Iter1, typename Iter2>
		struct iter_range {
		public:
			constexpr iter_range(Iter1 begin, Iter2 end)noexcept :_begin(begin), _end(end) { }
			constexpr Iter1 &begin()noexcept { return this->_begin; }
			constexpr Iter2 &end()noexcept { return this->_end; }
			constexpr const Iter1 &begin()const noexcept { return this->_begin; }
			constexpr const Iter2 &end()const noexcept { return this->_end; }
			auto operator*()noexcept {
				return *_begin;
			}
		private:
			Iter1 _begin;
			Iter2 _end;
		};
		template<typename Iter1>
		struct iter_range<Iter1, nullptr_t> {
		public:
			constexpr iter_range(Iter1 begin, nullptr_t)noexcept :_begin(begin) { }
			constexpr Iter1 &begin()noexcept { return this->_begin; }
			constexpr nullptr_t end()noexcept { return nullptr; }
			constexpr const Iter1 &begin()const noexcept { return this->_begin; }
			constexpr nullptr_t end()const noexcept { return nullptr; }
			auto operator*()noexcept {
				return *_begin;
			}
		private:
			Iter1 _begin;
		};

		enum class Compare :signed char {
			Larger = 0x1, Equal = 0x0, Smaller = -0x1
		};
		inline std::ostream &operator<<(std::ostream &o, Compare c)noexcept {
			switch (c) {
			case LargeInteger::Compare::Larger:
				o << "Larger";
				break;
			case LargeInteger::Compare::Equal:
				o << "Equal";
				break;
			case LargeInteger::Compare::Smaller:
				o << "Smaller";
				break;
			default:
				o << '?';
				break;
			}
			return o;
		}


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
			template<class ComputeFunction, typename subRange, typename Range, typename Data, typename PreLen>
			class [[deprecated]] SubPrincIterator {
			private:
				ComputeFunction c;
				//result;overflow
				std::pair<Data, Data> Result;
				subRange a;
			public:
				Range b;
				MY_LIB SubPrincIterator(subRange a, Range b)noexcept
					:c(), a(a), b(b), Result(c(Data(0), *(a.begin()), *(b.begin()))) {
					static_assert(std::is_same_v<Data, std::decay_t<decltype(*a.begin())>>, "It should be the same type");
					static_assert(std::is_same_v<Data, std::decay_t<decltype(*b.begin())>>, "It should be the same type");
				}
				MY_LIB ~SubPrincIterator()noexcept { }
				//Notice:
				//	this function move the iterator to its next place
				void MY_LIB operator++() noexcept {
					auto tmp(a.begin());
					if (++tmp == a.end()) {
						a.begin() = emplace_back(a.begin());
					}
					else a.begin() = tmp;
					++b.begin();
					Result = c(Result.second, *(a), *(b));
				}
				const Data &operator*()const noexcept {
					return Result.first;
				}
				bool MY_LIB operator==(end_ptr_t)const noexcept {
					return (Result.second == 0) && (a.begin() == a.end());
				}
				bool MY_LIB operator!=(end_ptr_t)const noexcept {
					return !(*this == end_ptr);
				}
				bool MY_LIB operator==(nullptr_t)const noexcept {
					return (Result.first == 0) && (Result.second == 0) && a.begin() == a.end();
				}
				bool MY_LIB operator!=(nullptr_t)const noexcept {
					return !(*this == nullptr);
				}
			};

			template<class ComputeFunction, typename Range, typename Data>
			class LineIterator {
			private:
				ComputeFunction c;
				//result;overflow
				std::pair<Data, Data> Result;
				Data a;
				Range b;
			public:
				static_assert(std::is_same_v<Data, std::decay_t<decltype(*b.begin())>>, "It should be the same type");
				MY_LIB LineIterator(Data a, Range b)noexcept :a(a), b(b), c(), Result(c(Data(0), a, *b.begin())) { }
				MY_LIB LineIterator(Data a, Range b, Data Carry)noexcept :a(a), b(b), c(), Result(c(Carry, a, *b)) { }
				MY_LIB ~LineIterator()noexcept { }
				//Notice:
				//	this function move the iterator b to its next place
				void MY_LIB operator++() noexcept {
					++b.begin();
					Result = c(Result.second, a, *b);
				}
				constexpr LineIterator MY_LIB operator+(size_t sz) const noexcept {
					LineIterator it(this->a, this->b + sz, Result.second);
					return it;
				}
				const Data &operator*()const noexcept {
					return Result.first;
				}
				constexpr bool MY_LIB operator==(end_ptr_t)const noexcept {
					return a == 0 || ((Result.second == 0) && (this->b == nullptr || this->b + 1 == nullptr));
				}
				constexpr bool MY_LIB operator==(nullptr_t)const noexcept {
					return a == 0 || ((this->b.begin() == this->b.end()) && (Result.first == 0) && (Result.second == 0));
				}
			};


			template<class ComputeFunction, typename Data, typename Value>
			class LayerIterator {
			public:
				LayerIterator(Value val) noexcept :c(), Result(c(val)) { }
				~LayerIterator()noexcept { }
				const Data &operator*()const noexcept {
					return Result.first;
				}
				void MY_LIB operator++() noexcept {
					Result = c(Result.second);
				}
				//if this iterator doesn't work,
				//it equals nullptr
				bool MY_LIB operator==(std::nullptr_t)const noexcept {
					return (Result.second == Data(0) && Result.first == Data(0));
				}
				bool MY_LIB operator!=(std::nullptr_t)const noexcept { return !(*this == nullptr); }

			private:
				ComputeFunction c;
				//Remained, Quotient
				std::pair<Data, Value> Result;
			};


			class Empty {
			public:
				Empty() = default;
				~Empty() = default;
				void MY_LIB operator()()const noexcept { }
			private:

			};


			template<typename Compute, typename subRange, typename Range, typename PreLen, typename... CallBack>
			static constexpr INLINED void MY_LIB SubPrinComputeTo(subRange &a, Range &b, PreLen pre, CallBack &... c)noexcept {
				static_assert(std::is_same_v<
					std::decay_t<decltype(*a.begin())>,
					std::decay_t<decltype(*b.begin())>>,
					"They should have the same type.");
				using Data = std::decay_t<decltype(*a.begin())>;
				//This element


				for (
					SubPrincIterator<Compute, subRange, Range, Data, PreLen> compute(a, b);
					(c(), ...), *(compute.b.begin()) = *compute, compute != end_ptr;
					++compute);
			}

			template<typename subRange, typename Range, typename PreLen, typename... CallBack>
			static constexpr INLINED void MY_LIB AddTo(subRange a, Range b, PreLen pre, CallBack &... c)noexcept {
				return SubPrinComputeTo<typename _Traits::Add, subRange, Range, PreLen, CallBack...>(a, b, pre, c...);
			}

			template<typename subRange, typename Range, typename PreLen, typename... CallBack>
			static constexpr INLINED void MY_LIB SubtractFrom(subRange a, Range b, PreLen pre, CallBack &... c)noexcept {
				assert(CompareTo(a, b) != Compare::Larger);
				return SubPrinComputeTo<typename _Traits::SubtractFrom, subRange, Range, PreLen, CallBack...>(a, b, pre, c...);
			}

			template<typename Range, typename Data>
			[[deprecated]] static INLINED void MY_LIB MultiplyTo(Data a, Range b) noexcept {
				Data Carry = Data(0);
				LineIterator<typename _Traits::Multiply, Range, Data> mul(a, b);
				while (true) {
					//This element
					*(mul.b) = mul.Result.first;
					if (mul.b == end_ptr) {
						if (mul.Result.second != Data(0)) {
							mul.b.insert(mul.b, mul.Result.second);
						}
						break;
					}
					++mul;
				}
			}


			//Compare a to b.
			template<typename subRange, typename Range>
			static INLINED Compare MY_LIB CompareTo(const subRange &a, const Range &b) noexcept {
				static_assert(
					std::is_same_v<
					std::decay_t<decltype(*a.begin())>,
					std::decay_t<decltype(*b.begin())>
					>,
					"They should have the same type");
				if (a.begin() == a.end() || b.begin() == b.end())return Compare::Equal;
				{
					Compare temp = Compare::Equal;
					auto _a = a.begin();
					auto _b = b.begin();
					for (;
						;
						) {
						if (*_a > *_b) {
							temp = Compare::Larger;
						}
						else if (*_a < *_b) {
							temp = Compare::Smaller;
						}
						bool __a = (++_a, _a == a.end()), __b = (++_b, _b == b.end());
						if (__a && __b) {
							break;
						}
						else if (__a) {
							return Compare::Smaller;
						}
						else if (__b) {
							return Compare::Larger;
						}
					}
					return temp;
				}
			}
			//Extension for Compare()
			template<typename subRange, typename Range>
			static INLINED auto MY_LIB _CompareTo(const subRange &a, const Range &b) noexcept {
			#if RV_DISPLAY_ON
				for (auto i = a; i != nullptr; i = i + 1) {
					std::cout << *i << '\t';
				}
				std::cout << std::endl;
				for (auto i = b; i != nullptr; i = i + 1) {
					std::cout << *i << '\t';
				}
				std::cout << std::endl;
			#endif // RV_DISPLAY_ON
				using Data = typename std::decay_t<decltype(*a.begin())> ;
				static_assert(std::is_same_v<std::decay_t<decltype(*a.begin())>, std::decay_t<decltype(*b.begin())>>);
				constexpr auto PracticedRadix = ((_Traits::getRadix() == 0) ? std::numeric_limits<Data>::max() : _Traits::getRadix());
				{
					bool HasChanged = false;
					Compare temp = Compare::Equal;
					subRange _a = a;
					Range _b = b;
					for (;
						;
						++_a.begin(), ++_b.begin(), HasChanged = true
						) {
						if (*_a.begin() > *_b.begin()) {
							temp = Compare::Larger;
						}
						else if (*_a < *_b) {
							temp = Compare::Smaller;
						}
						subRange aNext(_a);
						Range bNext(_b);
						++aNext.begin(), ++bNext.begin();
						//I wonder whether I should use if-else or ?:
						if ((aNext.begin() == aNext.end()) && (bNext.begin() == bNext.end())) {
							if (temp == Compare::Larger) {
								return RV_PAIR_DISPLAY(std::pair<Data, Compare>(
									((*_a > *_b)
										?
										((!HasChanged && (*_b != 0))
											? Data(*_a / (*_b)) : Data(*_a / (*_b + Data(1))))
										:
										Data(1)),
									Compare::Larger));
							}
							else if (temp == Compare::Smaller) {
								return RV_PAIR_DISPLAY(std::pair<Data, Compare>(
									((*_a < *_b)
										?
										((!HasChanged && (*_a != 0))
											? Data(*_b / (*_a)) : Data(*_b / (*_a + Data(1))))
										:
										Data(1)),
									Compare::Smaller));
							}
							else {
								return RV_PAIR_DISPLAY(std::pair<Data, Compare>((1), Compare::Equal));
							}
						}
						else if (aNext.begin() == aNext.end()) {
							return RV_PAIR_DISPLAY(std::pair<Data, Compare>(
								((*bNext > *_a || (!HasChanged && *bNext == *_a))
									? (PracticedRadix - 1) : (PracticedRadix / (*_a + 1) * *bNext)),
								Compare::Smaller));
						}
						else if (bNext.begin() == bNext.end()) {
							return RV_PAIR_DISPLAY(std::pair<Data, Compare>(
								((*aNext > *_b || (!HasChanged && *aNext == *_b))
									? (PracticedRadix - 1) : (PracticedRadix / (*_b + 1) * *aNext)),
								Compare::Larger));
						}
					}
				}
			}
			template<typename Accumulation, typename Recursion, typename subRange, typename Range, typename PreLen, typename Data>
			static INLINED void MY_LIB __DivideInto(const subRange &_a, const Range &_b, PreLen pre, Recursion Move, Accumulation Accum)noexcept {
				{
					switch (CompareTo(_a, _b)) {
					case Compare::Larger:
						return;
					case Compare::Equal:
						Accum(_a, _b, 1);
						return;
					case Compare::Smaller:
					{
						Range next(_b);
						++next.begin();
						__DivideInto<Accumulation, Recursion, subRange, Range, PreLen, Data>(_a, next, pre, Move, Accum);
						Move();
					}
						break;
					default:
						assert(false);
						break;
					}
				}
				while (true) {
					auto [res, cmpr] = _CompareTo(_b, _a);
					if (cmpr == Compare::Smaller) {
						assert(CompareTo(_b, _a) == Compare::Smaller);
						return;
					}
					else {
						assert(CompareTo(_b, _a) != Compare::Smaller);
						if (cmpr == Compare::Equal) Accum(_a, _b, 1);
						else Accum(_a, _b, res);
						//return;
					}
				}
			}
			template<typename Linear, typename subRange, typename Range, typename Simplify, typename PreLen>
			static INLINED void MY_LIB DivideInto(Linear &Res, subRange a, Range b, Simplify s, PreLen pre) noexcept {
				static_assert(std::is_same_v<std::decay_t<decltype(*a.begin())>, std::decay_t<decltype(*b.begin())>>, "Same type required");
				using Data = typename std::decay_t<decltype(*a.begin())>;
				//Regarding of the compatibility, we didn't use any majorization.
				auto func1 = [&Res, &pre, &s](const subRange &a, const Range &b, Data times)->void {
					LineIterator<typename _Traits::Multiply, subRange, decltype(times)> temp(times, a);
					SubtractFrom(iter_range<decltype(temp), nullptr_t>(temp, nullptr), b, pre);
					s(b);
					Res += times;
				};
				auto func2 = [&Res]()->void {Res <<= 1; };
				__DivideInto<decltype(func1), decltype(func2), subRange, Range, PreLen, Data>(a, b, pre, func2, func1);
			}
			template<typename subRange, typename Range, typename Simplify, typename PreLen>
			static INLINED void MY_LIB DivideInto(subRange a, Range b, Simplify s, PreLen pre) {
				static_assert(std::is_same_v<std::decay_t<decltype(*a.begin())>, std::decay_t<decltype(*b.begin())>>, "Same type required.");
				using Data = typename std::decay_t<decltype(*a.begin())>;
				//Regarding of the compatibility, we didn't use any majorization.
				auto func = [&pre, &s](const subRange &a, const Range &b, Data times)->void {
					LineIterator<typename _Traits::Multiply, subRange, decltype(times)> temp(times, a);
					SubtractFrom(iter_range<decltype(temp), nullptr_t>(temp, nullptr), b, pre);
					s(b);
				};
				Empty null;
				__DivideInto<decltype(func), Empty, subRange, Range, PreLen, Data>(a, b, pre, null, func);
			}
		};

		template<typename Data>
		class StdCmptTraits {
		public:
			StdCmptTraits() = delete;
			~StdCmptTraits() = delete;
			static constexpr Data MY_LIB getRadix()noexcept { return static_cast<Data>(0); }
			class Add {
			public:
				MY_LIB Add()noexcept { }
				MY_LIB ~Add()noexcept { }
				std::pair<Data, Data> MY_LIB operator()(Data Carry, const Data &a, const Data &b)noexcept {
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

			class SubtractFrom {
			public:
				MY_LIB SubtractFrom()noexcept { }
				MY_LIB ~SubtractFrom()noexcept { }
				std::pair<Data, Data> MY_LIB operator()(Data Carry, const Data &a, const Data &b)noexcept {
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

			class Multiply {
			public:
				MY_LIB Multiply()noexcept { }
				MY_LIB ~Multiply()noexcept { }
				std::pair<Data, Data> MY_LIB operator()(Data Carry, Data a, Data b)noexcept {
					if (a > b) {
						Data temp = b;
						b = a;
						a = temp;
					}
					assert(a <= b);
					if (a == Data(0)) {
						return std::pair<Data, Data>(Carry, Data(0));
					}
					Data Res = b;
					if (Res > Data(~Carry)) { Carry = Data(1); }
					else Carry = Data(0);
					for (Data i = Data(1); i < a; ++i) {
						if (Res > Data(~b)) {
							Carry += Data(1);
						}
						Res += b;
					}
					return std::pair<Data, Data>(Res, Carry);
				}
			};

			template<auto divisor>
			class Divide {
			public:
				using radix_t = decltype(divisor);
				MY_LIB Divide()noexcept { }
				MY_LIB ~Divide()noexcept { }

				std::pair<radix_t, Data> operator()(const Data &that) noexcept {
					static_assert(sizeof(radix_t) * 2 >= sizeof(Data), "It can't be stored safely!");
					using resT = std::pair<radix_t, Data>;
					static_assert(std::is_same_v<Depack_t<Data>, Data>, "The type must not be num!");
					if constexpr (divisor == 0) {
						if constexpr (sizeof(divisor) >= sizeof(Data)) {
							return resT(static_cast<radix_t>(that), static_cast <Data>(0));
						}
						else return resT(
							static_cast<radix_t>(that),
							static_cast<Data>(
								that >> (LargeInteger::BitsPerByte * sizeof(divisor))
								)
						);
					}
					else return resT(
						static_cast<radix_t>(that % divisor),
						static_cast<Data>(that / divisor)
					);
				}
			};


		private:

		};
	}
}