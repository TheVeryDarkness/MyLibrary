#pragma once

#include "PreciseMath.h"

namespace LL {
	class Function
	{
	public:
		MY_LIBRARY Function()noexcept {}

		MY_LIBRARY ~Function()noexcept {}

		virtual Function& MY_LIBRARY diff()noexcept { return *this; }
		virtual Function& MY_LIBRARY integral()noexcept { return *this; }
		virtual void MY_LIBRARY destruct()noexcept {}

		virtual std::ostream& MY_LIBRARY Print(std::ostream& o)const noexcept { return o; }
		friend std::ostream& MY_LIBRARY operator<<(std::ostream& o, const Function& func) noexcept {
			return func.Print(o);
		}
	private:

	};

	class power:public Function
	{
	public:
		MY_LIBRARY power(const Q& Coefficient, const Q& Index)noexcept
			:c(Coefficient, true), i(Index, true) {
			this->simplify();
		}
		MY_LIBRARY power(
			long Coefficient1, unsigned short Coefficient2,
			unsigned short Index1, unsigned short Index2
		)noexcept 
			: c(Coefficient1, Coefficient2), i(Index1, Index2) {
			if (Index2 == 0 || Coefficient2 == 0)
			{
				assert(false);
				this->i = Coefficient1;
				this->c = Index1;
				return;
			}
			this->simplify();
		}
		MY_LIBRARY ~power()noexcept {}
		void MY_LIBRARY destruct()noexcept { this->c.destruct(); this->i.destruct(); }
		power& MY_LIBRARY diff() noexcept {
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
		power& MY_LIBRARY integral()noexcept {
			i += 1;
			if (i == 0)
			{
				assert(false);
				return *this;
			}
			c /= i;
			return *this;
		}
		void MY_LIBRARY simplify()noexcept {
			c.Simplify();
			i.Simplify();
		}
		std::ostream& MY_LIBRARY Print(std::ostream& o)const noexcept {
			return o << '(' << c << ')' << "*x^(" << i << ')';
		}
		INLINED bool MY_LIBRARY operator==(const power& that)const noexcept {
			return (this->c == that.c && this->i == that.i);
		}
		INLINED bool MY_LIBRARY operator!=(const power& that)const noexcept {
			return !(*this == that);
		}
		friend std::ostream& MY_LIBRARY operator<<(std::ostream& o, const power& p) noexcept {
			return p.Print(o);
		}
	private:
		Q c;//coefficent
		Q i;//index of issue
	};
}