#pragma once

#include "PreciseMath.h"

namespace LL {
	class power
	{
	public:
		MY_LIBRARY power(const Q& Coefficient, const Q& Index)noexcept
			:c(Coefficient, true), i(Index, true) {
			this->simplify();
		}
		MY_LIBRARY power(
			long Coefficient1, unsigned short Coefficient2,
			unsigned short Index1, unsigned short Index2
		) : c(Coefficient1, Coefficient2), i(Index1, Index2) {
			if (Index2 == 0 || Coefficient2 == 0)
			{
#ifdef _DEBUG
				DEBUG_OUT;
#endif // _DEBUG
				throw std::invalid_argument("Denominator can't be 0.");
			}
			this->simplify();
		}
		MY_LIBRARY ~power() {}
		void MY_LIBRARY destruct() { this->c.destruct(); this->i.destruct(); }
		power& MY_LIBRARY diff() {
			if (i == 0)
			{
				i = 0;
				c = 0;
			}
			else
			{
				c *= i;
				i -= 1;
			}
			return *this;
		}
		power& MY_LIBRARY integral() {
			i += 1;
			if (i == 0)
			{
#ifdef _DEBUG
				DEBUG_OUT;
#endif // _DEBUG
				throw std::runtime_error("Not supported");
			}
			c /= i;
			return *this;
		}
		void MY_LIBRARY simplify() {
			c.Simplify();
			i.Simplify();
		}
		std::ostream& MY_LIBRARY Print(std::ostream& o)const {
			return o << '(' << c << ')' << "*x^(" << i << ')';
		}
		INLINED bool MY_LIBRARY operator==(const power& that)const {
			return (this->c == that.c && this->i == that.i);
		}
		INLINED bool MY_LIBRARY operator!=(const power& that)const {
			return !(*this == that);
		}
		friend std::ostream& MY_LIBRARY operator<<(std::ostream& o, const power& p) {
			return p.Print(o);
		}
	private:
		Q c;//coefficent
		Q i;//index of issue
	};
}