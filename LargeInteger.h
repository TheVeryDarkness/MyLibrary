#pragma once

#include "Shared.h"
#include "Statistics.h"
#include "ComputeTemplate.h"
#include "CustomizedRadixCharSet.h"
#include "_Bytes.h"
#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>

namespace Darkness {
	inline namespace LargeInteger {
		template<typename LL, typename LL::value_type radix> class LargeUnsigned;
		template<typename LL, typename LL::value_type radix> class LargeSigned;
		template<auto Radix> class _LLCmptTraits;
		template<auto Radix, bool is0> class helper;

		template<typename T>constexpr bool isZero(T t)noexcept {
			return t == T(0);
		}

		template<auto Radix> using LLCmptTraits = typename helper<Radix, isZero(Radix)>::trts;

		template<auto Radix>
		class helper<Radix, true> {
		public:
			using trts = StdCmptTraits<decltype(Radix)>;
		};

		template<auto Radix>
		class helper<Radix, false> {
		public:
			using trts = _LLCmptTraits<Radix>;
		};



		template<auto Radix>
		class _LLCmptTraits :public LargeInteger::StdCmptTraits<decltype(Radix)> {
		public:
			using Std = LargeInteger::StdCmptTraits<decltype(Radix)>;
			static_assert(Radix != 0, "This is not aimed at dealed with 0 radix.");
			using Data = decltype(Radix);
			MY_LIB _LLCmptTraits() = delete;
			MY_LIB ~_LLCmptTraits() = delete;
			static constexpr Data getRadix()noexcept {
				return Radix;
			}

			class Add {
			public:
				MY_LIB Add()noexcept { }
				MY_LIB ~Add()noexcept { }
				std::pair<Data, Data> MY_LIB operator()(Data Carry, const Data &a, const Data &b)noexcept {
					assert(Carry < Radix);
					assert(a < Radix);
					assert(b < Radix);
					const bool &&overflow = ((Carry > 0) ?
						(
						(a >= static_cast<Data>((Radix - 1) - b))
							? true : false)
						:
						(a >= static_cast<Data>(Radix - b)
							? true : false));
					return std::pair<Data, Data>(
						overflow ? (a - (Radix - b - Carry)) : (a + b + Carry),
						Data(overflow)
						);
				};
			};

			class SubtractFrom {
			public:
				MY_LIB SubtractFrom()noexcept { }
				MY_LIB ~SubtractFrom()noexcept { }
				std::pair<Data, Data> MY_LIB operator()(Data Carry, const Data &a, const Data &b)noexcept {
					assert(Carry < Radix);
					assert(a < Radix);
					assert(b < Radix);
					const bool &&underFlow =
						(Carry > 0) ?
						((b <= a) ? true : false)
						:
						((b < a) ? true : false);
					return std::pair<Data, Data>(
						underFlow ? (b + (Radix - a - Carry)) : (b - a - Carry),
						Data(underFlow)
						);
				}
			};
			class Multiply {
			public:
				using Num = decltype(Radix);
				using Data = Num;
				Multiply() = default;
				~Multiply() = default;

				//remain, ratio
				std::pair<Num, Num> MY_LIB operator()(Num Carry, Num a, Num b)const noexcept {
					assert(Carry < Radix);
					assert(a < Radix);
					assert(b < Radix);
					using namespace LargeInteger;
					if constexpr (IntelligentLength<2 * GetMinLength(Radix)>().first == 0) {
						using wT = typename _Int<IntelligentLength<2 * GetMinLength(Radix) * 2>().second>::type;
						if constexpr (Radix > std::numeric_limits<Data>::max() / Radix) {
							wT This(a);
							This *= wT(b);
							This += wT(Carry);
							wT &&radix = static_cast<wT>(Radix);
							return std::pair<Num, Num>(
								Num(This % radix),
								Num(This / radix)
								);
						}
						else {
							a *= b;
							a += Carry;
							return std::pair<Num, Num>(Num(a % Radix), Num(a / Radix));
						}
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
							a *= b;
							a += Carry;
							return std::pair<Num, Num>(Num(a % Radix), Num(a / Radix));
						}
					}
				}
			};
		};


		template<typename Cntnr, typename BaseType>
		//简单输出到控制台窗口
		//需要用户补换行
		INLINED void MY_LIB SinglePrint(
			const Cntnr &that,
			std::ostream &out,
			bool ShowComma = true,
			unsigned MinLength = 0,
			BaseType base = 10
		) noexcept {
			if (that + 1 != nullptr) {
				SinglePrint(that + 1, out, ShowComma, MinLength, base);
				out << ((ShowComma) ? "," : "");

				auto tmp = *that;
				std::string res;
				while (tmp) {
					res.push_back(Set<base>::to_char_type(tmp % base));
				}
				auto Fill = out.fill();
				out << std::setw(MinLength) << std::setfill('0') << res << std::setfill(Fill);
			}
			else {
				out << *that;
			}
			return;
		}

		template<
			typename Iter1,
			typename Iter2,
			typename BaseType,
			bool ShowComma = true,
			unsigned MinLength = 0,
			BaseType base = 10>
			//简单输出到控制台窗口
			//需要用户补换行
			INLINED bool MY_LIB SinglePrint(
				const Iter1 &begin,
				const Iter2 &end,
				std::ostream &out
			) noexcept {
			static_assert(base < BaseType(INT_MAX), "Base not supprted");
			if (begin != end) {
				if (SinglePrint<Iter1, Iter2, BaseType, ShowComma, MinLength, base>(++Iter1(begin), end, out)) {
					out << *begin;
				}
				else {
					out << ((ShowComma) ? "," : "");
					if constexpr (base == 10 || base == 16 || base == 8) {
						out << std::setbase(base) << std::setw(MinLength) << std::setfill('0') << *begin;
					}
					else {
						static_assert(MinLength == 1, "Error");
						if constexpr (MinLength == 1) {
							out << *begin;
						}
						else {
							auto tmp = *begin;
							std::string res;
							while (tmp) {
								res.push_back(Set<base>::to_char_type(tmp % base));
							}
							res.reserve();

							auto Fill = out.fill();
							out << std::setw(MinLength) << std::setfill('0') << res << std::setfill(Fill);
						}
					}
				}
				return false;
			}
			else return true;
		}

		template<typename LL, typename LL::value_type radix>
		class LargeUnsigned :protected LL {
		public:
			using radix_t = decltype(radix);
			using Data = radix_t;
		protected:
			static_assert(radix != radix_t(1), "Radix can't be 1");
			static_assert(radix >= 0, "Positive radix required.");
			static_assert(std::is_same_v<radix_t, typename LL::value_type>, "Value type should be the same");

			template<typename Range> INLINED void MY_LIB range_mul(const Range &b) noexcept {
				LargeUnsigned This(std::move(*static_cast<LL *>(this)));
				this->emplace_front(0);
				//this->destruct();
				auto Ptr = this->arange();
				auto OprtPtr = b.begin();
				for (; ; ) {
					typename LargeInteger::template LongCmpt<typename LargeInteger::LLCmptTraits<radix>>::template LineIterator<typename LargeInteger::LLCmptTraits<radix>::Multiply, decltype(This.crange()), Data> temp(*OprtPtr, This.crange());
					LargeInteger::LongCmpt<typename LargeInteger::LLCmptTraits<radix>>::template AddTo(iter_range<decltype(temp), nullptr_t>(temp, nullptr), Ptr);
					++OprtPtr;
					if (OprtPtr == b.end())break;
					++Ptr;
				}
				//This.release();
			}
			template<typename range>
			class pre {
			public:
				pre(LargeUnsigned *ptr)noexcept :p(ptr) { }
				auto operator()(range it, size_t l = 1) noexcept {
					for (; l != 0; --l) {
						auto tmp(it.begin());
						++tmp;
						if (tmp == it.end()) {
							it.begin() = p->LL::emplace_after(it.begin(), 0);
							it.end() = p->end();
						}
					}
					return it;
				}

			private:
				LargeUnsigned *const p;
			};

			constexpr auto pre1() noexcept {
				return pre<decltype(this->range())>(this);
			}
			constexpr auto pre2() const noexcept {
				return pre<decltype(this->crange())>(this);
			}
			template<typename Range> INLINED void MY_LIB range_add(const Range &b) noexcept {
				LargeInteger::LongCmpt<LLCmptTraits<radix>>::AddTo(b, this->arange());
			}
			template<typename Range> INLINED void MY_LIB range_sub(const Range &b) noexcept {
				LargeInteger::LongCmpt<LLCmptTraits<radix>>::SubtractFrom(b, this->arange());
			}
			template<typename Range> INLINED void MY_LIB range_div(LargeUnsigned &Res, const Range &b) noexcept {
				LargeInteger::LongCmpt<LLCmptTraits<radix>>::template DivideInto(Res, b, this->arange(), Sim(this));
			}
			template<typename Range> INLINED void MY_LIB range_div(const Range &b) noexcept {
				LargeInteger::LongCmpt<LLCmptTraits<radix>>::template DivideInto(b, this->arange(), Sim(this));
			}
			//*this < b
			template<typename Range> INLINED bool MY_LIB range_smaller(const Range &b) const noexcept {
				return (LargeInteger::LongCmpt<LLCmptTraits<radix>>::CompareTo(this->crange(), b) == Compare::Smaller);
			}
			//*this > b
			template<typename Range> INLINED bool MY_LIB range_larger(const Range &b) const noexcept {
				return (LargeInteger::LongCmpt<LLCmptTraits<radix>>::CompareTo(this->crange(), b) == Compare::Larger);
			}
			//*this == b
			template<typename Range> INLINED bool MY_LIB range_equal(const Range &b) const noexcept {
				return (LargeInteger::LongCmpt<LLCmptTraits<radix>>::CompareTo(this->crange(), b) == Compare::Equal);
			}
		public:
			using super = LL;
			static constexpr radix_t getRadix()noexcept { return radix; }
			constexpr INLINED auto range() noexcept {
				return iter_range<decltype(this->begin()), decltype(this->end())>{ this->begin(), this->end() };
			}
			constexpr INLINED auto arange() noexcept {
				return receive_range<decltype(this->begin()), decltype(this->end()), LL>{ this->begin(), this->end(), *this };
			}
			constexpr INLINED auto range() const noexcept {
				return iter_range<decltype(this->cbegin()), decltype(this->cend())>{ this->cbegin(), this->cend() };
			}
			constexpr INLINED auto crange() const noexcept {
				return iter_range<decltype(this->cbegin()), decltype(this->cend())>{ this->cbegin(), this->cend() };
			}
			constexpr INLINED auto begin() noexcept {
				return this->LL::begin();
			}
			constexpr INLINED auto begin()const noexcept {
				return this->LL::begin();
			}
			constexpr INLINED auto cbegin()const noexcept {
				return this->LL::cbegin();
			}
			constexpr INLINED auto end() noexcept {
				return this->LL::end();
			}
			constexpr INLINED auto end() const noexcept {
				return this->LL::end();
			}
			constexpr INLINED auto cend() const noexcept {
				return this->LL::cend();
			}
			constexpr INLINED auto rbegin() noexcept {
				return this->LL::rbegin();
			}
			constexpr INLINED auto rbegin()const noexcept {
				return this->LL::rbegin();
			}
			constexpr INLINED auto crbegin()const noexcept {
				return this->LL::crbegin();
			}
			constexpr INLINED auto rend() noexcept {
				return this->LL::rend();
			}
			constexpr INLINED auto rend() const noexcept {
				return this->LL::rend();
			}
			constexpr INLINED auto crend() const noexcept {
				return this->LL::crend();
			}
			template<typename val> static auto Layer(const val &Val)noexcept {
				return typename LongCmpt<StdCmptTraits<val>>::template LayerIterator<typename StdCmptTraits<val>::template Divide<radix>, radix_t, val>(Val);
			}
			template<typename val>static auto LayerRange(const val &Val)noexcept {
				return iter_range<decltype(Layer(Val)), nullptr_t>(Layer(Val), nullptr);
			}
			template<typename val>
			explicit MY_LIB LargeUnsigned(val Val)noexcept :LL(0) {
				static_assert(std::is_integral_v<val>, "Integral type required.");
				static_assert(!std::is_same_v<val, bool>, "Never use bool type");
				if (Val == 0) {
					this->emplace_front(0);
					return;
				}
				auto it = Layer(Val);
				for (auto index = this->before_begin(); it != nullptr; ) {
					index = this->LL::emplace_after(index, *it);
					*index = *it;
					++it;
				}
				return;
			}
			//end should not points to '\0'
			static LargeUnsigned MY_LIB MakeFromString(const char *begin, const char *end)noexcept {
				assert(end >= begin);
				for (; end >= begin && *end == '\0'; --end);
				for (; end >= begin && *begin == '0'; ++begin);
				size_t i = 0;
				radix_t sum = 0;
				for (; end >= begin; --end) {
					if (Set<base>::exist(*end)) {
						sum += (*end - '0') * Power(base, i);
						++i;
						if (i == len - 1) {
							i = 0;
							break;
						}
					}
				}
				LargeUnsigned res(sum);
				sum = 0;
				auto iter = res.begin();
				for (; end >= begin; --end) {
					if (*end <= '9' || *end >= '0') {
						sum += Power(base, i);
						++i;
						if (i == len - 1) {
							i = 0;
							++iter;
							*iter = sum;
							sum = 0;
						}
					}
				}
				return res;
			}
			static LargeUnsigned MY_LIB MakeFromString(const char *str)noexcept {
				const char *end = str;
				for (; *end != '\0'; ++end);
				return MakeFromString(str, end);
			}
			explicit MY_LIB LargeUnsigned(const char *str)noexcept
				:LL(std::move(MakeFromString(str))) { }
			explicit MY_LIB LargeUnsigned(const char *begin, const char *end)noexcept
				:LL(std::move(MakeFromString(begin, end))) {

			}
			explicit MY_LIB LargeUnsigned(LL &&ll)noexcept :LL(std::move(ll)) { }
			MY_LIB LargeUnsigned(LargeUnsigned &&ll)noexcept :LL(static_cast<LL &&>(ll)) { }
			LargeUnsigned &MY_LIB operator=(LargeUnsigned &&ll)noexcept {
				this->LL::operator=(static_cast<LL &&>(ll));
				return *this;
			}
			static constexpr LargeUnsigned MY_LIB Copy(const LargeUnsigned &that)noexcept {
				return LargeUnsigned(std::move(LL(that)));
			}
			template<auto Radix>
			class RadixSelector {
			public:
				using Radix_t = decltype(Radix);
				RadixSelector() = delete;
				~RadixSelector() = delete;
				constexpr static Radix_t MY_LIB Base()noexcept {
					if constexpr (Math::GetPowerTimes(Radix, static_cast<Radix_t>(10U)) != 0) {
						return 10;
					}
					if constexpr (Math::GetPowerTimes(Radix, static_cast<Radix_t>(16U)) != 0) {
						return 16;
					}
					if constexpr (Math::GetPowerTimes(Radix, static_cast<Radix_t>(8U)) != 0) {
						return 8;
					}
					if constexpr (Math::GetPowerTimes(Radix, static_cast<Radix_t>(2U)) != 0) {
						return 2;
					}
					return Radix;
				}
				constexpr static Radix_t MY_LIB Length_Each()noexcept {
					if constexpr (Math::GetPowerTimes(Radix, static_cast<Radix_t>(10U)) != 0) {
						return Math::GetPowerTimes(Radix, static_cast<Radix_t>(10U));
					}
					if constexpr (Math::GetPowerTimes(Radix, static_cast<Radix_t>(16U)) != 0) {
						return Math::GetPowerTimes(Radix, static_cast<Radix_t>(16U));
					}
					if constexpr (Math::GetPowerTimes(Radix, static_cast<Radix_t>(8U)) != 0) {
						return Math::GetPowerTimes(Radix, static_cast<Radix_t>(8U));
					}
					if constexpr (Math::GetPowerTimes(Radix, static_cast<Radix_t>(2U)) != 0) {
						return Math::GetPowerTimes(Radix, static_cast<Radix_t>(2U));
					}
					return Radix_t(1);
				}
			};
			template<typename Iter1, typename Iter2, auto Radix>
			//二进制输出到控制台窗口
			//不再自动换行
			static /*INLINED*/std::ostream &MY_LIB _Print(
				const Iter1 &begin,
				const Iter2 &end,
				std::ostream &out
			) noexcept {
				if constexpr (Radix == static_cast<decltype(Radix)>(0)) {
					out << "0x"
						<< std::setbase(16);
					LargeInteger::SinglePrint(begin, end, out, false, 2 * sizeof(Radix), 16);
				}
				else {
					constexpr auto a = RadixSelector<Radix>::Base();
					constexpr auto b = RadixSelector<Radix>::Length_Each();


					if constexpr (a == 10) {
						LargeInteger::SinglePrint<Iter1, Iter2, int, false, b, a>(begin, end, out);
					}
					else if constexpr (a == 16) {
						out << "0x" << std::setbase(10);
						LargeInteger::SinglePrint<Iter1, Iter2, int, false, b, a>(begin, end, out);
					}
					else if constexpr (a == 8) {
						out << "0" << std::setbase(8);
						LargeInteger::SinglePrint<Iter1, Iter2, int, false, b, a>(begin, end, out);
					}
					else if constexpr (a == 2) {
						out << "0b" << std::setbase(2);
						LargeInteger::SinglePrint<Iter1, Iter2, int, false, b, a>(begin, end, out);
					}
					else {
						out << "(Base:"
							<< Radix
							<< ")";
						LargeInteger::SinglePrint<Iter1, Iter2, int, true, b, a>(begin, end, out);
					}
				}
				out << std::setbase(10);
				return out;
			}
			INLINED std::ostream &MY_LIB Print(std::ostream &o) const noexcept {
				return _Print<decltype(this->cbegin()), decltype(this->cend()), radix>(this->cbegin(), this->cend(), o);
			}
			//从控制台输入
			INLINED friend std::istream &MY_LIB operator>>(
				std::istream &in,
				LargeUnsigned &l
				) noexcept {
				return l.Scan(in);
			}
		protected:
			constexpr static radix_t base = RadixSelector<radix>::Base();
			constexpr static radix_t len = RadixSelector<radix>::Length_Each();
			template<typename _begin, typename _end>Data iter(_begin Begin, _end End) {
				Data sum = 0;
				for (auto i = Begin; ;) {
					if (i != End) sum *= base; else break;
					sum += *i;
					++i;
				}
				return sum;
			}
			template<typename end, bool is_end> auto scan(
				std::istream &in,
				std::vector<char, std::allocator<char>> &arr
			) {
				if constexpr (is_end) {
					if (Set<base>::to_int_type(static_cast<char>(in.peek())) == 0) {
						in.ignore();
					}
				}
				auto c = static_cast<char>(in.get());
				end e;
				if (e(c)) {
					return this->begin();
				}
				else {
					using Iter = decltype(scan<end, false>(in, arr));
					auto intgr = Set<base>::to_int_type(c);
					Iter it = scan<end, false>(in, arr);
					if (arr.size() == len) {
						*it = iter(arr.rbegin(), arr.rend());
						arr.resize(0);
						++it;
					}
					assert(arr.size() < len);
					if (intgr != '?') {
						arr.push_back(intgr);
					}
					if constexpr (is_end) {
						*it = iter(arr.rbegin(), arr.rend());
						arr.resize(0);
						return it;
					}
					else return it;
				}
			}
		public:
			template<char...Delim>char __stdcall get_until(std::istream &in)noexcept {
				using charset = BaseSet<char, char, Delim...>;
				static_assert(sizeof...(Delim) > 0, "Delim should be given");
				auto lambda = [](char c) { return charset::exist(c); };
				std::vector<char, std::allocator<char>> vec(len, 0);
				vec.resize(0);
				scan<decltype(lambda), true>(in, vec);
				return static_cast<char>(in.get());
			}
			INLINED std::istream &MY_LIB Scan(std::istream &in) noexcept {
				auto lambda = [](char c) { return !Set<base>::exist(c); };
				std::vector<char> vec(len, 0);
				vec.resize(0);
				scan<decltype(lambda), true>(in, vec);
				return in;
			}
			//输出到控制台窗口
			/*INLINED*/friend std::ostream &MY_LIB operator<<(
				std::ostream &out,
				const LargeUnsigned &l
				) noexcept {
				return LargeUnsigned::_Print<decltype(l.cbegin()), decltype(l.cend()), l.getRadix()>(l.cbegin(), l.cend(), out);
			}

			template<typename Int>
			/*INLINED*/ auto &MY_LIB operator*=(const Int &that) noexcept {
				static_assert(std::is_integral_v<Int>);
				auto it = LayerRange(that);
				this->range_mul(it);
				return *this;
			}
			template<typename Int>
			/*INLINED*/LargeUnsigned MY_LIB operator*(const Int &that) const noexcept {
				static_assert(std::is_integral_v<Int>);
				LargeUnsigned res = Copy(*this), temp(that);
				res *= temp;
				//temp.release();
				return res;
			}
			constexpr void MY_LIB Swap(LargeUnsigned &that)noexcept {
				this->LL::swap(*static_cast<LL *>(&that));
			}
			template<typename Begin, typename End>
			static constexpr auto MY_LIB Simplify(LargeUnsigned *p, Begin begin, End end)noexcept {
				auto Flag = begin;
				if (begin != end) {
					auto OprtPtr = begin;
					while (OprtPtr != end) {
						if (*OprtPtr != Data(0U)) {
							Flag = OprtPtr;
						}
						++OprtPtr;
					}
					p->erase_after(Flag, end);
				}
			#ifndef NDEBUG
				bool flag = true;
				for (const auto &i : p->crange()) {
					flag = (i != 0);
				}
				assert(flag);
			#endif // !NDEBUG
				return Flag;
			}
			constexpr auto MY_LIB Simplify()noexcept {
				return Simplify(this, this->cbegin(), this->cend());
			}
			/*INLINED*/void MY_LIB operator*=(const LargeUnsigned &b) noexcept {
				this->range_mul(b.range());
			}
			//重载
			/*INLINED*/LargeUnsigned MY_LIB operator*(const LargeUnsigned &b)const noexcept {
				LargeUnsigned &&Res = Copy(*this);
				Res *= b;
				return std::move(Res);
			}
			//重载
			INLINED void MY_LIB operator-=(const LargeUnsigned &that) noexcept {
				if (that == 0) {
					return;
				}
				if (*this == 0) {
					*this = Copy(that);
					return;
				}
				assert(*this >= that);
				range_sub(that.crange());
				this->Simplify();
			}
			//重载LinkedList链表负号
			INLINED LargeUnsigned MY_LIB operator-(
				)const noexcept {
				LargeUnsigned res = Copy(*this);
				return res;
			}
			//重载LinkedList链表+=
			INLINED void MY_LIB operator+=(const LargeUnsigned &that) noexcept {
				if (that == 0) {
					return;
				}
				if (*this == 0) {
					*this = Copy(that);
					return;
				}
				range_add(that.crange());
			}
			INLINED LargeUnsigned MY_LIB operator+(
				const LargeUnsigned &b//操作数
				)  const noexcept {
				LargeUnsigned Result = Copy(*this);//存储结果
				Result += b;
				return Result;
			}
			/*INLINED*/LargeUnsigned MY_LIB operator-(
				const LargeUnsigned &b//操作数
				)const noexcept {
				LargeUnsigned Result = Copy(*this);//存储结果
				Result -= b;
				return Result;
			}
			void MY_LIB operator++() {
				*this += 1;
			}
			void MY_LIB operator--() {
				*this -= 1;
			}


			template<typename Int>
			INLINED auto &MY_LIB operator+=(const Int &that)noexcept {
				static_assert(std::is_integral_v<Int>, "integral type required.");
				if (that == 0) return *this;
				if (*this == 0) {
					*this = that;
					return *this;
				}
				auto it = LayerRange(that);
				range_add(it);
				return *this;
			}
			template<typename Int>
			INLINED auto &MY_LIB operator-=(const Int &that)noexcept {
				static_assert(std::is_integral_v<Int>, "integral type required.");
				if (that == 0) 	return *this;
				auto it = LayerRange(that);
				range_sub(it);
				return *this;
			}
			template<typename Int>
			INLINED LargeUnsigned MY_LIB operator+(const Int &that)const noexcept {
				static_assert(std::is_integral_v<Int>, "integral type required.");
				LargeUnsigned res = Copy(*this);
				res += that;
				return res;
			}
			template<typename Int>
			INLINED LargeUnsigned MY_LIB operator-(const Int &that)const noexcept {
				static_assert(std::is_integral_v<Int>, "integral type required.");
				LargeUnsigned res = Copy(*this);
				res -= that;
				return res;
			}



			//位移运算
			//按独立进制而非二进制
			//左移时用默认值补齐
			/*INLINED*/LargeUnsigned &operator<<=(
				unsigned int bits) noexcept {
				if (*this != 0) {
					for (unsigned int i = 0; i < bits; i++) {
						this->LL::insert_after(this->before_begin(), 0);
					}
				}
				return *this;
			}
			//位移运算
			//按独立进制而非二进制
			//右移时第一位销毁
			/*INLINED*/LargeUnsigned &operator>>=(unsigned int bits) noexcept {
				this->LL::operator>>=(bits);
				return *this;
			}



			bool MY_LIB operator==(const LargeUnsigned &that)const noexcept {
				return range_equal(that.crange());
			}
			bool MY_LIB operator!=(const LargeUnsigned &that)const noexcept {
				return !range_equal(that.crange());
			}
			template<typename Int>
			bool MY_LIB operator==(const Int &that)const noexcept {
				static_assert(std::is_integral_v<Int>, "integral type required.");
				if (that == Int(0)) {
					for (auto i : *this) {
						if (i != 0) {
							return false;
						}
					}
					return true;
				}
				auto it = LayerRange(that);
				return range_equal(it);
			}
			template<typename Int>
			bool MY_LIB operator!=(const Int &that)const noexcept {
				static_assert(std::is_integral_v<Int>, "integral type required.");
				return !(*this == that);
			}
			bool MY_LIB operator<(const LargeUnsigned &that)const noexcept {
				return range_smaller(that.crange());
			}
			bool MY_LIB operator>(const LargeUnsigned &that)const noexcept {
				return range_larger(that.crange());
			}
			bool MY_LIB operator<=(const LargeUnsigned &that)const noexcept {
				return !(*this > that);
			}
			bool MY_LIB operator>=(const LargeUnsigned &that)const noexcept {
				return !(*this < that);
			}
			template<typename Int>
			bool MY_LIB operator<(const Int &that)const noexcept {
				static_assert(std::is_integral_v<Int>, "integral type required.");
				auto it = LayerRange(that);
				return range_smaller(it);
			}
			template<typename Int>
			bool MY_LIB operator>(const Int &that)const noexcept {
				static_assert(std::is_integral_v<Int>, "integral type required.");
				auto it = LayerRange(that);
				return range_larger(it);
			}
			template<typename Int>
			bool MY_LIB operator<=(const Int &that)const noexcept {
				static_assert(std::is_integral_v<Int>, "integral type required.");
				LargeUnsigned T(that);
				return (*this <= T);
			}
			template<typename Int>
			bool MY_LIB operator>=(const Int &that)const noexcept {
				static_assert(std::is_integral_v<Int>, "integral type required.");
				LargeUnsigned T = LargeUnsigned(that);
				return (*this >= T);
			}

			class Sim {
				LargeUnsigned *const p;
			public:
				template<typename Iter1, typename Iter2>
				void MY_LIB operator()(Iter1 begin, Iter2 end)noexcept {
					Simplify(p, begin, end);
				}
				template<typename Range>
				void MY_LIB operator()(Range range)noexcept {
					Simplify(p, range.begin(), range.end());
				}
				MY_LIB Sim(LargeUnsigned *p) :p(p) { }

				MY_LIB ~Sim() = default;
			};
			void MY_LIB operator%=(const LargeUnsigned &that)noexcept {
				assert(that != 0);
				if (that == 0) {
					return;
				}
				range_div(that.crange());
			}
			void MY_LIB operator/=(const LargeUnsigned &that)noexcept {
				assert(that != 0);
				if (that == 0) {
					return;
				}
				LargeUnsigned Res(0);
				range_div(Res, that.crange());
				*this = std::move(Res);
			}
			LargeUnsigned MY_LIB Divide(const LargeUnsigned &that)noexcept {
				assert(that != 0);
				LargeUnsigned Res(0);
				range_div(Res, that.crange());
				return Res;
			}
			template<typename Int>
			auto &MY_LIB operator%=(const Int &that)noexcept {
				static_assert(std::is_integral_v<Int>, "integral type required.");
				auto it = LayerRange(that);
				range_div(it);
				return *this;
			}
			template<typename Int>
			auto &MY_LIB operator/=(const Int &that)noexcept {
				static_assert(std::is_integral_v<Int>, "integral type required.");
				auto it = LayerRange(that);
				LargeUnsigned Res(0);
				range_div(Res, it);
				*this = std::move(Res);
				return *this;
			}
			template<typename Int>
			LargeUnsigned MY_LIB Divide(const Int &that)noexcept {
				static_assert(std::is_integral_v<Int>, "integral type required.");
				auto it = LayerRange(that);
				LargeUnsigned Res(0);
				range_div(Res, it);
				return Res;
			}


			//覆盖赋值
			template<typename Int>
			/*INLINED*/LargeUnsigned &MY_LIB operator=(
				Int Val
				) noexcept {
				if (Val == 0) {
					this->clear();
				}
				auto it = Layer(Val);
				auto index = this->before_begin();
				for (; it != nullptr; ) {
					index = this->LL::emplace_after(index, *it);
					*index = *it;
					++it;
				}
				this->LL::erase_after(index, this->cend());
				return *this;
			}


			//获取存储的值
			//可能溢出
			template<typename Val = __int64>
			/*INLINED*/Val MY_LIB GetValue()const noexcept {
				Val value = 0;
				size_t n = 0;
				auto OprtPtr = this->cbegin();
				if (OprtPtr == this->cend()) {
					return 0;
				}
				while (true) {
					value += (static_cast<Val>(*OprtPtr)) *Math::Power(static_cast<Val>(radix), n);
					++OprtPtr;
					if (OprtPtr != this->cend()) {
						++n;
					}
					else  break;
				}
				return (value);
			}
			const auto &MY_LIB GetThis()const noexcept {
				return this->data;
			}
			static LargeUnsigned MY_LIB pow(size_t _base, size_t expo) noexcept {
				if (expo == 0) return LargeUnsigned(1);
				LargeUnsigned res(_base);
				for (size_t i = 1; i < expo; i++) {
					res *= _base;
				}
				return res;
			}
			~LargeUnsigned() noexcept = default;
		};
		template<typename LL, typename LL::value_type radix>
		class LargeSigned :protected LargeUnsigned<LL, radix> {
			//friend class Q;
		public:
			using radix_t = decltype(radix);
			using Data = radix_t;
			using super = LargeUnsigned<LL, radix>;
			static constexpr radix_t getRadix()noexcept { return radix; }
			constexpr INLINED auto begin() noexcept {
				return this->LargeUnsigned<LL, radix>::begin();
			}
			constexpr INLINED auto begin()const noexcept {
				return this->LargeUnsigned<LL, radix>::begin();
			}
			constexpr INLINED auto cbegin()const noexcept {
				return this->LargeUnsigned<LL, radix>::cbegin();
			}
			constexpr INLINED auto end() noexcept {
				return this->LargeUnsigned<LL, radix>::end();
			}
			constexpr INLINED auto end() const noexcept {
				return this->LargeUnsigned<LL, radix>::end();
			}
			constexpr INLINED auto cend() const noexcept {
				return this->LargeUnsigned<LL, radix>::cend();
			}
			explicit MY_LIB LargeSigned(bool sign, LL &&ll)noexcept :PosSign(sign), super(std::move(ll)) { }
			template<typename val> explicit MY_LIB LargeSigned(val Val)noexcept
				:PosSign(Val >= 0), LargeUnsigned<LL, radix>(Math::ABS(Val)) { }
			template<typename val> explicit MY_LIB LargeSigned(bool Pos, val Val)noexcept
				:PosSign(Pos), LargeUnsigned<LL, radix>(Val) {
				assert(Val >= 0);
			}
			explicit MY_LIB LargeSigned(const char *str)noexcept
				:PosSign(*str != '-'), super(std::move(super::MakeFromString(str))) { }
			explicit MY_LIB LargeSigned(const char *begin, const char *end)noexcept
				:PosSign(*begin != '-'), super(std::move(super::MakeFromString(begin, end))) { }
			explicit MY_LIB LargeSigned(bool sign, LargeUnsigned<LL, radix> uns)noexcept
				:PosSign(sign), LargeUnsigned<LL, radix>(uns) { }
			const super &operator()()const noexcept {
				return static_cast<const super &>(*this);
			}
			static constexpr LargeSigned MY_LIB Copy(const LargeSigned &that)noexcept {
				LargeSigned This(that.PosSign, LargeUnsigned<LL, radix>::Copy(that));
				return This;
			}
			constexpr void MY_LIB _Swap(LargeSigned &that)noexcept {
				this->LargeUnsigned<LL, radix>::Swap(that);
			}
			constexpr void MY_LIB Swap(LargeSigned &that)noexcept {
				this->_Swap(that);
				bool temp = that.PosSign;
				that.PosSign = this->PosSign;
				this->PosSign = temp;
			}
			constexpr void MY_LIB destruct() noexcept {
				this->LL::destruct();
			}

			bool MY_LIB operator==(const LargeSigned &that)const noexcept {
				return (this->LargeUnsigned<LL, radix>::operator==(*static_cast<const LargeUnsigned<LL, radix> *>(&that)) && (this->PosSign == that.PosSign));
			}
			bool MY_LIB operator!=(const LargeSigned &that)const noexcept {
				return !(*this == that);
			}
			bool MY_LIB operator<(const LargeSigned &that)const noexcept {
				if ((!this->PosSign) && that.PosSign) {
					return true;
				}
				if (this->PosSign && !(that.PosSign)) {
					return false;
				}
				if (this->PosSign && that.PosSign) {
					return this->LargeUnsigned<LL, radix>::operator<(*static_cast<const LargeUnsigned<LL, radix> *>(&that));
				}
				else {
					assert((!this->PosSign) && (!that.PosSign));
					return this->LargeUnsigned<LL, radix>::operator>=(*static_cast<const LargeUnsigned<LL, radix> *>(&that));
				}
			}
			bool MY_LIB operator>(const LargeSigned &that)const noexcept {
				if ((!this->PosSign) && that.PosSign) {
					return false;
				}
				if (this->PosSign && !(that.PosSign)) {
					return true;
				}
				if (this->PosSign && that.PosSign) {
					return this->LargeUnsigned<LL, radix>::operator>(*static_cast<const LargeUnsigned<LL, radix> *>(&that));
				}
				else {
					assert((!this->PosSign) && (!that.PosSign));
					return this->LargeUnsigned<LL, radix>::operator<=(*static_cast<const LargeUnsigned<LL, radix> *>(&that));
				}
			}
			bool MY_LIB operator<=(const LargeSigned &that)const noexcept {
				return !(*this > that);
			}
			bool MY_LIB operator>=(const LargeSigned &that)const noexcept {
				return !(*this < that);
			}
			template<typename Int>
			bool MY_LIB operator==(const Int &that)const noexcept {
				static_assert(std::is_integral_v<Int>);
				return ((that == 0 || (this->PosSign == (that > 0))) && this->LargeUnsigned<LL, radix>::operator==(that));
			}
			template<typename Int>
			bool MY_LIB operator!=(const Int &that)const noexcept {
				static_assert(std::is_integral_v<Int>);
				return !(*this == that);
			}
			template<typename Int>
			bool MY_LIB operator>(const Int &that)const noexcept {
				static_assert(std::is_integral_v<Int>);
				return (this->LargeUnsigned<LL, radix>::operator>(that));
			}
			template<typename Int>
			bool MY_LIB operator<(const Int &that)const noexcept {
				static_assert(std::is_integral_v<Int>);
				return (this->LargeUnsigned<LL, radix>::operator<(that));
			}
			template<typename Int>
			bool MY_LIB operator>=(const Int &that)const noexcept {
				static_assert(std::is_integral_v<Int>);
				return (this->LargeUnsigned<LL, radix>::operator>=(that));
			}
			template<typename Int>
			bool MY_LIB operator<=(const Int &that)const noexcept {
				static_assert(std::is_integral_v<Int>);
				return (this->LargeUnsigned<LL, radix>::operator<=(that));
			}

			template<typename Val>
			LargeSigned &MY_LIB operator=(Val value) noexcept {
				if (value >= 0) {
					this->PosSign = true;
				}
				else {
					this->PosSign = false;
					value = -value;
				}
				this->LargeUnsigned<LL, radix>::operator=(value);
				return *this;
			}
			LargeSigned &MY_LIB operator+=(const LargeSigned &that) noexcept {
				if ((this->PosSign && that.PosSign) || (!this->PosSign && !that.PosSign)) {
					LargeUnsigned<LL, radix>::operator+=(*static_cast<const LargeUnsigned<LL, radix> *>(&that));
				}
				else {
					LargeInteger::Compare Cmpr = LargeInteger::LongCmpt<LLCmptTraits<radix>>::CompareTo(this->begin(), that.begin());
					if (Cmpr == LargeInteger::Compare::Equal) {
						this->destruct();
					}
					if (Cmpr == LargeInteger::Compare::Larger) {
						this->LargeUnsigned<LL, radix>::operator-=(*static_cast<const LargeUnsigned<LL, radix> *>(&that));
					}
					else {
						LargeSigned temp = Copy(that);
						temp.LargeUnsigned<LL, radix>::operator-=(*static_cast<const LargeUnsigned<LL, radix> *>(this));
						*this = temp;
						//this->PosSign = !this->PosSign;
					}
				}
				return *this;
			}
			LargeSigned MY_LIB operator+(const LargeSigned &that) const noexcept {
				LargeSigned temp = Copy(*this);
				temp += that;
				return temp;
			}
			LargeSigned &MY_LIB operator-=(const LargeSigned &that) noexcept {
				if ((this->PosSign && !that.PosSign) || (!this->PosSign && that.PosSign)) {
					LargeUnsigned<LL, radix>::operator+=(*static_cast<const LargeUnsigned<LL, radix> *>(&that));
				}
				else {
					LargeInteger::Compare Cmpr = LargeInteger::LongCmpt<LLCmptTraits<radix>>::CompareTo(this->begin(), that.begin());
					if (Cmpr == LargeInteger::Compare::Equal) {
						this->destruct();
					}
					if (Cmpr == LargeInteger::Compare::Larger) {
						LargeInteger::LongCmpt<LLCmptTraits<radix>>::SubtractFrom(that.begin(), this->begin());
					}
					else {
						LargeSigned temp = Copy(that);
						temp.LargeUnsigned<LL, radix>::operator-=(*static_cast<const LargeUnsigned<LL, radix> *>(this));
						*this = temp;
						this->PosSign = !this->PosSign;
					}
				}
				return *this;
			}
			LargeSigned MY_LIB operator-(const LargeSigned &that) const noexcept {
				LargeSigned temp = Copy(*this);
				temp -= that;
				return temp;
			}
			template<typename Int>
			LargeSigned &MY_LIB operator+=(const Int &that) noexcept {
				static_assert(std::is_integral_v<Int>);
				LargeSigned That(that);
				*this += That;
				That.destruct();
				return *this;
			}
			template<typename Int>
			LargeSigned MY_LIB operator+(const Int &that) const noexcept {
				static_assert(std::is_integral_v<Int>);
				LargeSigned temp = Copy(*this);
				temp += that;
				return temp;
			}
			template<typename Int>
			LargeSigned &MY_LIB operator-=(const Int &that) noexcept {
				static_assert(std::is_integral_v<Int>);
				LargeSigned That(that);
				*this -= That;
				That.destruct();
				return *this;
			}
			template<typename Int>
			LargeSigned MY_LIB operator-(const Int &that) const noexcept {
				static_assert(std::is_integral_v<Int>);
				LargeSigned temp = Copy(*this);
				temp -= that;
				return temp;
			}
			LargeSigned &MY_LIB operator*=(const LargeSigned &that) noexcept {
				if (!that.PosSign) {
					this->PosSign = !this->PosSign;
				}
				this->LargeUnsigned<LL, radix>::operator*=(*static_cast<const LargeUnsigned<LL, radix> *>(&that));
				return *this;
			}
			LargeSigned MY_LIB operator*(const LargeSigned &that) const noexcept {
				LargeSigned temp = Copy(*this);
				temp *= that;
				return temp;
			}
			template<typename Int>
			LargeSigned &MY_LIB operator*=(const Int &that) noexcept {
				static_assert(std::is_integral_v<Int>);
				LargeSigned That(that);
				*this *= That;
				That.destruct();
				return *this;
			}
			template<typename Int>
			LargeSigned MY_LIB operator*(const Int &that) const noexcept {
				static_assert(std::is_integral_v<Int>);
				LargeSigned temp = Copy(*this);
				temp *= that;
				return temp;
			}
			LargeSigned &MY_LIB operator%=(const LargeSigned &that) noexcept {
				if (!that.PosSign) {
					this->PosSign = !this->PosSign;
				}
				this->LargeUnsigned<LL, radix>::operator%=(*static_cast<const LargeUnsigned<LL, radix> *>(&that));
				return *this;
			}
			LargeSigned MY_LIB operator%(const LargeSigned &that) const noexcept {
				LargeSigned temp = Copy(*this);
				temp %= that;
				return temp;
			}
			LargeSigned &MY_LIB operator/=(const LargeSigned &that) noexcept {
				if (!that.PosSign) {
					this->PosSign = !this->PosSign;
				}
				this->LargeUnsigned<LL, radix>::operator/=(*static_cast<const LargeUnsigned<LL, radix> *>(&that));
				return *this;
			}
			LargeSigned MY_LIB operator/(const LargeSigned &that) const noexcept {
				LargeSigned temp = Copy(*this);
				temp /= that;
				return temp;
			}
			template<typename Int>
			LargeSigned &MY_LIB operator%=(const Int &that) noexcept {
				static_assert(std::is_integral_v<Int>);
				LargeSigned That(that);
				*this %= That;
				That.destruct();
				return *this;
			}
			template<typename Int>
			LargeSigned MY_LIB operator%(const Int &that) const noexcept {
				static_assert(std::is_integral_v<Int>);
				LargeSigned temp = Copy(*this);
				temp %= that;
				return temp;
			}
			template<typename Int>
			LargeSigned &MY_LIB operator/=(const Int &that) noexcept {
				static_assert(std::is_integral_v<Int>);
				LargeSigned That(that);
				*this /= That;
				That.destruct();
				return *this;
			}
			template<typename Int>
			LargeSigned MY_LIB operator/(const Int &that) const noexcept {
				static_assert(std::is_integral_v<Int>);
				LargeSigned temp = Copy(*this);
				temp /= that;
				return temp;
			}
			void MY_LIB SetToContradict() noexcept {
				this->PosSign = !this->PosSign;
			}

			//获取存储的值
			//可能溢出
			template<typename val = __int64>
			/*INLINED*/val MY_LIB GetValue()const noexcept {
				val &&value = LargeUnsigned<LL, radix>::template GetValue<val>();
				if (!PosSign) {
					value = -value;
				}
				return value;
			}

			const auto &MY_LIB GetThis()const noexcept {
				return this->data;
			}

			//二进制输出到控制台窗口
			/*INLINED*/friend std::ostream &MY_LIB operator<<(
				std::ostream &out,
				const LargeSigned &l
				) noexcept {
				return l.Print(out);
			}

			//从控制台输入
			INLINED friend std::istream &MY_LIB operator>>(
				std::istream &in,
				LargeSigned &l
				) noexcept {
				return l.Scan(in);
			}
			template<char...Delim>char __stdcall get_until(std::istream &in)noexcept {
				this->PosSign = (in.peek() == '-' ? false : true);
				return this->LargeUnsigned<LL, radix>::template get_until<Delim...>(in);
			}
			INLINED std::istream &MY_LIB Scan(std::istream &in) {
				this->PosSign = (in.peek() == '-' ? false : true);
				return in >> *static_cast<LargeUnsigned<LL, radix> *>(this);
			}
			INLINED std::ostream &MY_LIB Print(std::ostream &o = std::cout) const noexcept {
				if (!this->PosSign) {
					o << "-";
				}
				return LargeUnsigned<LL, radix>::template _Print<decltype(this->begin()), radix>(this->begin(), o);
			}

			~LargeSigned() { }

		private:
			bool PosSign;
		};

	}
}