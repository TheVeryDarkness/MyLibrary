#pragma once
#include "Shared.h"
#include <sstream>

namespace Darkness {
	inline namespace LargeRational {

		//非负有理数
		//Non-negative Rational Number
		//分子分母上的负号将被忽略
		//negative sign on numerator and denominator will be ignored
		template<typename N>
		class RationalUnsigned {
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
				DecimalMaker() noexcept { }
				~DecimalMaker()noexcept = default;
				N first(const char *str, const char *const end = nullptr) noexcept {
					assert(is_numerator);
					is_numerator = false;//怕忘了改
					for (; *str == '0' && (end == nullptr || str <= end); ++str);
					const char *p = str;
					for (; *p != '\0' && (end == nullptr || p <= end); ++p) {
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
					return std::move<N>(str == p ? N(0) : N::MakeFromString(str, p - 1));
				}
				N second(const char *end = nullptr) noexcept {
					assert(!is_numerator);
					return (
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
			explicit MY_LIB RationalUnsigned(const char *str, DecimalMaker maker = DecimalMaker())noexcept
				:Numerator(std::move(maker.first(str))), Denominator(std::move(maker.second())) {
				this->Simplify();
			}
			explicit MY_LIB RationalUnsigned(const char *begin, const char *end, DecimalMaker maker = DecimalMaker())noexcept
				:Numerator(std::move(maker.first(begin, end))), Denominator(std::move(maker.second())) {
				this->Simplify();
			}
			template<typename Val1, typename Val2>
			explicit MY_LIB RationalUnsigned(const Val1 &&n, const Val2 &&d = 1)noexcept :
				Numerator(std::move(n)),
				Denominator(std::move(d)) {
				//static_assert(std::is_integral_v<Val1> && std::is_integral_v<Val2>, "Integral type required.");
				this->Simplify();
			}
			explicit MY_LIB RationalUnsigned(N &&n, N &&d)noexcept :
				Numerator(std::move(n)),
				Denominator(std::move(d)) {
				//static_assert(std::is_integral_v<Val1> && std::is_integral_v<Val2>, "Integral type required.");
				this->Simplify();
			}
			MY_LIB RationalUnsigned(const RationalUnsigned &that) = delete;
			MY_LIB RationalUnsigned(RationalUnsigned &&that) noexcept :
				Numerator(std::move(that.Numerator)),
				Denominator(std::move(that.Denominator)) { }
			INLINED MY_LIB ~RationalUnsigned() noexcept = default;
			static RationalUnsigned MY_LIB Copy(const RationalUnsigned &that)noexcept {
				return RationalUnsigned(std::move(N::Copy(that.Numerator)), std::move(N::Copy(that.Denominator)));
			}
			bool MY_LIB isInteger()const noexcept {
				return this->Denominator == 1;
			}
			RationalUnsigned &MY_LIB toReciprocal()noexcept {
				this->Numerator.Swap(this->Denominator);
				return *this;
			}
			template<typename val = double> val MY_LIB estim()const noexcept {
				static_assert(std::is_arithmetic_v<val> && !std::is_integral_v<val>);
				return (this->Numerator.template GetValue<val>() / this->Denominator.template GetValue<val>());
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
					assert(Denominator != 0);
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
			RationalUnsigned &MY_LIB operator=(RationalUnsigned &&that)noexcept {
				this->Numerator = std::move(that.Numerator);
				this->Denominator = std::move(that.Denominator);
				return *this;
			}
			template<typename Int>
			void MY_LIB operator+=(Int that) noexcept {
				N Product(this->Denominator * that);
				this->Numerator += Product;
				//Product.destruct();
				this->Simplify();
			}
			template<typename Int>
			void MY_LIB operator-=(Int that) noexcept {
				N Product = this->Denominator * that;
				this->Numerator -= Product;
				//Product.destruct();
				this->Simplify();
			}
			template<typename Int>
			void MY_LIB operator*=(Int that) noexcept {
				this->Numerator *= that;
				this->Simplify();
			}
			template<typename Int>
			void MY_LIB operator/=(Int that) noexcept {
				this->Denominator *= that;
				this->Simplify();
			}
			RationalUnsigned MY_LIB operator-(const RationalUnsigned &that)const noexcept {
				RationalUnsigned Res = Copy(*this);
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

			void MY_LIB operator+=(const RationalUnsigned &that) noexcept {
				this->Numerator *= that.Denominator;
				N &&tmp = that.Numerator * this->Denominator;
				this->Numerator += tmp;
				tmp.destruct();
				this->Denominator *= that.Denominator;
				this->Simplify();
			}
			void MY_LIB operator-=(const RationalUnsigned &that) noexcept {
				this->Numerator *= that.Denominator;
				N &&tmp = that.Numerator * this->Denominator;
				assert(this->Numerator >= tmp);
				this->Numerator -= tmp;
				tmp.destruct();
				this->Denominator *= that.Denominator;
				this->Simplify();
			}
			RationalUnsigned MY_LIB operator+(const RationalUnsigned &that)const noexcept {
				RationalUnsigned Res = RationalUnsigned::Copy(*this);
				Res += that;
				return Res;
			}
			void MY_LIB operator*=(const RationalUnsigned &that) noexcept {
				this->Denominator *= that.Denominator;
				this->Numerator *= that.Numerator;
				this->Simplify();
			}
			void MY_LIB operator/=(const RationalUnsigned &that) noexcept {
				this->Denominator *= that.Numerator;
				this->Numerator *= that.Denominator;
				this->Simplify();
			}
			RationalUnsigned MY_LIB operator*(const RationalUnsigned &that)const noexcept {
				RationalUnsigned Res = RationalUnsigned::Copy(*this);
				Res *= that;
				return Res;
			}
			RationalUnsigned MY_LIB operator/(const RationalUnsigned &that)const noexcept {
				RationalUnsigned Res = RationalUnsigned::Copy(*this);
				Res /= that;
				return Res;
			}
			RationalUnsigned &MY_LIB pow(const RationalUnsigned &times) {
				if (times == 0) {
					*this = 1;
					return *this;
				}
				RationalUnsigned base = Copy(*this);
				if (times.isInteger()) {
					for (N i(1); times > i; i += 1) {
						*this *= base;
					}
				}
				return *this;
			}
			template<typename Int>
			bool MY_LIB operator==(Int that)const noexcept {
				N temp = this->Denominator * that;
				bool ret = (temp == this->Numerator);
				//temp.destruct();
				return ret;
			}
			bool MY_LIB operator==(const RationalUnsigned &that)const noexcept {
				return ((this->Numerator == that.Numerator) && (this->Denominator == that.Denominator));
			}
			bool MY_LIB operator>(const RationalUnsigned &that)const noexcept {
				N &&temp1 = this->Numerator * that.Denominator, &&temp2 = this->Denominator * that.Numerator;
				bool &&res = (temp1 > temp2);
				//temp1.destruct();
				//temp2.destruct();
				return res;
			}
			bool MY_LIB operator<(const RationalUnsigned &that)const noexcept {
				N &&temp1 = this->Numerator * that.Denominator, &&temp2 = this->Denominator * that.Numerator;
				bool &&res = (temp1 < temp2);
				//temp1.destruct();
				//temp2.destruct();
				return res;
			}
			bool MY_LIB operator<=(const RationalUnsigned &that)const noexcept { return !(*this > that); }
			bool MY_LIB operator>=(const RationalUnsigned &that)const noexcept { return !(*this < that); }
			bool MY_LIB operator>(const N &that)const noexcept {
				N &&temp2 = this->Denominator * that;
				bool &&res = (this->Numerator > temp2);
				//temp2.destruct();
				return res;
			}
			bool MY_LIB operator<(const N &that)const noexcept {
				N &&temp2 = this->Denominator * that;
				bool &&res = (this->Numerator < temp2);
				//temp2.destruct();
				return res;
			}
			bool MY_LIB operator<=(const N &that)const noexcept { return !(*this > that); }
			bool MY_LIB operator>=(const N &that)const noexcept { return !(*this < that); }
			std::ostream &Print(std::ostream &o)const noexcept {
				if (this->Denominator == 1) return Numerator.Print(o);
				return Denominator.Print(Numerator.Print(o) << '/');
			}
			friend std::ostream &operator<<(std::ostream &o, const RationalUnsigned &q) noexcept {
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
		template<typename N>
		class RationalSigned :protected RationalUnsigned<N> {
		protected:
			using Data = typename N::Data;
			bool PosSign;
			using super = RationalUnsigned<N>;
		public:
			explicit MY_LIB RationalSigned(const char *str)noexcept
				: PosSign(*str != '-'), super(*str != '-' ? str : str + 1) { }
			explicit MY_LIB RationalSigned(const char *begin, const char *end)noexcept
				: PosSign(*begin != '-'), super(*begin != '-' ? begin : begin + 1, end) { }
			template<typename Val1, typename Val2>
			explicit MY_LIB RationalSigned(bool sign, const Val1 &&n, const Val2 &&d = 1)noexcept :
				PosSign(sign), super(std::move(n), std::move(d)) {
				this->checkSign();
			}
			explicit MY_LIB RationalSigned(bool sign, N &&n, N &&d)noexcept :
				PosSign(sign), super(std::move(n), std::move(d)) {
				this->checkSign();
			}
			MY_LIB RationalSigned(RationalSigned &&that)noexcept :PosSign(that.PosSign), super(static_cast<super &&>(that)) { }
			INLINED MY_LIB ~RationalSigned() noexcept = default;
			const super &MY_LIB operator()()const noexcept {
				return static_cast<const super &>(*this);
			}
			static RationalSigned MY_LIB Copy(const RationalSigned &that)noexcept {
				return RationalSigned(that.PosSign, N::Copy(that.Numerator), N::Copy(that.Denominator));
			}
			RationalSigned &MY_LIB toReciprocal()noexcept {
				this->super::toReciprocal();
				return *this;
			}
			template<typename val = double> val MY_LIB estim()const noexcept {
				static_assert(std::is_arithmetic_v<val> && !std::is_integral_v<val>);
				return (this->PosSign ?
					(this->Numerator.template GetValue<val>() / (this->Denominator.template GetValue<val>()))
					:
					-(this->Numerator.template GetValue<val>() / (this->Denominator.template GetValue<val>()))
					);
			}
			void MY_LIB checkSign()noexcept {
				if (this->Numerator == 0) this->PosSign = true;
			}
			//约分
			void MY_LIB Simplify() noexcept {
				this->super::Simplify();
				this->checkSign();
			}
			bool MY_LIB isInteger()const noexcept {
				return this->super::isInteger();
			}
			bool MY_LIB	isPositive()const noexcept {
				return this->PosSign;
			}
			template<typename Int>
			RationalSigned &MY_LIB operator=(Int that) noexcept {
				static_assert(std::is_integral_v<Int>, "Integral type required.");
				this->PosSign = that >= 0;
				this->Denominator = 1;
				this->Numerator = that;
				return *this;
			}
			RationalSigned &MY_LIB operator=(RationalSigned &&that) noexcept {
				this->PosSign = that.PosSign;
				this->super::operator=(static_cast<super &&>(that));
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
			RationalSigned MY_LIB operator-(const RationalSigned &that)const noexcept {
				RationalSigned Res = Copy(*this);
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

			void MY_LIB operator+=(const RationalSigned &that) noexcept {
				this->Numerator *= that.Denominator;
				N &&tmp = that.Numerator * this->Denominator;
				if (that.PosSign ^ this->PosSign) {
					if (this->Numerator > tmp) {
						this->Numerator -= tmp;
					}
					else {
						N &&res = tmp - this->Numerator;
						this->Numerator.Swap(res);
						//res.destruct();
						this->PosSign = !this->PosSign;
					}
				}
				else {
					this->Numerator += tmp;
				}
				//tmp.destruct();
				this->Denominator *= that.Denominator;
				this->Simplify();
			}
			void MY_LIB operator-=(const RationalSigned &that) noexcept {
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
						N res = tmp - this->Numerator;
						this->Numerator.Swap(res);
						//res.destruct();
					}
				}
				//tmp.destruct();
				this->Denominator *= that.Denominator;
				this->Simplify();
			}
			RationalSigned MY_LIB operator+(const RationalSigned &that)const noexcept {
				RationalSigned Res = RationalSigned::Copy(*this);
				Res += that;
				return Res;
			}
			void MY_LIB operator*=(const RationalSigned &that) noexcept {
				this->PosSign = !(that.PosSign ^ this->PosSign);
				this->Denominator *= that.Denominator;
				this->Numerator *= that.Numerator;
				this->Simplify();
			}
			void MY_LIB operator/=(const RationalSigned &that) noexcept {
				this->PosSign = !(that.PosSign ^ this->PosSign);
				this->Denominator *= that.Numerator;
				this->Numerator *= that.Denominator;
				this->Simplify();
			}
			RationalSigned MY_LIB operator*(const RationalSigned &that)const noexcept {
				RationalSigned Res = RationalSigned::Copy(*this);
				Res *= that;
				return Res;
			}
			RationalSigned MY_LIB operator/(const RationalSigned &that)const noexcept {
				RationalSigned Res = RationalSigned::Copy(*this);
				Res /= that;
				return Res;
			}
			RationalSigned &MY_LIB pow(const RationalSigned &times) {
				this->super::pow(times);
				if (!times.PosSign) {
					this->toReciprocal();
				}
				return *this;
			}
			template<typename Int>
			bool MY_LIB operator==(Int that)const noexcept {
				N temp = this->Denominator * that;
				bool ret = (temp == this->Numerator);
				//temp.destruct();
				return ret;
			}
			template<typename Int>
			bool MY_LIB operator!=(Int that)const noexcept { return !(*this == that); }
			bool MY_LIB operator==(const RationalSigned &that)const noexcept {
				return ((this->Numerator == that.Numerator) && (this->Denominator == that.Denominator));
			}
			bool MY_LIB operator!=(const RationalSigned &that)const noexcept { return !(*this == that); }
			bool MY_LIB operator>(const RationalSigned &that)const noexcept {
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
			bool MY_LIB operator<(const RationalSigned &that)const noexcept {
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
			bool MY_LIB operator<=(const RationalSigned &that)const noexcept { return !(*this > that); }
			bool MY_LIB operator>=(const RationalSigned &that)const noexcept { return !(*this < that); }
			std::ostream &Print(std::ostream &o)const noexcept {
				if (!PosSign) {
					o << std::setw(o.width() - 1) << '-';
				}
				return this->super::Print(o);
			}
			friend std::ostream &operator<<(std::ostream &o, const RationalSigned &q) {
				std::stringstream sout;
				auto width = o.width();
				if (!q.PosSign) {
					sout << std::setw(1) << '-';
				}
				static_cast<const super &>(q).Print(sout);
				std::string tmp(size_t((width > sout.str().size()) ? width - sout.str().size() : 0), o.fill());
				sout >> tmp;
				return o << tmp;
			}
			friend RationalSigned operator-(RationalSigned &&that)noexcept {
				if (that.Numerator != 0) {
					that.PosSign = !that.PosSign;
				}
				return std::move(that);
			}
		};
	}
}