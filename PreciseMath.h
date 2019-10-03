#pragma once

#include "LinkedList.h"
#include "LargeInteger.h"
#include "OLL.h"
#include "DLL.h"
constexpr unsigned int Z_MAX = 1000000000U;

namespace LargeInteger {

	typedef LargeInteger::LargeSigned<LL::DLL<unsigned int>, Z_MAX> Z;
	typedef LargeInteger::LargeUnsigned<LL::DLL<unsigned int>, Z_MAX> N;

	//有理数
	//Rational Number
	//negative sign on denominator will be ignored
	class Q {
	protected:
		using Data=unsigned int;
		bool PosSign;
		N Numerator;//分子
		N Denominator;//分母
	public:
		//MY_LIB Q();
		explicit MY_LIB Q(bool sign, long n, unsigned short d = 1)noexcept :
			PosSign(sign),
			Numerator(static_cast<unsigned long>(abs(n))),
			Denominator(static_cast<unsigned long>(d)) { }
		MY_LIB Q(const Q &that) noexcept :
			PosSign(that.PosSign),
			Numerator(that.Numerator),
			Denominator(that.Denominator) { }
		explicit MY_LIB Q(bool sign, const N &Numerator, const N &Denominator) noexcept :
			PosSign(sign),
			Numerator(Numerator),
			Denominator(Denominator) { }
		static Q MY_LIB Copy(const Q &that)noexcept {
			Q temp = Q(that.PosSign, N::Copy(that.Numerator), N::Copy(that.Denominator));
			return temp;
		}
		Q &MY_LIB toReciprocal()noexcept {
			this->Numerator.Swap(this->Denominator);
		}
		template<typename val = double> val MY_LIB estim()const noexcept {
			static_assert(std::is_arithmetic_v<val> && !std::is_integral_v<val>);
			return this->Numerator.GetValue<val>() / (this->Denominator.GetValue<val>());
		}
		MY_LIB ~Q();
		INLINED void MY_LIB destruct() noexcept {
			this->Denominator.destruct();
			this->Numerator.destruct();
		}
		//约分
		void MY_LIB Simplify() {
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
				while (true) {
					if (a == 0) {
						this->Numerator /= b;
						this->Denominator /= b;
						a.destruct(); b.destruct();
						return;
					}
					if (b == 0) {
						this->Numerator /= a;
						this->Denominator /= a;
						a.destruct(), b.destruct();
						return;
					}
					if (a == b) {
						this->Numerator /= a;
						this->Denominator /= b;
						a.destruct();
						b.destruct();
						return;
					}
					{
						if (a < b) {
							b %= a;
						}
						else a %= b;
					}
				}
			}
		}
		void MY_LIB operator=(long that) {
			this->Denominator = 1;
			this->Numerator = that;
		}
		template<typename Int>
		void MY_LIB operator+=(Int that) {
			N Product(this->Denominator * that);
			this->Numerator += Product;
			Product.destruct();
			this->Simplify();
		}
		template<typename Int>
		void MY_LIB operator-=(Int that) {
			N Product = this->Denominator * that;
			this->Numerator -= Product;
			Product.destruct();
			this->Simplify();
		}
		Q MY_LIB operator-(const Q &that)const {
			Q Res = Copy(*this);
			Res -= that;
			return Res;
		}

		void MY_LIB operator+=(const N &that) {
			this->Numerator += that * this->Denominator;
			this->Simplify();
		}
		void MY_LIB operator-=(const N &that) {
			this->Numerator -= that * this->Denominator;
			this->Simplify();
		}

		void MY_LIB operator+=(const Q &that) {
			this->Numerator *= that.Denominator;
			this->Numerator += (that.Numerator * this->Denominator);
			this->Simplify();
		}
		void MY_LIB operator-=(const Q &that) {
			this->Numerator *= that.Denominator;
			this->Numerator -= (that.Numerator * this->Denominator);
			this->Denominator *= that.Denominator;
			this->Simplify();
		}
		Q MY_LIB operator+(const Q &that)const {
			Q Res = Q::Copy(*this);
			Res += that;
			return Res;
		}
		void MY_LIB operator*=(const Q &that) {
			this->Denominator *= that.Denominator;
			this->Numerator *= that.Numerator;
			this->Simplify();
		}
		void MY_LIB operator/=(const Q &that) {
			this->PosSign = that.PosSign ? (this->PosSign) : (!this->PosSign);
			this->Denominator *= that.Numerator;
			this->Numerator *= that.Denominator;
			this->Simplify();
		}
		Q MY_LIB operator*(const Q &that)const {
			Q Res = Q::Copy(*this);
			Res *= that;
			return Res;
		}
		Q MY_LIB operator/(const Q &that)const {
			Q Res = Q::Copy(*this);
			Res /= that;
			return Res;
		}
		template<typename Int>
		bool MY_LIB operator==(Int that)const {
			Z temp = this->Denominator * that;
			bool ret = (temp == this->Numerator);
			temp.destruct();
			return ret;
		}
		bool MY_LIB operator==(const Q &that)const {
			return ((this->Numerator == that.Numerator) && (this->Denominator == that.Denominator));
		}
		bool MY_LIB operator>(const Q &that)const {
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
		bool MY_LIB operator<(const Q &that)const {
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
		bool MY_LIB operator<=(const Q &that)const { return !(*this > that); }
		bool MY_LIB operator>=(const Q &that)const { return !(*this < that); }
		std::ostream &Print(std::ostream &o)const {
			if (this->Denominator == 1)
				return Numerator.Print();
			return Denominator.Print((Numerator.Print(o) << '/'));
		}
		friend std::ostream &operator<<(std::ostream &o, const Q &q) {
			return q.Print(o);
		}
	};
	INLINED MY_LIB Q::~Q() { }
}
#ifdef Z_MAX
#undef Z_MAX
#endif // Z_MAX