#pragma once

#include "PreciseMath.h"

namespace LL {
	class Function; 
	class power;
	class sum;
	class product;

	class Function
	{
	public:
		MY_LIBRARY Function()noexcept {};
		MY_LIBRARY Function(const Function& that)noexcept = delete;

		virtual MY_LIBRARY ~Function()noexcept {}

		virtual void MY_LIBRARY diff(Function*&) noexcept = 0;
		virtual void MY_LIBRARY integral(Function*&) noexcept = 0;
		virtual Function* MY_LIBRARY copy()noexcept = 0;
		virtual std::ostream& MY_LIBRARY Print(std::ostream& o) const noexcept = 0;
		friend std::ostream& MY_LIBRARY operator<<(std::ostream& o, const Function& fun)noexcept {
			return fun.Print(o);
		}
	private:

	};

	class power:public Function
	{
		using Q=LargeInteger::Q;
	public:
		explicit MY_LIBRARY power(const Q& Coefficient, const Q& Index)noexcept
			:c(Q::Copy(Coefficient)), i(Q::Copy(Index)) {
			ERR("逐项复制构造幂函数于" << this << ";值为" << *this << std::endl);
			this->simplify();
		}
		explicit MY_LIBRARY power(
			long Coefficient1, unsigned short Coefficient2,
			unsigned short Index1, unsigned short Index2
		)noexcept 
			: c(Coefficient1, Coefficient2), i(Index1, Index2) {
			ERR("输入值法构造幂函数于" << this << std::endl);
			if (Index2 == 0 || Coefficient2 == 0)
			{
				assert(false);
				this->i = Coefficient1;
				this->c = Index1;
				return;
			}
			this->simplify();
		}
		MY_LIBRARY ~power()noexcept { 
			ERR("析构幂函数:" << this << ";值为" << *this << std::endl);
			this->c.destruct(); 
			this->i.destruct(); 
		}
		Function* MY_LIBRARY copy()noexcept { 
			ERR("复制了位于" << this << "的幂级数;其值为" << *this << std::endl);
			return new power(this->c, this->i);
		}
		void MY_LIBRARY diff(Function*& f) noexcept {
			assert(this == f);
			ERR("幂函数于" << this << "求导;值为" << *this << std::endl);
			if (this->i == 0)
			{
				this->i = 0;
				this->c = 0;
			}
			else
			{
				this->c *= i;
				this->i -= 1;
			}
			return;
		}
		void MY_LIBRARY integral(Function*& f) noexcept {
			assert(this == f);
			power* res = new power(this->c, this->i);
			res->i += 1;
			if (res->i == 0)
			{
				assert(false);
				return;
			}
			res->c /= i;
			return;
		}
		void MY_LIBRARY simplify()noexcept {
			c.Simplify();
			i.Simplify();
		}
		std::ostream& MY_LIBRARY Print(std::ostream& o)const noexcept {
			return o <<"((" << c << ')' << " * x^(" << i << "))";
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

	class sum:public Function
	{
	public:
		MY_LIBRARY sum(Function* a, Function* b) noexcept :a(a), b(b) { }
		MY_LIBRARY ~sum() noexcept {
			delete a, b;
		}
		sum* MY_LIBRARY copy()noexcept { return new sum(a->copy(), b->copy()); }
		void MY_LIBRARY integral(Function*& f) noexcept {
			assert(this == f);
			a->integral(a);
			b->integral(b);
			return;
		}
		void MY_LIBRARY diff(Function*& f) noexcept {
			assert(this == f);
			a->diff(a);
			b->diff(b);
			return;
		}
		std::ostream& MY_LIBRARY Print(std::ostream& o)const noexcept {
			return o << "(" << *a << " + " << *b << ")";
		}
	private:
		Function* a, * b;
	};


	class product :public Function {
	public:
		MY_LIBRARY product(Function* a, Function* b) noexcept :a(a), b(b) { ERR("乘积构造于" << this << std::endl); }
		MY_LIBRARY ~product() noexcept {
			ERR("乘积析构于" << this << ", 两成员为" << a << "和" << b << std::endl);
			if (a != nullptr)
			{
				delete a;
			}
			if (b != nullptr)
			{
				delete b;
			}
		}
		std::ostream& MY_LIBRARY Print(std::ostream&o)const noexcept {
			return o << "(" << *a << " * " << *b << ")";
		}
		product* MY_LIBRARY copy()noexcept {
			return new product(a->copy(), b->copy());
		}
		void MY_LIBRARY integral(Function*& f) noexcept {
			assert(this == f);
			assert(false);
			auto&& temp = new product(a->copy(), b->copy());
			return;
		}
		void MY_LIBRARY diff(Function*& f) noexcept {
			assert(this == f);
			auto&& _a = a->copy();
			auto&& _b = b->copy();
			_a->diff(_a);
			_b->diff(_b);
			auto&& temp = f;
			f = new sum(new product(a, _b), new product(_a, b));
			a = b = nullptr;
			delete temp;
			return;
		}
	private:
		Function* a, * b;
	};
}