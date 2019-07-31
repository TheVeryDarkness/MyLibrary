#pragma once

#include "LinkedList.h"

#define Z_MAX 10000

namespace LL {

	typedef OLL<unsigned short, Z_MAX> Z;

	//有理数
	//Rational Number
	//negative sign on denominator will be ignored
	class Q
	{
	protected:
		using Data=LargeInteger::Num<unsigned short, Z_MAX>;
		Z Numerator;//分子
		Z Denominator;//分母
	public:
		//MY_LIBRARY Q();
		explicit MY_LIBRARY Q(long n, unsigned short d = 1)noexcept :
			Numerator(((n > 0) ? 1 : 0), static_cast<unsigned long>(abs(n))),
			Denominator(true, static_cast<unsigned long>(d)) {}
		MY_LIBRARY Q(const Q& that, bool DeepCopy) noexcept :
			Denominator(that.Denominator, DeepCopy),
			Numerator(that.Numerator, DeepCopy) {}
		MY_LIBRARY ~Q();
		INLINED void MY_LIBRARY destruct() {
			this->Denominator.destruct();
			this->Numerator.destruct();
		}
		//约分
		void MY_LIBRARY Simplify() {
			while (true)
			{
				if (this->Denominator.next == nullptr && this->Numerator.next == nullptr)
				{
					return;
				}
				if (this->Denominator.next == nullptr || this->Numerator.next == nullptr)
				{
					break;
				}
				if (
					(this->Denominator.next->data == Data(0))
					&&
					(this->Numerator.next->data == Data(0)))
				{
					this->Denominator >>= 1;
					this->Numerator >>= 1;
				}
				else break;
			}
			if (Numerator == Denominator)
			{
				Numerator = 1;
				Denominator = 1;
			}
			else if (Numerator == Z(0))
			{
				Denominator = 1;
			}
			else if (Denominator == Z(0))
			{
				MY_ASSERT(false,0/0);
				Numerator = 1;
			}
			else
			{
				Z a(Numerator, true), b(Denominator, true);
				a.data = Data(1);
				b.data = Data(1);
				while (true)
				{
					{
						bool _a = (a.next == nullptr), _b = (b.next == nullptr);
						if (_a && _b)
						{
							MY_ASSERT(false, Unknown error);
							a.destruct(); b.destruct();
							return;
						}
						if (_a)
						{
							this->Numerator /= b;
							this->Denominator /= b;
							a.destruct(); b.destruct();
							return;
						}
						if (_b)
						{
							this->Numerator /= a;
							this->Denominator /= a;
							a.destruct(), b.destruct();
							return;
						}
					}
					if (a == b)
					{
						this->Numerator /= a;
						this->Denominator /= b;
						a.destruct();
						b.destruct();
						return;
					}
					{
						if (a < b)
						{
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

		void MY_LIBRARY operator+=(Data that) {
			Z Product(this->Denominator * that);
			this->Numerator += Product;
			Product.destruct();
			this->Simplify();
		}
		void MY_LIBRARY operator-=(Data that) {
			Z Product = this->Denominator * that;
			this->Numerator -= Product;
			Product.destruct();
			this->Simplify();
		}
		Q MY_LIBRARY operator-(const Q& that)const {
			Q Res(*this, true);
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
			Q Res(*this, true);
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
			if (this->Numerator.IsPositive() && (!this->Denominator.IsPositive()))
			{
				this->Numerator.SetToContradict();
				this->Denominator.SetToContradict();
			}
		}
		Q MY_LIBRARY operator*(const Q& that)const {
			Q Res(*this, true);
			Res *= that;
			return Res;
		}
		Q MY_LIBRARY operator/(const Q& that)const {
			Q Res(*this, true);
			Res /= that;
			return Res;
		}
		bool MY_LIBRARY operator==(Data that)const {
			Z temp = this->Denominator * that;
			bool ret = (temp == this->Numerator);
			temp.destruct();
			return ret;
		}
		bool MY_LIBRARY operator==(const Q& that)const {
			return ((this->Numerator == that.Numerator) && (this->Denominator == that.Denominator));
		}
		std::ostream& Print(std::ostream& o)const {
			if (this->Denominator == Z(1))
				return Numerator.Print();
			return Denominator.Print((Numerator.Print(o) << '/'));
		}
		friend std::ostream& operator<<(std::ostream& o, const Q& q) {
			return q.Print(o);
		}
	};
	INLINED MY_LIBRARY Q::~Q() {}
}
#ifdef Z_MAX
#undef Z_MAX
#endif // Z_MAX