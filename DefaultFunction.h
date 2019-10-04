#pragma once

#include "PreciseMath.h"
#include <map>


//#define new DBG_NEW
namespace Function {
	using value=double;
	enum class vari {
		DEF, x, y, z, w
	};
	enum class gener {
		i, j, k, l
	};
	std::map<vari, value> num_map = {};
	

	class function;
	template<vari> class num;
	template<size_t n>class self_increase;
	template<size_t n>class partial_sum;
	template<size_t count>class sum;
	template<size_t count>class product;
	class f_ln;
	class f_power;
	class f_sin;

	#define PI acos(-1)

	class function {
	public:
		MY_LIB function()noexcept { };
		MY_LIB function(const function& that)noexcept = delete;

		virtual MY_LIB ~function()noexcept { }

		virtual void MY_LIB diff(function*&) noexcept = 0;
		virtual void MY_LIB integral(function*&) noexcept = 0;
		virtual function* MY_LIB copy()noexcept = 0;
		virtual value MY_LIB estimate()const noexcept = 0;
		virtual std::ostream& MY_LIB Print(std::ostream& o) const noexcept = 0;
		friend std::ostream& MY_LIB operator<<(std::ostream& o, const function& fun)noexcept {
			return fun.Print(o);
		}
	private:

	};

	template<vari var = vari::DEF>
	class num :public function {
	public:
		MY_LIB num()noexcept = default;
		MY_LIB ~num()noexcept = default;
		void MY_LIB diff(function*& f) noexcept {
			assert(this == f);
		};
		void MY_LIB integral(function*&) noexcept { };
		function* MY_LIB copy()noexcept { return new num(); };
		value MY_LIB estimate()const noexcept { 
			auto&& it = num_map.find(var);
			if (it != num_map.end()) {
				return (*it).second;
			}
			else {
				std::cout << var << " = " << std::flush;
				value v;
				std::cin >> v;
				num_map.insert(std::pair(var, v));
				return v;
			}
		}
		std::ostream& MY_LIB Print(std::ostream& o) const noexcept { return o << var; };
	};

	template<>
	class num<vari::DEF> :public function {
	public:
		template<typename Val>
		MY_LIB num(bool sign, const Val &&val)noexcept :q(sign, std::move(val), 1) { }
		template<typename Val>
		MY_LIB num(bool sign, Val val1, Val val2)noexcept :q(sign, val1, val2) { }
		MY_LIB num(const LargeInteger::Q& val)noexcept :q(val) { }

		MY_LIB ~num()noexcept {
			q.destruct();
		}

		void MY_LIB diff(function*& f) noexcept {
			assert(this == f);
			q = 0;
		};
		void MY_LIB integral(function*&) noexcept { };
		function* MY_LIB copy()noexcept { return new num(LargeInteger::Q::Copy(q)); };
		value MY_LIB estimate()const noexcept { return this->q.estim<value>(); }
		std::ostream& MY_LIB Print(std::ostream& o) const noexcept { return q.Print(o); };

	private:
		LargeInteger::Q q;
	};

	template<size_t n>
	class self_increase :public function{
	public:
		MY_LIB self_increase(size_t _i = 1)noexcept :i(new size_t(_i)) { }
		MY_LIB ~self_increase()noexcept { delete i; }

		void MY_LIB diff(function*& f) noexcept {
			assert(this == f);
			f = new num<vari::DEF>(0);
			delete this;
		};
		void MY_LIB integral(function*& f) noexcept {
			assert(this == f);
			f = new sum<2>(this, new num<vari::x>());
		};
		function* MY_LIB copy()noexcept { return new self_increase(*i); };
		value MY_LIB estimate() const noexcept {
			return value((*i == n) ? (*i = 1) : ((*i)++));
		}
		std::ostream& MY_LIB Print(std::ostream& o) const noexcept {
			return o << 'i';
		};
	private:
		size_t* i;
	};

	template<size_t n>
	class partial_sum :public function{
	public:
		MY_LIB partial_sum(function* _p)noexcept:p(_p) { }
		MY_LIB ~partial_sum()noexcept {
			delete p;
		}

		void MY_LIB diff(function*& f) noexcept {
			assert(this == f);
			p->diff(p);
		};
		void MY_LIB integral(function*& f) noexcept { 
			assert(this == f);
			p->integral(p);
		};
		function* MY_LIB copy()noexcept { return new partial_sum(p->copy()); };
		value MY_LIB estimate()const noexcept { 
			value s = 0;
			for (size_t i = 0; i < n; i++) {
				s += this->p->estimate();
			}
			return s; 
		}
		std::ostream& MY_LIB Print(std::ostream& o) const noexcept { 
			return p->Print(o << "(��(1:1:" << n << ')') << ')';
		};
	private:
		function* p;
	};


	template<>
	class sum<1> :public function {
	public:
		MY_LIB sum(function* p)noexcept :p(p) { }
		MY_LIB ~sum() noexcept { delete p; }
		sum* MY_LIB copy()noexcept { return new sum(p->copy()); }
		std::ostream& MY_LIB Print(std::ostream& o)const noexcept {
			return o << *p << ")";
		}
		void MY_LIB integral(function*& f) noexcept {
			assert(this == f);
			assert(false);
			return;
		}
		void MY_LIB diff(function*& f) noexcept {
			assert(this == f);
			f->diff(f);
			return;
		}
		value MY_LIB estimate()const noexcept {
			return this->p->estimate();
		}
		std::ostream& MY_LIB _Print(std::ostream& o)const noexcept {
			return o << *p << ')';
		}
	private:
		function* p;
	};

	template<size_t count>
	class sum :public sum<count - 1>/*, public function */ {
	public:
		template<typename ...pack>
		MY_LIB sum(function* p, pack... _p) noexcept :p(p), sum<count - 1>(_p...) {
		}
		MY_LIB ~sum() noexcept {
			delete p;
		}
		sum* MY_LIB copy()noexcept { return new sum(p->copy(), this->sum<count - 1>::copy()); }
		void MY_LIB integral(function*& f) noexcept {
			assert(this == f);
			p->integral(p);
			return;
		}
		void MY_LIB diff(function*& f) noexcept {
			assert(this == f);
			p->diff(p);
			return;
		}
		value MY_LIB estimate()const noexcept {
			return this->p->estimate() + this->sum<count - 1>::estimate();
		}
		std::ostream& MY_LIB _Print(std::ostream& o)const noexcept {
			o << *p << " + ";
			return static_cast<const sum<count - 1>*>(this)->_Print(o);
		}
		std::ostream& MY_LIB Print(std::ostream& o)const noexcept {
			return this->_Print(o << '(');
		}
	private:
		function* p;
	};

	template<>
	class product<1> :public function {
	public:
		MY_LIB product(function* p)noexcept :p(p) { 
		}
		MY_LIB ~product() noexcept { 
			delete p; 
		}
		product* MY_LIB copy()noexcept { return new product(p->copy()); }
		std::ostream& MY_LIB Print(std::ostream& o)const noexcept {
			return o << *p;
		}
		void MY_LIB integral(function*& f) noexcept {
			assert(this == f);
			assert(false);
			return;
		}
		void MY_LIB diff(function*& f) noexcept {
			assert(this == f);
			p->diff(p);
			return;
		}
		value MY_LIB estimate()const noexcept {
			return this->p->estimate();
		}
		std::ostream& MY_LIB _Print(std::ostream& o)const noexcept {
			return o << *p << ')';
		}
	private:
		function* p;
	};

	template<size_t count = 2>
	class product :public product<count - 1>/*, public function */ {
	public:
		template<typename ...pack> MY_LIB product(function* p, pack... _p) noexcept :p(p), product<count - 1>(_p...) {
		}
		explicit MY_LIB product(product* _p) noexcept 
			:p(_p->p), product<count - 1>(static_cast<product<count - 1>*>(_p)) { }
		MY_LIB ~product() noexcept {
			if (p != nullptr) {
				delete p;
			}
		}
		std::ostream& MY_LIB _Print(std::ostream& o)const noexcept {
			o << *p << " * ";
			return static_cast<const product<count - 1>*>(this)->_Print(o);
		}
		std::ostream& MY_LIB Print(std::ostream& o)const noexcept {
			return this->_Print(o << '(');
		}
		value MY_LIB estimate()const noexcept {
			return this->p->estimate() * this->product<count - 1>::estimate();
		}
		product<count>* MY_LIB copy()noexcept {
			return new product(p->copy(), product<count - 1>::copy());
		}
		void MY_LIB integral(function*& f) noexcept {
			assert(this == f);
			assert(false);
			return;
		}
		void MY_LIB diff(function*& f) noexcept {
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
		MY_LIB f_ln(function* in)noexcept :inner(in) {
		}
		MY_LIB ~f_ln()noexcept {
			if (inner != nullptr) {
				delete this->inner;
			}
		}
		function* MY_LIB copy()noexcept {
			auto&& temp = new f_ln(this->inner->copy());
			return temp;
		}
		void MY_LIB diff(function*& f) noexcept;
		void MY_LIB integral(function*& f) noexcept {
			assert(this == f);
			assert(false);
			return;
		}
		value MY_LIB estimate()const noexcept {
			return std::log(this->inner->estimate());
		}
		std::ostream& MY_LIB Print(std::ostream& o)const noexcept {
			return o << "ln(" << *inner << ')';
		}
	private:
		function* inner;

	};

	class f_power :public function {
		using Q=LargeInteger::Q;
	public:
		MY_LIB f_power(function* base, function* expo) noexcept
			:base(base), expo(expo) {
		}
		MY_LIB ~f_power()noexcept {
			if (base != nullptr) {
				delete base;
			}
			if (expo != nullptr) {
				delete expo;
			}
		}
		function* MY_LIB copy()noexcept {
			auto&& temp = new f_power(this->base->copy(), this->expo->copy());
			return temp;
		}
		void MY_LIB diff(function*& f) noexcept;
		void MY_LIB integral(function*& f) noexcept {
			assert(this == f);
			assert(false);
			return;
		}
		value MY_LIB estimate()const noexcept {
			return pow(this->base->estimate(), this->expo->estimate());
		}
		std::ostream& MY_LIB Print(std::ostream& o)const noexcept {
			return o << '(' << *base << "^(" << *expo << "))";
		}
	private:
		function* base, * expo;
	};

	//function sin()
	class f_sin :public function {
		using Q=LargeInteger::Q;
	public:
		MY_LIB f_sin(function* in)noexcept
			:inner(in) {
		}
		MY_LIB ~f_sin()noexcept {
			delete this->inner;
		}
		function* MY_LIB copy()noexcept {
			auto&& temp = new f_sin(this->inner->copy());
			return temp;
		}
		void MY_LIB diff(function*& f) noexcept {
			assert(this == f);
			inner = new sum<2>(inner, new num<vari::DEF>(1, 2));
			function* temp = inner->copy();
			temp->diff(temp);
			f = new product<2>(temp, f);
			return;
		}
		void MY_LIB integral(function*& f) noexcept {
			assert(this == f);
			assert(false);
			return;
		}
		value MY_LIB estimate()const noexcept {
			return std::sin(PI * this->inner->estimate());
		}
		std::ostream& MY_LIB Print(std::ostream& o)const noexcept {
			return o << "sin(pi * " << *inner << ')';
		}
	private:
		function* inner;
	};

	INLINED void MY_LIB f_power::diff(function*& f) noexcept {
		assert(this == f);
		function *_base = base->copy(), *_expo = expo->copy();
		_base->diff(_base);
		_expo->diff(_expo);
		f = new product<2>(new sum<2>(new product<3>(base->copy(), _expo, new f_ln(base->copy())), new product<2>(_base, expo->copy())), f);
		return;
	}
	INLINED void MY_LIB f_ln::diff(function*& f) noexcept {
		assert(this == f);
		f = new f_power(inner, new num<vari::DEF>(false, 1));
		inner = nullptr;
		delete this;
		return;
	}
	INLINED std::ostream& MY_LIB operator<<(std::ostream& o, gener var)noexcept {
		switch (var) {
		case Function::gener::i:
			o << 'i';
			break;
		case Function::gener::j:
			o << 'j';
			break;
		case Function::gener::k:
			o << 'k';
			break;
		case Function::gener::l:
			o << 'l';
			break;
		default:
			assert(false);
			break;
		}
	}
	INLINED std::ostream& MY_LIB operator<<(std::ostream& o, vari var)noexcept {
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
			assert(false);
			break;
		}
		return o;
	}
}
#undef new