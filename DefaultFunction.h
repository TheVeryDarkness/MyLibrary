#pragma once

#include "PreciseMath.h"

namespace LL {
	class Function; 
	class power_func;
	class sum;
	class product;
	class sin_pi;

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

	class power_func:public Function
	{
		using Q=LargeInteger::Q;
	public:
		explicit MY_LIBRARY power_func(const Q& Coefficient, const Q& Index)noexcept
			:coeff(Q::Copy(Coefficient)), power(Q::Copy(Index)) {
			ERR("幂函数逐项复制构造于" << this << ";值为" << *this << std::endl);
			this->simplify();
		}
		explicit MY_LIBRARY power_func(const Q& Coefficient)noexcept
			:coeff(Q::Copy(Coefficient)), power(1) {
			ERR("幂函数仅分子复制构造于" << this << ";值为" << *this << std::endl);
			this->simplify();
		}
		explicit MY_LIBRARY power_func(
			long Coefficient1, unsigned short Coefficient2,
			unsigned short Index1, unsigned short Index2
		)noexcept 
			: coeff(Coefficient1, Coefficient2), power(Index1, Index2) {
			ERR("幂函数初始值构造于" << this << std::endl);
			if (Index2 == 0 || Coefficient2 == 0)
			{
				assert(false);
				this->power = Coefficient1;
				this->coeff = Index1;
				return;
			}
			this->simplify();
		}
		MY_LIBRARY ~power_func()noexcept { 
			ERR("幂函数析构于" << this << ";值为" << *this << std::endl);
			this->coeff.destruct(); 
			this->power.destruct(); 
		}
		Function* MY_LIBRARY copy()noexcept { 
			auto&& temp = new power_func(this->coeff, this->power);
			ERR("复制了位于" << this << "的幂级数;其值为" << *this << ";新幂函数在" << temp << std::endl);
			return temp;
		}
		void MY_LIBRARY diff(Function*& f) noexcept {
			assert(this == f);
			ERR("幂函数求导于" << this << ";值为" << *this << std::endl);
			if (this->power == 0)
			{
				this->power = 0;
				this->coeff = 0;
			}
			else
			{
				this->coeff *= power;
				this->power -= 1;
			}
			return;
		}
		void MY_LIBRARY integral(Function*& f) noexcept {
			assert(this == f);
			power_func* res = new power_func(this->coeff, this->power);
			res->power += 1;
			if (res->power == 0)
			{
				assert(false);
				return;
			}
			res->coeff /= power;
			return;
		}
		void MY_LIBRARY simplify()noexcept {
			coeff.Simplify();
			power.Simplify();
		}
		std::ostream& MY_LIBRARY Print(std::ostream& o)const noexcept {
			return o <<"((" << coeff << ')' << " * x^(" << power << "))";
		}
		INLINED bool MY_LIBRARY operator==(const power_func& that)const noexcept {
			return (this->coeff == that.coeff && this->power == that.power);
		}
		INLINED bool MY_LIBRARY operator!=(const power_func& that)const noexcept {
			return !(*this == that);
		}
	private:
		Q coeff;//coefficent
		Q power;//index of issue
	};

	class sum:public Function
	{
	public:
		MY_LIBRARY sum(Function* a, Function* b) noexcept :a(a), b(b) { ERR("加和构造于" << this << ", 两成员为" << a << "和" << b << std::endl); }
		MY_LIBRARY ~sum() noexcept {
			ERR("加和析构于" << this << ", 两成员为" << a << "和" << b << std::endl);
			delete a;
			delete b;
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
		MY_LIBRARY product(Function* a, Function* b) noexcept :a(a), b(b) { ERR("乘积构造于" << this << ", 两成员为" << a << "和" << b << std::endl); }
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
			auto _a = a->copy();
			auto _b = b->copy();
			_a->diff(_a);
			_b->diff(_b);
			auto temp = f;
			f = new sum(new product(a, _b), new product(_a, b));
			a = b = nullptr;
			delete temp;
			return;
		}
	private:
		Function* a, * b;
	};

	class sin_pi :public Function {
		using Q=LargeInteger::Q;
	public:
		explicit MY_LIBRARY sin_pi(const Q& Coefficient, const Q& Index)noexcept
			:omega(Q::Copy(Coefficient)), phi(Q::Copy(Index)) {
			ERR("正弦函数逐项复制构造于" << this << ";值为" << *this << std::endl);
			this->simplify();
		}
		explicit MY_LIBRARY sin_pi(
			long Coefficient1, unsigned short Coefficient2,
			unsigned short Index1, unsigned short Index2
		)noexcept
			: omega(Coefficient1, Coefficient2), phi(Index1, Index2) {
			ERR("正弦函数初始值构造于" << this << std::endl);
			if (Index2 == 0 || Coefficient2 == 0)
			{
				assert(false);
				this->phi = Coefficient1;
				this->omega = Index1;
				return;
			}
			this->simplify();
		}
		MY_LIBRARY ~sin_pi()noexcept {
			ERR("正弦函数析构于" << this << ";值为" << *this << std::endl);
			this->omega.destruct();
			this->phi.destruct();
		}
		Function* MY_LIBRARY copy()noexcept {
			auto&& temp = new power_func(this->omega, this->phi);
			ERR("复制了位于" << this << "的正弦级数;其值为" << *this << ";新正弦函数在" << temp << std::endl);
			return temp;
		}
		void MY_LIBRARY diff(Function*& f) noexcept {
			assert(this == f);
			ERR("正弦函数求导于" << this << ";值为" << *this << std::endl);
			this->phi += 1;
			f = new product(new power_func(omega), f);
			return;
		}
		void MY_LIBRARY integral(Function*& f) noexcept {
			assert(this == f);
			sin_pi* res = new sin_pi(this->omega, this->phi);
			res->phi += 1;
			if (res->phi == 0)
			{
				assert(false);
				return;
			}
			res->omega /= phi;
			return;
		}
		void MY_LIBRARY simplify()noexcept {
			omega.Simplify();
			phi.Simplify();
		}
		std::ostream& MY_LIBRARY Print(std::ostream& o)const noexcept {
			return o << "((" << omega << ')' << " * x^(" << phi << "))";
		}
		INLINED bool MY_LIBRARY operator==(const sin_pi& that)const noexcept {
			return (this->omega == that.omega && this->phi == that.phi);
		}
		INLINED bool MY_LIBRARY operator!=(const sin_pi& that)const noexcept {
			return !(*this == that);
		}
	private:
		Q omega;//coefficent
		Q phi;//index of issue
	};
}