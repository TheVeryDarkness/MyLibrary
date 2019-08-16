#pragma once

#include "PreciseMath.h"

#define new DBG_NEW
namespace Function {
	using value=double;
	class function;
	class num;
	class x;
	template<size_t count>class sum;
	template<size_t count>class product;
	class power_func;
	class fsin;

	class function {
	public:
		MY_LIBRARY function()noexcept { };
		MY_LIBRARY function(const function& that)noexcept = delete;

		virtual MY_LIBRARY ~function()noexcept { }

		virtual void MY_LIBRARY diff(function*&) noexcept = 0;
		virtual void MY_LIBRARY integral(function*&) noexcept = 0;
		virtual function* MY_LIBRARY copy()noexcept = 0;
		virtual value MY_LIBRARY estimate()const noexcept = 0;
		virtual std::ostream& MY_LIBRARY Print(std::ostream& o) const noexcept = 0;
		friend std::ostream& MY_LIBRARY operator<<(std::ostream& o, const function& fun)noexcept {
			return fun.Print(o);
		}
	private:

	};

	class num :public function {
	public:
		template<typename Val>
		MY_LIBRARY num(Val val)noexcept :q(val) { }
		MY_LIBRARY num(const LargeInteger::Q& val)noexcept :q(val) { }

		MY_LIBRARY ~num()noexcept {
			q.destruct();
		}

		void MY_LIBRARY diff(function*& f) noexcept {
			assert(this == f);
			q = 0;
		};
		void MY_LIBRARY integral(function*&) noexcept { };
		function* MY_LIBRARY copy()noexcept { return new num(LargeInteger::Q::Copy(q)); };
		value MY_LIBRARY estimate()const noexcept { return this->q.estim<value>(); }
		std::ostream& MY_LIBRARY Print(std::ostream& o) const noexcept { return q.Print(o); };

	private:
		LargeInteger::Q q;
	};

	template<>
	class sum<1> :public function {
	public:
		MY_LIBRARY sum(function* p)noexcept :p(p) { }
		MY_LIBRARY ~sum() noexcept { delete p; }
		sum* MY_LIBRARY copy()noexcept { return new sum(p->copy()); }
		std::ostream& MY_LIBRARY Print(std::ostream& o)const noexcept {
			return o << *p << ")";
		}
		void MY_LIBRARY integral(function*& f) noexcept {
			assert(this == f);
			assert(false);
			return;
		}
		void MY_LIBRARY diff(function*& f) noexcept {
			assert(this == f);
			f->diff(f);
			return;
		}
		value MY_LIBRARY estimate()const noexcept {
			return this->p->estimate();
		}
		std::ostream& MY_LIBRARY _Print(std::ostream& o)const noexcept {
			return o << *p << ')';
		}
	private:
		function* p;
	};

	template<size_t count>
	class sum :public sum<count - 1>, public function {
	public:
		template<typename ...pack>
		MY_LIBRARY sum(function* p, pack... _p) noexcept :p(p), sum<count - 1>(_p...) {
			ERR("加和构造于" << this << ", 当前成员在" << p << "." << std::endl);
		}
		MY_LIBRARY ~sum() noexcept {
			ERR("加和析构于" << this << ", 当前成员为" << p << "." << std::endl);
			delete p;
		}
		sum* MY_LIBRARY copy()noexcept { return new sum(p->copy(), this->sum<count - 1>::copy()); }
		void MY_LIBRARY integral(function*& f) noexcept {
			assert(this == f);
			p->integral(p);
			return;
		}
		void MY_LIBRARY diff(function*& f) noexcept {
			assert(this == f);
			p->diff(p);
			return;
		}
		value MY_LIBRARY estimate()const noexcept {
			return this->p->estimate() + this->sum<count - 1>::estimate();
		}
		std::ostream& MY_LIBRARY _Print(std::ostream& o)const noexcept {
			o << *p << " + ";
			return static_cast<const sum<count - 1>*>(this)->_Print(o);
		}
		std::ostream& MY_LIBRARY Print(std::ostream& o)const noexcept {
			return this->_Print(o << '(');
		}
	private:
		function* p;
	};

	template<>
	class product<1> :public function {
	public:
		MY_LIBRARY product(function* p)noexcept :p(p) { }
		MY_LIBRARY ~product() noexcept { delete p; }
		product* MY_LIBRARY copy()noexcept { return new product(p->copy()); }
		std::ostream& MY_LIBRARY Print(std::ostream& o)const noexcept {
			return o << *p;
		}
		void MY_LIBRARY integral(function*& f) noexcept {
			assert(this == f);
			assert(false);
			return;
		}
		void MY_LIBRARY diff(function*& f) noexcept {
			assert(this == f);
			p->diff(p);
			return;
		}
		value MY_LIBRARY estimate()const noexcept {
			return this->p->estimate();
		}
		std::ostream& MY_LIBRARY _Print(std::ostream& o)const noexcept {
			return o << *p << ')';
		}
	private:
		function* p;
	};

	template<size_t count = 2>
	class product :public product<count - 1>, public function {
	public:
		template<typename ...pack> MY_LIBRARY product(function* p, pack... _p) noexcept :p(p), product<count - 1>(_p...) { ERR("乘积构造于" << this << ", 当前成员在" << p << "." << std::endl); }
		explicit MY_LIBRARY product(product* _p) noexcept :p(_p->p), product<count - 1>(static_cast<product<count - 1>*>(_p)) { }
		MY_LIBRARY ~product() noexcept {
			ERR("乘积析构于" << this << ", 当前成员为" << p << "." << std::endl);
			if (p != nullptr) {
				delete p;
			}
		}
		std::ostream& MY_LIBRARY _Print(std::ostream& o)const noexcept {
			o << *p << " * ";
			return static_cast<const product<count - 1>*>(this)->_Print(o);
		}
		std::ostream& MY_LIBRARY Print(std::ostream& o)const noexcept {
			return this->_Print(o << '(');
		}
		value MY_LIBRARY estimate()const noexcept {
			return this->p->estimate() * this->product<count - 1>::estimate();
		}
		product* MY_LIBRARY copy()noexcept {
			return new product(p->copy(), product<count - 1>::copy());
		}
		void MY_LIBRARY integral(function*& f) noexcept {
			assert(this == f);
			assert(false);
			return;
		}
		void MY_LIBRARY diff(function*& f) noexcept {
			assert(this == f);
			auto _p = p->copy();
			_p->diff(_p);
			function* _this = this->product<count - 1>::copy();
			_this->diff(_this);
			f = new sum<2>(new product<2>(_p, this->product<count - 1>::copy()), new product<2>(p, _this));
			p = nullptr;
			delete this;
			return;
		}
	private:
		function* p;
	};

	class power_func :public function {
		using Q=LargeInteger::Q;
	public:
		MY_LIBRARY power_func(function* base, function* expo) noexcept
			:base(base), expo(expo) {
			ERR("幂构造于" << this << ", 两成员为" << *base << "和" << *expo << std::endl);
		}
		MY_LIBRARY ~power_func()noexcept {
			ERR("幂函数析构于" << this << ";值为" << *this << std::endl);
		}
		function* MY_LIBRARY copy()noexcept {
			auto&& temp = new power_func(this->base->copy(), this->expo->copy());
			ERR("复制了位于" << this << "的幂级数;其值为" << *this << ";新幂函数在" << temp << std::endl);
			return temp;
		}
		void MY_LIBRARY diff(function*& f) noexcept {
			assert(this == f);
			ERR("幂函数求导于" << this << ";值为" << *this << std::endl);
			function* _base = base->copy(), * _expo = expo->copy();
			_base->diff(_base);
			_expo->diff(_expo);
			f = new product<2>(new sum<2>(new product<3>(_base, _expo, base->copy()), new product<2>(_base, _expo)), f);
			return;
		}
		void MY_LIBRARY integral(function*& f) noexcept {
			assert(this == f);
			assert(false);
			return;
		}
		value MY_LIBRARY estimate()const noexcept {
			return pow(this->base->estimate(), this->expo->estimate());
		}
		std::ostream& MY_LIBRARY Print(std::ostream& o)const noexcept {
			return o << '(' << *base << "^(" << *expo << "))";
		}
	private:
		function* base, * expo;
	};

	//function sin()
	class fsin :public function {
		using Q=LargeInteger::Q;
	public:
		MY_LIBRARY fsin(function* in)noexcept
			:inner(in) {
			ERR("正弦函数构造于" << this << ";值为" << *this << std::endl);
		}
		MY_LIBRARY ~fsin()noexcept {
			ERR("正弦函数析构于" << this << ";值为" << *this << std::endl);
			delete this->inner;
		}
		function* MY_LIBRARY copy()noexcept {
			auto&& temp = new fsin(this->inner->copy());
			ERR("复制了位于" << this << "的正弦级数;其值为" << *this << ";新正弦函数在" << temp << std::endl);
			return temp;
		}
		void MY_LIBRARY diff(function*& f) noexcept {
			assert(this == f);
			ERR("正弦函数求导于" << this << ";值为" << *this << std::endl);
			inner->diff(inner);
			function* temp = inner->copy();
			temp->diff(temp);
			f = new product<2>(temp, f);
			return;
		}
		void MY_LIBRARY integral(function*& f) noexcept {
			assert(this == f);
			assert(false);
			return;
		}
		value MY_LIBRARY estimate()const noexcept {
			return std::sin(this->inner->estimate());
		}
		std::ostream& MY_LIBRARY Print(std::ostream& o)const noexcept {
			return o << "sin(" << *inner << ')';
		}
	private:
		function* inner;
	};
}
#undef new