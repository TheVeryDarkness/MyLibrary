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
	class f_ln;
	class f_power;
	class f_sin;

	#define PI acos(-1)

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
		template<typename Val>
		MY_LIBRARY num(Val val1, Val val2)noexcept :q(val1, val2) { }
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
	class sum :public sum<count - 1>/*, public function */ {
	public:
		template<typename ...pack>
		MY_LIBRARY sum(function* p, pack... _p) noexcept :p(p), sum<count - 1>(_p...) {
			ERR(count << "-level sum constructs on " << this << ", with a member on " << p << "." << std::endl);
		}
		MY_LIBRARY ~sum() noexcept {
			ERR(count << "-level sum destructs on " << this << ", with a member on " << p << "." << std::endl);
			delete p;
		}
		sum* MY_LIBRARY copy()noexcept { return new sum(p->copy(), this->sum<count - 1>::copy()); }
		void MY_LIBRARY integral(function*& f) noexcept {
			assert(this == f);
			p->integral(p);
			return;
		}
		void MY_LIBRARY diff(function*& f) noexcept {
			ERR(count << "-level sum diffs on " << this << ", with a member on " << p << "." << std::endl);
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
		MY_LIBRARY product(function* p)noexcept :p(p) { 
			ERR(1 << "-level product constructs on ") << this << ", with a member on " << p << "." << std::endl;
		}
		MY_LIBRARY ~product() noexcept { 
			ERR(1 << "-level product constructs on ") << this << ", with a member on " << p << "." << std::endl; 
			delete p; 
		}
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
	class product :public product<count - 1>/*, public function */ {
	public:
		template<typename ...pack> MY_LIBRARY product(function* p, pack... _p) noexcept :p(p), product<count - 1>(_p...) {
			ERR(count << "-level product constructs" << this << ", with a member on " << p << "." << std::endl);
		}
		explicit MY_LIBRARY product(product* _p) noexcept :p(_p->p), product<count - 1>(static_cast<product<count - 1>*>(_p)) { }
		MY_LIBRARY ~product() noexcept {
			ERR(count << "-level product denstructs" << this << ", with a member on " << p << "." << std::endl);
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
		product<count>* MY_LIBRARY copy()noexcept {
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

	class f_ln :public function {
	public:
		MY_LIBRARY f_ln(function* in)noexcept :inner(in) {
			ERR("f_ln constructs on " << this << ", with a member on " << inner << "." << std::endl);
		}
		MY_LIBRARY ~f_ln()noexcept {
			ERR("f_ln destructs on " << this << ", with a member on " << inner << "." << std::endl);
			delete this->inner;
		}
		function* MY_LIBRARY copy()noexcept {
			auto&& temp = new f_ln(this->inner->copy());
			ERR("a f_ln on " << this << "has been copied. it has a value of " << *this << ". new f_ln is on " << temp << std::endl);
			return temp;
		}
		void MY_LIBRARY diff(function*& f) noexcept;
		void MY_LIBRARY integral(function*& f) noexcept {
			assert(this == f);
			assert(false);
			return;
		}
		value MY_LIBRARY estimate()const noexcept {
			return std::log(this->inner->estimate());
		}
		std::ostream& MY_LIBRARY Print(std::ostream& o)const noexcept {
			return o << "ln(" << *inner << ')';
		}
	private:
		function* inner;

	};

	class f_power :public function {
		using Q=LargeInteger::Q;
	public:
		MY_LIBRARY f_power(function* base, function* expo) noexcept
			:base(base), expo(expo) {
			ERR("f_power destructs on " << this << " with two members on " << base << "and" << expo << std::endl);
		}
		MY_LIBRARY ~f_power()noexcept {
			ERR("f_power destructs on " << this << " with two members on " << base << "and" << expo << std::endl);
		}
		function* MY_LIBRARY copy()noexcept {
			auto&& temp = new f_power(this->base->copy(), this->expo->copy());
			ERR("a f_power on " << this << "has been copied. it has a value of " << *this << ". new f_power is on " << temp << std::endl);
			return temp;
		}
		void MY_LIBRARY diff(function*& f) noexcept;
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
	class f_sin :public function {
		using Q=LargeInteger::Q;
	public:
		MY_LIBRARY f_sin(function* in)noexcept
			:inner(in) {
			ERR("f_sin constructs on " << this << ", with a member on " << inner << "." << std::endl);
		}
		MY_LIBRARY ~f_sin()noexcept {
			ERR("f_sin destructs on " << this << ", with a member on " << inner << "." << std::endl);
			delete this->inner;
		}
		function* MY_LIBRARY copy()noexcept {
			auto&& temp = new f_sin(this->inner->copy());
			ERR("a f_sin on " << this << "has been copied. it has a value of " << *this << ". new f_sin is on " << temp << std::endl);
			return temp;
		}
		void MY_LIBRARY diff(function*& f) noexcept {
			assert(this == f);
			ERR("a f_sin on " << this << "diffs. its origin value is" << *this << std::endl);
			inner = new sum<2>(inner, new num(1, 2));
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
			return std::sin(PI * this->inner->estimate());
		}
		std::ostream& MY_LIBRARY Print(std::ostream& o)const noexcept {
			return o << "sin(pi * " << *inner << ')';
		}
	private:
		function* inner;
	};

	INLINED void MY_LIBRARY f_power::diff(function*& f) noexcept {
		assert(this == f);
		ERR("f_power on " << this << "diff. it has a value of " << *this << std::endl);
		function *_base = base->copy(), *_expo = expo->copy();
		_base->diff(_base);
		_expo->diff(_expo);
		f = new product<2>(new sum<2>(new product<3>(base, _expo, new f_ln(base->copy())), new product<2>(_base, expo)), f);
		base = nullptr;
		expo = nullptr;
		delete this;
		return;
	}
	INLINED void MY_LIBRARY f_ln::diff(function*& f) noexcept {
		assert(this == f);
		ERR("a f_ln on " << this << "diffs. its origin value is" << *this << std::endl);
		f = new f_power(inner, new num(-1));
		delete this;
		return;
	}
}
#undef new