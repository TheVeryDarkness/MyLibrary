#pragma once

#include "LinkedList.h"
#include "LargeInteger.h"
#include "OLL.h"
#include "DLL.h"
constexpr unsigned int Z_MAX = 1000000000U;

namespace LargeInteger {

	typedef LargeInteger::LargeSigned<LL::DLL<LargeInteger::Num<unsigned int, Z_MAX>>, Z_MAX> Z;

	//有理数
	//Rational Number
	//negative sign on denominator will be ignored
	class Q {
	protected:
		using Data=LargeInteger::Num<unsigned int, Z_MAX>;
		Z Numerator;//分子
		Z Denominator;//分母
	public:
		//MY_LIBRARY Q();
		explicit MY_LIBRARY Q(long n, unsigned short d = 1)noexcept :
			Numerator(((n > 0) ? 1 : 0), static_cast<unsigned long>(abs(n))),
			Denominator(true, static_cast<unsigned long>(d)) { }
		MY_LIBRARY Q(const Q& that) noexcept :
			Numerator(that.Numerator),
			Denominator(that.Denominator) { }
		explicit MY_LIBRARY Q(const Z& Numerator, const Z& Denominator) noexcept :
			Numerator(Numerator),
			Denominator(Denominator) { }
		static Q MY_LIBRARY Copy(const Q& that)noexcept {
			Q temp = Q(Z::Copy(that.Numerator), Z::Copy(that.Denominator));
			return temp;
		}
		Q& MY_LIBRARY toReciprocal()noexcept {
			this->Numerator._Swap(this->Denominator);
		}
		template<typename val = double> val MY_LIBRARY estim()const noexcept {
			static_assert(std::is_arithmetic_v<val> && !std::is_integral_v<val>);
			return this->Numerator.GetValue<val>() / (this->Denominator.GetValue<val>());
		}
		MY_LIBRARY ~Q();
		INLINED void MY_LIBRARY destruct() noexcept {
			this->Denominator.destruct();
			this->Numerator.destruct();
		}
		//约分
		void MY_LIBRARY Simplify() {
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
				Z a = Z::Copy(Numerator), b = Z::Copy(Denominator);
				a.PosSign = true;
				b.PosSign = true;
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
		void MY_LIBRARY operator=(long that) {
			this->Denominator = 1;
			this->Numerator = that;
		}
		template<typename Int>
		void MY_LIBRARY operator+=(Int that) {
			Z Product(this->Denominator * that);
			this->Numerator += Product;
			Product.destruct();
			this->Simplify();
		}
		template<typename Int>
		void MY_LIBRARY operator-=(Int that) {
			Z Product = this->Denominator * that;
			this->Numerator -= Product;
			Product.destruct();
			this->Simplify();
		}
		Q MY_LIBRARY operator-(const Q& that)const {
			Q Res = Copy(*this);
			Res -= that;
			return Res;
		}

		void MY_LIBRARY operator+=(const Z& that) {
			this->Numerator += that * this->Denominator;
			this->Simplify();
		}
		void MY_LIBRARY operator-=(const Z& that) {
			this->Numerator -= that * this->Denominator;
			this->Simplify();
		}

		void MY_LIBRARY operator+=(const Q& that) {
			this->Numerator *= that.Denominator;
			this->Numerator += (that.Numerator * this->Denominator);
			this->Simplify();
		}
		void MY_LIBRARY operator-=(const Q& that) {
			this->Numerator *= that.Denominator;
			this->Numerator -= (that.Numerator * this->Denominator);
			this->Simplify();
		}
		Q MY_LIBRARY operator+(const Q& that)const {
			Q Res = Q::Copy(*this);
			Res += that;
			return Res;
		}
		void MY_LIBRARY operator*=(const Q& that) {
			this->Denominator *= that.Denominator;
			this->Numerator *= that.Numerator;
			this->Simplify();
		}
		void MY_LIBRARY operator/=(const Q& that) {
			this->Denominator *= that.Numerator;
			this->Numerator *= that.Denominator;
			this->Simplify();
			if (this->Numerator > 0 && !(this->Denominator > 0)) {
				this->Numerator.SetToContradict();
				this->Denominator.SetToContradict();
			}
		}
		Q MY_LIBRARY operator*(const Q& that)const {
			Q Res = Q::Copy(*this);
			Res *= that;
			return Res;
		}
		Q MY_LIBRARY operator/(const Q& that)const {
			Q Res = Q::Copy(*this);
			Res /= that;
			return Res;
		}
		template<typename Int>
		bool MY_LIBRARY operator==(Int that)const {
			Z temp = this->Denominator * that;
			bool ret = (temp == this->Numerator);
			temp.destruct();
			return ret;
		}
		bool MY_LIBRARY operator==(const Q& that)const {
			return ((this->Numerator == that.Numerator) && (this->Denominator == that.Denominator));
		}
		std::ostream& Print(std::ostream& o)const {
			if (this->Denominator == 1)
				return Numerator.Print();
			return Denominator.Print((Numerator.Print(o) << '/'));
		}
		friend std::ostream& operator<<(std::ostream& o, const Q& q) {
			return q.Print(o);
		}
	};
	INLINED MY_LIBRARY Q::~Q() { }
}
#ifdef Z_MAX
#undef Z_MAX
#endif // Z_MAX