#pragma once

#include "LinkedList.h"
#include "LargeInteger.h"
#include "OLL.h"
#include "DLL.h"
#include "list.h"
#include <iomanip>
#include <sstream>
#include <string>
constexpr unsigned int Z_MAX = 1000000000U;

namespace Darkness {
	inline namespace LargeInteger {

		typedef LargeSigned<LL::OAL<unsigned int, 64U / sizeof(void *) - 1>, Z_MAX> Z;
		typedef LargeUnsigned<LL::OAL<unsigned int, 64U / sizeof(void *) - 1 >, Z_MAX> N;

		//非负有理数
		//Non-negative Rational Number
		//分子分母上的负号将被忽略
		//negative sign on numerator and denominator will be ignored
		class uQ {
		protected:
			using Data = unsigned int;
			N Numerator;//分子
			N Denominator;//分母
			size_t DecimalLength(const char *str)noexcept {
				const char *pDot = nullptr;
				for (; *str != '\0'; ++str) {
					if (*str == '.')pDot = str;
				}
				return (pDot == nullptr ? 0 : str - pDot - 1ULL);
			}
			class DecimalMaker {
			public:
				DecimalMaker() = default;
				~DecimalMaker()noexcept = default;
				N first(const char *str, const char *const end = nullptr) noexcept {
					assert(is_numerator);
					is_numerator = false;//怕忘了改
					for (; *str == '0' && (end == nullptr || str <= end); ++str);
					const char *p = str;
					for (; *p != '\0' && (end == nullptr ||p <= end); ++p) {
						if (*p == '\0') {
							is_decimal = true;
							info.len = 0;
							break;
						}
						else if (*p == '.') {
							is_decimal = true;
						}
						else if (*p == '/') {
							is_decimal = false;
							info.begin = p + 1;
							while (!('0' <= *info.begin && *info.begin <= '9')) {
								++info.begin;
							}
							break;
						}
						else if (is_decimal) {
							++info.len;
						}
					}
					return std::move(str == p ? N(0) : N::MakeFromString(str, p - 1));
				}
				N second(const char *end = nullptr) noexcept {
					assert(!is_numerator);
					return std::move(
						is_decimal ?
						N::pow(10, info.len)
						:
						(info.begin) ? N::MakeFromString(info.begin, end) : N(1)
					);
				}
			private:
				bool is_numerator = true;
				bool is_decimal = false;//是小数
				union denominator_info {
					const char *begin = nullptr;
					size_t len;
				}info;
			};

		public:
			explicit MY_LIB uQ(const char *str, DecimalMaker maker = DecimalMaker())noexcept
				:Numerator(std::move(maker.first(str))), Denominator(std::move(maker.second())) {
				this->Simplify();
			}
			explicit MY_LIB uQ(const char *begin, const char *end, DecimalMaker maker = DecimalMaker())noexcept
				:Numerator(std::move(maker.first(begin, end))), Denominator(std::move(maker.second())) {
				this->Simplify();
			}
			template<typename Val1, typename Val2>
			explicit MY_LIB uQ(const Val1 &&n, const Val2 &&d = 1)noexcept :
				Numerator(n),
				Denominator(d) {
				this->Simplify();
			}
			MY_LIB uQ(const uQ &that) noexcept :
				Numerator(that.Numerator),
				Denominator(that.Denominator) { }
			explicit MY_LIB uQ(const N &Numerator, const N &Denominator) noexcept :
				Numerator(Numerator),
				Denominator(Denominator) {
				this->Simplify();
			}
			INLINED MY_LIB ~uQ() noexcept = default;
			static uQ MY_LIB Copy(const uQ &that)noexcept {
				return uQ(N::Copy(that.Numerator), N::Copy(that.Denominator));
			}
			uQ &MY_LIB toReciprocal()noexcept {
				this->Numerator.Swap(this->Denominator);
				return *this;
			}
			template<typename val = double> val MY_LIB estim()const noexcept {
				static_assert(std::is_arithmetic_v<val> && !std::is_integral_v<val>);
				return (this->Numerator.GetValue<val>() / this->Denominator.GetValue<val>());
			}
			INLINED void MY_LIB destruct() noexcept {
				this->Denominator.destruct();
				this->Numerator.destruct();
			}
			//约分
			void MY_LIB Simplify() noexcept {
				if (Numerator == Denominator) {
					Numerator = 1;
					Denominator = 1;
				}
				else if (Numerator == 0) {
					Denominator = 1;
				}
				else if (Denominator == 0) {
					MY_ASSERT(false, 0 / 0);
					Numerator = 1;
				}
				else {
					N a = N::Copy(Numerator), b = N::Copy(Denominator);
					N *Larger, *Smaller;
					(a > b) ? (Larger = &a, Smaller = &b) : (Larger = &b, Smaller = &a);
					while (*Smaller != 0) {
						*Larger %= *Smaller;
						std::swap(Larger, Smaller);
					}
					this->Numerator /= *Larger;
					this->Denominator /= *Larger;
				}
			}
			void MY_LIB operator=(long that) noexcept {
				this->Denominator = 1;
				this->Numerator = that;
			}
			template<typename Int>
			void MY_LIB operator+=(Int that) noexcept {
				N Product(this->Denominator * that);
				this->Numerator += Product;
				Product.destruct();
				this->Simplify();
			}
			template<typename Int>
			void MY_LIB operator-=(Int that) noexcept {
				N Product = this->Denominator * that;
				this->Numerator -= Product;
				Product.destruct();
				this->Simplify();
			}
			uQ MY_LIB operator-(const uQ &that)const noexcept {
				uQ Res = Copy(*this);
				Res -= that;
				return Res;
			}

			void MY_LIB operator+=(const N &that) noexcept {
				this->Numerator += that * this->Denominator;
				this->Simplify();
			}
			void MY_LIB operator-=(const N &that) noexcept {
				this->Numerator -= that * this->Denominator;
				this->Simplify();
			}
			void MY_LIB operator*=(const N &that) noexcept {
				this->Numerator *= that;
				this->Simplify();
			}
			void MY_LIB operator/=(const N &that) noexcept {
				this->Denominator *= that;
				this->Simplify();
			}

			void MY_LIB operator+=(const uQ &that) noexcept {
				this->Numerator *= that.Denominator;
				N &&tmp = that.Numerator * this->Denominator;
				this->Numerator += tmp;
				tmp.destruct();
				this->Denominator *= that.Denominator;
				this->Simplify();
			}
			void MY_LIB operator-=(const uQ &that) noexcept {
				this->Numerator *= that.Denominator;
				N &&tmp = that.Numerator * this->Denominator;
				assert(this->Numerator >= tmp);
				this->Numerator -= tmp;
				tmp.destruct();
				this->Denominator *= that.Denominator;
				this->Simplify();
			}
			uQ MY_LIB operator+(const uQ &that)const noexcept {
				uQ Res = uQ::Copy(*this);
				Res += that;
				return Res;
			}
			void MY_LIB operator*=(const uQ &that) noexcept {
				this->Denominator *= that.Denominator;
				this->Numerator *= that.Numerator;
				this->Simplify();
			}
			void MY_LIB operator/=(const uQ &that) noexcept {
				this->Denominator *= that.Numerator;
				this->Numerator *= that.Denominator;
				this->Simplify();
			}
			uQ MY_LIB operator*(const uQ &that)const noexcept {
				uQ Res = uQ::Copy(*this);
				Res *= that;
				return Res;
			}
			uQ MY_LIB operator/(const uQ &that)const noexcept {
				uQ Res = uQ::Copy(*this);
				Res /= that;
				return Res;
			}
			template<typename Int>
			bool MY_LIB operator==(Int that)const noexcept {
				N temp = this->Denominator * that;
				bool ret = (temp == this->Numerator);
				temp.destruct();
				return ret;
			}
			bool MY_LIB operator==(const uQ &that)const noexcept {
				return ((this->Numerator == that.Numerator) && (this->Denominator == that.Denominator));
			}
			bool MY_LIB operator>(const uQ &that)const noexcept {
				N &&temp1 = this->Numerator * that.Denominator, &&temp2 = this->Denominator * that.Numerator;
				bool &&res = (temp1 > temp2);
				temp1.destruct();
				temp2.destruct();
				return res;
			}
			bool MY_LIB operator<(const uQ &that)const noexcept {
				N &&temp1 = this->Numerator * that.Denominator, &&temp2 = this->Denominator * that.Numerator;
				bool &&res = (temp1 < temp2);
				temp1.destruct();
				temp2.destruct();
				return res;
			}
			bool MY_LIB operator<=(const uQ &that)const noexcept { return !(*this > that); }
			bool MY_LIB operator>=(const uQ &that)const noexcept { return !(*this < that); }
			std::ostream &Print(std::ostream &o)const noexcept {
				if (this->Denominator == 1) return Numerator.Print(o);
				return Denominator.Print(Numerator.Print(o) << '/');
			}
			friend std::ostream &operator<<(std::ostream &o, const uQ &q) noexcept {
				std::stringstream sout;
				q.Print(sout);
				std::string tmp(size_t((o.width() > sout.str().size()) ? o.width() - sout.str().size() : 0), o.fill());
				sout >> tmp;
				return o << tmp;
			};
		};


		//有理数
		//Rational Number
		//分子分母上的负号将被忽略
		//negative sign on numerator and denominator will be ignored
		class Q :protected uQ {
		protected:
			using Data = N::Data;
			bool PosSign;
			using super = uQ;
		public:
			explicit MY_LIB Q(const char *str)noexcept
				: PosSign(*str != '-'), super(*str != '-' ? str : str + 1) { }
			explicit MY_LIB Q(const char *begin, const char *end)noexcept
				: PosSign(*begin != '-'), super(*begin != '-' ? begin : begin + 1, end) { }
			template<typename Val1, typename Val2>explicit MY_LIB Q(bool sign, const Val1 &&n, const Val2 &&d = 1)noexcept :
				PosSign(sign), super(std::move(n), std::move(d)) {
				this->checkSign();
			}
			MY_LIB Q(const Q &that) noexcept :
				PosSign(that.PosSign), super(that.Numerator, that.Denominator) { }
			explicit MY_LIB Q(bool sign, const N &Numerator, const N &Denominator) noexcept :
				PosSign(sign), super(Numerator, Denominator) {
				this->checkSign();
			}
			INLINED MY_LIB ~Q() noexcept = default;
			MY_LIB Q(Q &&that)noexcept :PosSign(that.PosSign), super(static_cast<uQ &&>(that)) { }
			static Q MY_LIB Copy(const Q &that)noexcept {
				return Q(that.PosSign, N::Copy(that.Numerator), N::Copy(that.Denominator));
			}
			Q &MY_LIB toReciprocal()noexcept {
				this->uQ::toReciprocal();
				return *this;
			}
			template<typename val = double> val MY_LIB estim()const noexcept {
				static_assert(std::is_arithmetic_v<val> && !std::is_integral_v<val>);
				return (this->PosSign ?
					(this->Numerator.GetValue<val>() / (this->Denominator.GetValue<val>()))
					:
					-(this->Numerator.GetValue<val>() / (this->Denominator.GetValue<val>()))
					);
			}
			INLINED void MY_LIB destruct() noexcept {
				this->Denominator.destruct();
				this->Numerator.destruct();
			}
			void MY_LIB checkSign()noexcept {
				if (this->Numerator == 0) this->PosSign = true;
			}
			//约分
			void MY_LIB Simplify() noexcept {
				this->uQ::Simplify();
				this->checkSign();
			}
			bool MY_LIB isInteger()const noexcept { 
				return this->Denominator == 1;
			}
			bool MY_LIB	isPositive()const noexcept {
				return this->PosSign;
			}
			Q &MY_LIB operator=(long that) noexcept {
				this->PosSign = that >= 0;
				this->Denominator = 1;
				this->Numerator = that;
				return *this;
			}
			Q &MY_LIB operator=(const Q &that) noexcept {
				this->PosSign = that.PosSign;
				this->super::operator=(static_cast<const super &>(that));
				return *this;
			}
			template<typename Int>
			void MY_LIB operator+=(Int that) noexcept {
				N Product(this->Denominator * that);
				this->Numerator += Product;
				Product.destruct();
				this->Simplify();
			}
			template<typename Int>
			void MY_LIB operator-=(Int that) noexcept {
				N Product = this->Denominator * that;
				this->Numerator -= Product;
				Product.destruct();
				this->Simplify();
			}
			Q MY_LIB operator-(const Q &that)const noexcept {
				Q Res = Copy(*this);
				Res -= that;
				return Res;
			}

			void MY_LIB operator+=(const N &that) noexcept {
				this->Numerator += that * this->Denominator;
				this->Simplify();
			}
			void MY_LIB operator-=(const N &that) noexcept {
				this->Numerator -= that * this->Denominator;
				this->Simplify();
			}
			void MY_LIB operator*=(const N &that) noexcept {
				this->Numerator *= that;
				this->Simplify();
			}
			void MY_LIB operator/=(const N &that) noexcept {
				this->Denominator *= that;
				this->Simplify();
			}

			void MY_LIB operator+=(const Q &that) noexcept {
				this->Numerator *= that.Denominator;
				N &&tmp = that.Numerator * this->Denominator;
				if (that.PosSign ^ this->PosSign) {
					if (this->Numerator > tmp) {
						this->Numerator -= tmp;
					}
					else {
						N &&res = tmp - this->Numerator;
						this->Numerator.Swap(res);
						res.destruct();
						this->PosSign = !this->PosSign;
					}
				}
				else {
					this->Numerator += tmp;
				}
				tmp.destruct();
				this->Denominator *= that.Denominator;
				this->Simplify();
			}
			void MY_LIB operator-=(const Q &that) noexcept {
				this->Numerator *= that.Denominator;
				N &&tmp = that.Numerator * this->Denominator;
				if (that.PosSign ^ this->PosSign) {
					this->Numerator += tmp;
				}
				else {
					if (this->Numerator >= tmp) {
						this->Numerator -= tmp;
					}
					else {
						this->PosSign = !this->PosSign;
						N &&res = tmp - this->Numerator;
						this->Numerator.Swap(res);
						res.destruct();
					}
				}
				tmp.destruct();
				this->Denominator *= that.Denominator;
				this->Simplify();
			}
			Q MY_LIB operator+(const Q &that)const noexcept {
				Q Res = Q::Copy(*this);
				Res += that;
				return Res;
			}
			void MY_LIB operator*=(const Q &that) noexcept {
				this->PosSign = !(that.PosSign ^ this->PosSign);
				this->Denominator *= that.Denominator;
				this->Numerator *= that.Numerator;
				this->Simplify();
			}
			void MY_LIB operator/=(const Q &that) noexcept {
				this->PosSign = !(that.PosSign ^ this->PosSign);
				this->Denominator *= that.Numerator;
				this->Numerator *= that.Denominator;
				this->Simplify();
			}
			Q MY_LIB operator*(const Q &that)const noexcept {
				Q Res = Q::Copy(*this);
				Res *= that;
				return Res;
			}
			Q MY_LIB operator/(const Q &that)const noexcept {
				Q Res = Q::Copy(*this);
				Res /= that;
				return Res;
			}
			template<typename Int>
			bool MY_LIB operator==(Int that)const noexcept {
				N temp = this->Denominator * that;
				bool ret = (temp == this->Numerator);
				temp.destruct();
				return ret;
			}
			bool MY_LIB operator==(const Q &that)const noexcept {
				return ((this->Numerator == that.Numerator) && (this->Denominator == that.Denominator));
			}
			bool MY_LIB operator>(const Q &that)const noexcept {
				if (this->PosSign && !that.PosSign) {
					return true;
				}
				if (!this->PosSign && that.PosSign) {
					return false;
				}
				N &&temp1 = this->Numerator * that.Denominator, &&temp2 = this->Denominator * that.Numerator;
				bool &&res = ((temp1 > temp2) ? (this->PosSign) : (!this->PosSign));
				temp1.destruct();
				temp2.destruct();
				return res;
			}
			bool MY_LIB operator<(const Q &that)const noexcept {
				if (this->PosSign && !that.PosSign) {
					return false;
				}
				if (!this->PosSign && that.PosSign) {
					return true;
				}
				N &&temp1 = this->Numerator * that.Denominator, &&temp2 = this->Denominator * that.Numerator;
				bool &&res = ((temp1 < temp2) ? (this->PosSign) : (!this->PosSign));
				temp1.destruct();
				temp2.destruct();
				return res;
			}
			bool MY_LIB operator<=(const Q &that)const noexcept { return !(*this > that); }
			bool MY_LIB operator>=(const Q &that)const noexcept { return !(*this < that); }
			std::ostream &Print(std::ostream &o)const noexcept {
				if (!PosSign) {
					o << std::setw(o.width() - 1) << '-';
				}
				return this->uQ::Print(o);
			}
			friend std::ostream &operator<<(std::ostream &o, const Q &q) {
				std::stringstream sout;
				auto width = o.width();
				if (!q.PosSign) {
					sout << std::setw(1) << '-';
				}
				static_cast<const uQ &>(q).Print(sout);
				std::string tmp(size_t((width > sout.str().size()) ? width - sout.str().size() : 0), o.fill());
				sout >> tmp;
				return o << tmp;
			}
			friend Q operator-(Q &&that)noexcept {
				if (that.Numerator != 0) {
					that.PosSign = !that.PosSign;
				}
				return that;
			}
		};
	}
#ifdef Z_MAX
#undef Z_MAX
#endif // Z_MAX
}