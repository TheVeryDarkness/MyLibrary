#pragma once

#include "LinkedList.h"

#define Z_MAX 10000

namespace LL {

	typedef DLL<unsigned short, Z_MAX> Z;

	//有理数
	//Rational Number
	//negative sign on denominator will be ignored
	class Q
	{
	protected:
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
		inline void MY_LIBRARY destruct() {
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
					(this->Denominator.next->data == 0)
					&&
					(this->Numerator.next->data == 0))
				{
					this->Denominator >>= 1;
					this->Numerator >>= 1;
				}
				else break;
			}
			long long difference = 0;

			if (Numerator == Denominator)
			{
				Numerator = 1;
				Denominator = 1;
			}
			else if (Numerator == 0)
			{
				Denominator = 1;
			}
			else if (Denominator == 0)
			{
#ifdef _DEBUG
				DEBUG_OUT;
				throw std::out_of_range("Denominator can't be 0.(from LL::Q::Simplify())");
#else
				Numerator = 1;
#endif // _DEBUG
			}
			else
			{
				Z a(Numerator, true), b(Denominator, true);
				a.data = 1;
				b.data = 1;
				while (true)
				{
					{
						bool _a = (a == 0), _b = (b == 0);
						if (_a && _b)
						{
#ifdef _DEBUG
							~a; ~b;
							DEBUG_OUT;
							throw std::out_of_range("Computation error");
#else
							~a; ~b;
							return;
#endif // _DEBUG
						}

						if (_a)
						{
							this->Numerator /= b;
							this->Denominator /= b;
							~a; ~b;
							return;
						}
						if (_b)
						{
							this->Numerator /= a;
							this->Denominator /= a;
							~a, ~b;
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
						difference = static_cast<long long>(a.RawLength()) - b.RawLength();
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

		void MY_LIBRARY operator+=(long that) {
			Z Product(this->Denominator * that);
			this->Numerator += Product;
			Product.destruct();
			this->Simplify();
		}
		void MY_LIBRARY operator-=(long that) {
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
		bool MY_LIBRARY operator==(long that)const {
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
	inline MY_LIBRARY Q::~Q() {}
}
#ifdef Z_MAX
#undef Z_MAX
#endif // Z_MAX