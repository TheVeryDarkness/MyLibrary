#pragma once

#include "LinkedList.h"
#include "LargeInteger.h"
#include "OLL.h"
#include "DLL.h"
#include "list.h"
constexpr unsigned int Z_MAX = 1000000000U;

namespace LargeInteger {

	typedef LargeInteger::LargeSigned<LL::OAL<unsigned int, 64U / sizeof(void *) - 1>, Z_MAX> Z;
	typedef LargeInteger::LargeUnsigned<LL::OAL<unsigned int, 64U / sizeof(void *) - 1 >, Z_MAX> N;

	//有理数
	//Rational Number
	//negative sign on numerator and denominator will be ignored
	class Q {
	protected:
		using Data=unsigned int;
		bool PosSign;
		N Numerator;//分子
		N Denominator;//分母
	public:
		//MY_LIB Q();
		template<typename Val1, typename Val2>explicit MY_LIB Q(bool sign, const Val1 &&n, const Val2 &&d = 1)noexcept :
			PosSign(sign),
			Numerator(n),
			Denominator(d) { }
		MY_LIB Q(const Q &that) noexcept :
			PosSign(that.PosSign),
			Numerator(that.Numerator),
			Denominator(that.Denominator) { }
		explicit MY_LIB Q(bool sign, const N &Numerator, const N &Denominator) noexcept :
			PosSign(sign),
			Numerator(Numerator),
			Denominator(Denominator) { }
		INLINED MY_LIB ~Q() noexcept = default;
		static Q MY_LIB Copy(const Q &that)noexcept {
			return Q(that.PosSign, N::Copy(that.Numerator), N::Copy(that.Denominator));
		}
		Q &MY_LIB toReciprocal()noexcept {
			this->Numerator.Swap(this->Denominator);
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
				this->Denominator /= *Smaller;
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
			Z temp = this->Denominator * that;
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
			if (this->Denominator == 1)
				return Numerator.Print();
			return Denominator.Print((Numerator.Print(o) << '/'));
		}
		friend std::ostream &operator<<(std::ostream &o, const Q &q) {
			return q.Print(o);
		}
	};
}
#ifdef Z_MAX
#undef Z_MAX
#endif // Z_MAX