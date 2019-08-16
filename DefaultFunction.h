#pragma once

#include "PreciseMath.h"
#include <map>


#define new DBG_NEW
namespace Function {
	using value=double;
	enum class vari {
		DEF, x, y, z, w
	};
	INLINED std::ostream& MY_LIBRARY operator<<(std::ostream& o, vari var)noexcept {
		switch (var) {
		case Function::vari::DEF:
			assert(false);
			break;
		case Function::vari::x:
			o << 'x';
			break;
		case Function::vari::y:
			o << 'y';
			break;
		case Function::vari::z:
			o << 'z';
			break;
		case Function::vari::w:
			o << 'w';
			break;
		default:
			break;
		}
		return o;
	}
	std::map<vari, value> num_map = {};
	class function;
	template<vari> class num;
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

	template<vari var = vari::DEF>
	class num :public function {
	public:
		MY_LIBRARY num()noexcept = default;
		MY_LIBRARY ~num()noexcept = default;
		void MY_LIBRARY diff(function*& f) noexcept {
			assert(this == f);
		};
		void MY_LIBRARY integral(function*&) noexcept { };
		function* MY_LIBRARY copy()noexcept { return new num(); };
		value MY_LIBRARY estimate()const noexcept { 
			auto&& it = num_map.find(var);
			if (it != num_map.end()) {
				return (*it).second;
			}
			else {
				std::cout << var << " = " << std::endl;
				value v;
				std::cin >> v;
				num_map.insert(std::pair(var, v));
				return v;
			}
		}
		std::ostream& MY_LIBRARY Print(std::ostream& o) const noexcept { return o << estimate(); };
	};

	template<>
	class num<vari::DEF> :public function {
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
		}
		MY_LIBRARY ~sum() noexcept {
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
		MY_LIBRARY product(function* p)noexcept :p(p) { 
		}
		MY_LIBRARY ~product() noexcept { 
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
		}
		explicit MY_LIBRARY product(product* _p) noexcept 
			:p(_p->p), product<count - 1>(static_cast<product<count - 1>*>(_p)) { }
		MY_LIBRARY ~product() noexcept {
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
		}
		MY_LIBRARY ~f_ln()noexcept {
			if (inner != nullptr) {
				delete this->inner;
			}
		}
		function* MY_LIBRARY copy()noexcept {
			auto&& temp = new f_ln(this->inner->copy());
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
		}
		MY_LIBRARY ~f_power()noexcept {
			if (base != nullptr) {
				delete base;
			}
			if (expo != nullptr) {
				delete expo;
			}
		}
		function* MY_LIBRARY copy()noexcept {
			auto&& temp = new f_power(this->base->copy(), this->expo->copy());
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
		}
		MY_LIBRARY ~f_sin()noexcept {
			delete this->inner;
		}
		function* MY_LIBRARY copy()noexcept {
			auto&& temp = new f_sin(this->inner->copy());
			return temp;
		}
		void MY_LIBRARY diff(function*& f) noexcept {
			assert(this == f);
			inner = new sum<2>(inner, new num<vari::DEF>(1, 2));
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
		function *_base = base->copy(), *_expo = expo->copy();
		_base->diff(_base);
		_expo->diff(_expo);
		f = new product<2>(new sum<2>(new product<3>(base->copy(), _expo, new f_ln(base->copy())), new product<2>(_base, expo->copy())), f);
		return;
	}
	INLINED void MY_LIBRARY f_ln::diff(function*& f) noexcept {
		assert(this == f);
		f = new f_power(inner, new num<vari::DEF>(-1));
		inner = nullptr;
		delete this;
		return;
	}
}
#undef new