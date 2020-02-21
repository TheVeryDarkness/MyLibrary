#pragma once

#include "Shared.h"
#include "ConstantBuffer.h"
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
			static_assert(std::is_same_v<typename Iter1::value_type, typename Iter2::value_type>, "Same type required.");
			using value_type = typename Iter1::value_type;
			constexpr iter_range(Iter1 begin, Iter2 end)noexcept :_begin(begin), _end(end) { }
			constexpr Iter1 &begin()noexcept { return this->_begin; }
			constexpr Iter2 &end()noexcept { return this->_end; }
			constexpr const Iter1 &begin()const noexcept { return this->_begin; }
			constexpr const Iter2 &end()const noexcept { return this->_end; }
			constexpr bool operator==(nullptr_t)const noexcept { return this->_begin == this->_end; }
			constexpr bool operator!=(nullptr_t)const noexcept { return !(*this == nullptr); }
			constexpr auto &operator++()noexcept {
				if (_begin != _end) {
					++_begin;
				}
				return *this;
			}
			auto &operator*()noexcept {
				return *_begin;
			}
		private:
			Iter1 _begin;
			Iter2 _end;
		};
		template<typename Iter1>
		struct iter_range<Iter1, nullptr_t> {
		public:
			using value_type = typename Iter1::value_type;
			constexpr iter_range(Iter1 begin, nullptr_t)noexcept :_begin(begin) { }
			constexpr Iter1 &begin()noexcept { return this->_begin; }
			constexpr nullptr_t end()noexcept { return nullptr; }
			constexpr const Iter1 &begin()const noexcept { return this->_begin; }
			constexpr nullptr_t end()const noexcept { return nullptr; }
			constexpr bool operator==(nullptr_t)const noexcept { return this->_begin == nullptr; }
			constexpr bool operator!=(nullptr_t)const noexcept { return this->_begin != nullptr; }
			constexpr auto &operator++()noexcept {
				if (_begin != nullptr) {
					++_begin;
				}
				return *this;
			}
			auto operator*()noexcept {
				return *_begin;
			}
		private:
			Iter1 _begin;
		};
		template<typename Iter1, typename Iter2, typename Cntnr>
		struct receive_range {
		private:
			constexpr Iter1 operator()(size_t len = 1)noexcept {
				bool is_end = false;
				Iter1 p = _begin;
				for (; len > 0; --len) {
					if (is_end || (ending(p, _end) && (is_end = true, true))) {
						p = c.emplace_after(p, 0);
						_end = c.end();
					}
					else ++p;
				}
				return p;
			}
			static constexpr bool ending(const Iter1 &p, const Iter2 &end) noexcept {
				Iter1 tmp = p;
				++tmp;
				return tmp == end;
			}
		public:
			static_assert(std::is_same_v<typename Iter1::value_type, typename Iter2::value_type>, "Same type required.");
			using value_type = typename Iter1::value_type;
			constexpr receive_range(const receive_range &)noexcept = default;
			constexpr receive_range(Iter1 begin, Iter2 end, Cntnr &p)noexcept
				:_begin(begin), _end(end), c(p) { }
			[[nodiscard]] constexpr const Iter1 &begin()const noexcept { return zeros ? this->_end : this->_begin; }
			[[nodiscard]] constexpr const Iter2 &end()const noexcept { return this->_end; }
			const auto &operator*()const noexcept {
				return ((_begin == _end || zeros) ? ConstantBuffer<std::decay_t<decltype(*_begin)>>::get() : *_begin);
			}
			constexpr bool operator==(nullptr_t)const noexcept { return this->_begin == this->_end || zeros; }
			constexpr bool operator!=(nullptr_t)const noexcept { return !(*this == nullptr); }
			constexpr auto &operator++()noexcept {
				if (!ending(_begin, _end)) {
					++_begin;
				}
				else {
					++zeros;
				}
				return *this;
			}
			constexpr receive_range &operator=(const receive_range &)noexcept = default;
			constexpr receive_range &operator=(receive_range &&)noexcept = default;
			template<typename Data>constexpr auto &operator=(Data data)noexcept {
				if (data) {
					_begin = operator()(zeros), zeros = 0;
					*_begin = data;
				}
				return *this;
			}
		private:
			size_t zeros = 0;
			Iter1 _begin;
			Iter2 _end;
			Cntnr &c;
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
			template<class ComputeFunction, typename Range, typename Data>
			class LineIterator {
			private:
				ComputeFunction c;
				//result;overflow
				std::pair<Data, Data> Result;
				Data a;
				Range b;
			public:
				using value_type = Data;
				static_assert(std::is_same_v<Data, std::decay_t<typename Range::value_type>>, "It should be the same type");
				MY_LIB LineIterator(Data a, Range b)noexcept :a(a), b(b), c(), Result(c(Data(0), a, *b)) { }
				MY_LIB LineIterator(Data a, Range b, Data Carry)noexcept :a(a), b(b), c(), Result(c(Carry, a, *b)) { }
				MY_LIB ~LineIterator()noexcept = default;
				//Notice:
				//	this function move the iterator b to its next place
				void MY_LIB operator++() noexcept {
					if (b != nullptr) {
						++b;
					}
					Result = c(Result.second, a, (b != nullptr ? *b : 0));
				}
				const Data &MY_LIB operator*()const noexcept {
					return Result.first;
				};
				constexpr bool MY_LIB operator!=(nullptr_t)const noexcept {
					return !(*this == nullptr);
				}
				constexpr bool MY_LIB operator==(nullptr_t)const noexcept {
					return a == 0 || ((this->b == nullptr) && (Result.first == 0) && (Result.second == 0));
				}
			};


			template<class ComputeFunction, typename Data, typename Value>
			class LayerIterator {
			public:
				using value_type = Data;
				LayerIterator(Value val) noexcept :c(), Result(c(val)) { }
				~LayerIterator()noexcept = default;
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


			template<typename Compute, typename subRange, typename Range, typename... CallBack>
			static constexpr INLINED void MY_LIB SubPrinComputeTo(const subRange &_a, const Range &_b, CallBack &... cb)noexcept {
				static_assert(std::is_same_v<
					std::decay_t<typename subRange::value_type>,
					std::decay_t<typename Range::value_type>>,
					"They should have the same type.");
				using Data = std::decay_t<typename subRange::value_type>;
				if (_a == nullptr) return;
				Compute c;
				subRange a(_a);
				Range b(_b);
				//result;overflow
				std::pair<Data, Data> Result = c(0, *a, *b);
				//This element
				for (;; ++b, Result = c(Result.second, *a, *b)) {
					(cb(), ...);
					b = Result.first;
					if (a == nullptr) {
						if (Result.second == 0) {
							break;
						}
					}
					else ++a;
				}
			}

			template<typename subRange, typename Range, typename... CallBack>
			static constexpr INLINED void MY_LIB AddTo(subRange a, Range b, CallBack &... c)noexcept {
				return SubPrinComputeTo<typename _Traits::Add, subRange, Range, CallBack...>(a, b, c...);
			}

			template<typename subRange, typename Range, typename... CallBack>
			static constexpr INLINED void MY_LIB SubtractFrom(subRange a, Range b, CallBack &... c)noexcept {
				assert(CompareTo(a, b) != Compare::Larger);
				return SubPrinComputeTo<typename _Traits::SubtractFrom, subRange, Range, CallBack...>(a, b, c...);
			}


			//Compare a to b.
			template<typename subRange, typename Range>
			static INLINED Compare MY_LIB CompareTo(const subRange &a, const Range &b) noexcept {
				static_assert(
					std::is_same_v<
					std::decay_t<typename subRange::value_type>,
					std::decay_t<typename Range::value_type>
					>,
					"They should have the same type");
				if (a == nullptr || b == nullptr)return Compare::Equal;
				{
					Compare temp = Compare::Equal;
					auto _a = a;
					auto _b = b;
					for (;
						;
						) {
						if (*_a > *_b) {
							temp = Compare::Larger;
						}
						else if (*_a < *_b) {
							temp = Compare::Smaller;
						}
						bool __a = (++_a, _a == nullptr), __b = (++_b, _b == nullptr);
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
				for (auto i = a; i != nullptr; ++i) {
					std::cout << *i << '\t';
				}
				std::cout << std::endl;
				for (auto i = b; i != nullptr; ++i) {
					std::cout << *i << '\t';
				}
				std::cout << std::endl;
			#endif // RV_DISPLAY_ON
				using Data = typename std::decay_t<typename subRange::value_type>;
				static_assert(std::is_same_v<std::decay_t<typename subRange::value_type>, std::decay_t<typename Range::value_type>>);
				constexpr auto PracticedRadix = ((_Traits::getRadix() == 0) ? std::numeric_limits<Data>::max() : _Traits::getRadix());
				{
					bool HasChanged = false;
					Compare temp = Compare::Equal;
					subRange _a = a;
					Range _b = b;
					for (;
						;
						++_a, ++_b, HasChanged = true
						) {
						if (*_a > *_b) {
							temp = Compare::Larger;
						}
						else if (*_a < *_b) {
							temp = Compare::Smaller;
						}
						subRange aNext(_a);
						Range bNext(_b);
						++aNext, ++bNext;
						//I wonder whether I should use if-else or ?:
						if ((aNext == nullptr) && (bNext == nullptr)) {
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
								return RV_PAIR_DISPLAY(std::pair<Data, Compare>(1, Compare::Equal));
							}
						}
						else if (aNext == nullptr) {
							return RV_PAIR_DISPLAY(std::pair<Data, Compare>(
								((*bNext > *_a || (!HasChanged && *bNext == *_a))
									? (PracticedRadix - 1) : (PracticedRadix / (*_a + 1) * *bNext)),
								Compare::Smaller));
						}
						else if (bNext == nullptr) {
							return RV_PAIR_DISPLAY(std::pair<Data, Compare>(
								((*aNext > *_b || (!HasChanged && *aNext == *_b))
									? (PracticedRadix - 1) : (PracticedRadix / (*_b + 1) * *aNext)),
								Compare::Larger));
						}
					}
				}
			}
			template<typename Accumulation, typename Recursion, typename subRange, typename Range, typename Data>
			static INLINED void MY_LIB __DivideInto(const subRange &_a, const Range &_b, Recursion Move, Accumulation Accum)noexcept {
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
						++next;
						__DivideInto<Accumulation, Recursion, subRange, Range, Data>(_a, next, Move, Accum);
						Move();
					}
					break;
					default:
						assert(false);
					}
				}
				while (true) {
					auto [res, cmpr] = _CompareTo(_b, _a);
					assert(res != 0);
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
			template<typename Linear, typename subRange, typename Range, typename Simplify>
			static INLINED void MY_LIB DivideInto(Linear &Res, subRange a, Range b, Simplify s) noexcept {
				static_assert(std::is_same_v<std::decay_t<typename subRange::value_type>, std::decay_t<typename Range::value_type>>, "Same type required");
				using Data = typename std::decay_t<typename subRange::value_type>;
				//Regarding of the compatibility, we didn't use any majorization.
				auto func1 = [&Res, &s](const subRange &a, const Range &b, Data times)->void {
					LineIterator<typename _Traits::Multiply, subRange, decltype(times)> temp(times, a);
					SubtractFrom(iter_range<decltype(temp), nullptr_t>(temp, nullptr), b);
					s(b);
					Res += times;
				};
				auto func2 = [&Res]()->void {Res <<= 1; };
				__DivideInto<decltype(func1), decltype(func2), subRange, Range, Data>(a, b, func2, func1);
			}
			template<typename subRange, typename Range, typename Simplify>
			static INLINED void MY_LIB DivideInto(subRange a, Range b, Simplify s) {
				static_assert(std::is_same_v<std::decay_t<typename subRange::value_type>, std::decay_t<typename Range::value_type>>, "Same type required.");
				using Data = typename std::decay_t<typename subRange::value_type>;
				//Regarding of the compatibility, we didn't use any majorization.
				auto func = [&s](const subRange &a, const Range &b, Data times)->void {
					LineIterator<typename _Traits::Multiply, subRange, decltype(times)> temp(times, a);
					SubtractFrom(iter_range<decltype(temp), nullptr_t>(temp, nullptr), b);
					s(b);
				};
				Empty null;
				__DivideInto<decltype(func), Empty, subRange, Range, Data>(a, b, null, func);
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