#pragma once

#include "PreciseMath.h"
#include <map>


//#define new DBG_NEW
namespace Function {
	enum class vari {
		DEF, x, y, z, w
	};
	//I've fogotten what I use them for
	enum class gener {
		i, j, k, l
	};

#define PI acos(-1)

	class ptrHolder {
	private:
		using value = double;
		std::map<vari, value> num_map = {};
		class function;
		class Integralable;
		class constant;
	public:
		class num;
		template<size_t count>class sum;
		template<size_t count>class product;
		class f_pow_x_ln_x;
		class f_pow_x;
		class f_sin;
	private:

		class function {
		public:
			MY_LIB function()noexcept { }
			MY_LIB function(const function &that)noexcept = delete;

			virtual MY_LIB ~function()noexcept { }

			virtual void MY_LIB diff(function *&) noexcept = 0;
			virtual function *MY_LIB copy()noexcept = 0;
			virtual value MY_LIB estimate()const noexcept = 0;
			virtual std::ostream &MY_LIB Print(std::ostream &) const noexcept = 0;
			friend std::ostream &MY_LIB operator<<(std::ostream &o, const function &fun)noexcept {
				return fun.Print(o);
			}
		};
		class Integralable:public function {
		public:
			Integralable() { }
			~Integralable() { }
			virtual void MY_LIB definite_integral(function *&, constant* begin, constant* end) noexcept = 0;
		private:

		};

		class constant :public Integralable {
		public:
			MY_LIB constant() noexcept { }
			virtual MY_LIB ~constant() noexcept { }
			virtual void MY_LIB diff(function *&f) noexcept override { f = nullptr; delete this; }
			virtual void MY_LIB definite_integral(function *&, constant *begin, constant *end) noexcept override = 0;
			virtual constant *MY_LIB copy()noexcept = 0;
			virtual value MY_LIB estimate()const noexcept = 0;
			virtual std::ostream &MY_LIB Print(std::ostream &) const noexcept = 0;
		};
	public:
		class num :public constant {
		public:
			template<typename Val>
			MY_LIB num(bool sign, const Val &&val)noexcept :q(sign, std::move(val), 1) { }
			template<typename Val>
			MY_LIB num(bool sign, Val val1, Val val2)noexcept :q(sign, val1, val2) { }
			MY_LIB num(const LargeInteger::Q &val)noexcept :q(val) { }
			MY_LIB ~num()noexcept {
				q.destruct();
			}
			void MY_LIB diff(function *&f) noexcept override {
				assert(this == f);
				q = 0;
			};
			void MY_LIB definite_integral(function *&, constant *begin, constant *end) noexcept override { 

			};
			constant *MY_LIB copy()noexcept { return new num(LargeInteger::Q::Copy(q)); };
			value MY_LIB estimate()const noexcept { return this->q.estim<value>(); }
			std::ostream &MY_LIB Print(std::ostream &o) const noexcept { return q.Print(o); };

		private:
			LargeInteger::Q q;
		};

		template<size_t count = 2>
		class sum : public function {
			MY_LIB sum()noexcept = default;
			friend class product<count>;
		public:
			template<typename ...Pack>MY_LIB sum(Pack...pack)noexcept :p{ pack... } {
				static_assert(sizeof...(Pack) == count, "Parameter not matched");
			}
			MY_LIB sum(std::initializer_list<function *>p) noexcept :p(p) { }
			MY_LIB ~sum() noexcept {
				for (auto ptr : p) {
					if (ptr) {
						delete ptr;
						//ptr = nullptr;
					}
				}
			}
			sum *MY_LIB copy()noexcept {
				sum *res = new sum;
				function *p1 = this->p[0], *p2 = res->p[0];
				for (size_t i = 0; i < count; ++i) {
					p2 = p1->copy();
					++p1, ++p2;
				}
				return res;
			}
			void MY_LIB definite_integral(function *&f) noexcept {
				assert(this == f);
				for (auto ptr : p) {
					ptr->definite_integral(ptr);
				}
				return;
			}
			void MY_LIB diff(function *&f) noexcept {
				assert(this == f);
				for (auto ptr : p) {
					ptr->diff(ptr);
				}
				return;
			}
			value MY_LIB estimate()const noexcept {
				value res = 0;
				for (auto ptr : p) {
					res += ptr->estimate();
				}
				return res;
			}
			std::ostream &MY_LIB Print(std::ostream &o)const noexcept {
				const function *const *const end = p + count;
				function *const *ptr = p;
				o << '(' << **ptr;
				for (++ptr; ptr != end; ++ptr) {
					o << " + " << *ptr;
				}
				o << ')';
				return o;
			}
		private:
			function *p[count];
		};

		template<>
		class product<1> :public function {
		public:
			MY_LIB product(function *p)noexcept :p(p) { }
			MY_LIB ~product() noexcept {
				delete p;
			}
			product *MY_LIB copy()noexcept { return new product(p->copy()); }
			std::ostream &MY_LIB Print(std::ostream &o)const noexcept {
				return o << *p;
			}
			void MY_LIB definite_integral(function *&f) noexcept {
				assert(this == f);
				assert(false);
				return;
			}
			void MY_LIB diff(function *&f) noexcept {
				assert(this == f);
				p->diff(p);
				return;
			}
			value MY_LIB estimate()const noexcept {
				return this->p->estimate();
			}
			std::ostream &MY_LIB _Print(std::ostream &o)const noexcept {
				return o << *p << ')';
			}
		private:
			function *p;
		};

		template<size_t count = 2>
		class product :public product<count - 1>/*, public function */ {
		public:
			template<typename ...pack> MY_LIB product(function *p, pack... _p) noexcept :p(p), product<count - 1>(_p...) { }
			explicit MY_LIB product(product *_p) noexcept
				:p(_p->p), product<count - 1>(static_cast<product<count - 1> *>(_p)) { }
			MY_LIB ~product() noexcept {
				if (p != nullptr) {
					delete p;
				}
			}
			std::ostream &MY_LIB _Print(std::ostream &o)const noexcept {
				o << *p << " * ";
				return static_cast<const product<count - 1> *>(this)->_Print(o);
			}
			std::ostream &MY_LIB Print(std::ostream &o)const noexcept {
				return this->_Print(o << '(');
			}
			value MY_LIB estimate()const noexcept {
				return this->p->estimate() * this->product<count - 1>::estimate();
			}
			product<count> *MY_LIB copy()noexcept {
				return new product(p->copy(), product<count - 1>::copy());
			}
			void MY_LIB definite_integral(function *&f) noexcept {
				assert(this == f);
				assert(false);
				return;
			}
			void MY_LIB diff(function *&f) noexcept {
				assert(this == f);
				auto _p = p->copy();
				_p->diff(_p);
				function *_this = this->product<count - 1>::copy();
				_this->diff(_this);
				f = new sum<2>(new product<2>(_p, this->product<count - 1>::copy()), new product<2>(p, _this));
				p = nullptr;
				delete this;
				return;
			}
		private:
			function *p;
		};

		class f_pow_x :public function {
		public:
			template<typename Val1, typename Val2, typename Val3, typename Val4>
			MY_LIB f_pow_x(Val1 &&coeff1, Val2 &&coeff2, Val3 &&base1, Val4 &&base2, size_t expo) noexcept :
				coeff(true, std::move(coeff1), std::move(coeff2)),
				base(true, std::move(base1), std::move(base2)),
				expo(expo) { }
			MY_LIB f_pow_x(LargeInteger::Q &&coeff, LargeInteger::Q &&base, LargeInteger::N &&expo) noexcept
				:coeff(coeff), base(base), expo(expo) { }
			MY_LIB ~f_pow_x()noexcept {
				coeff.destruct();
				base.destruct();
				expo.destruct();
			}
			void MY_LIB diff(function *&) noexcept {
				this->coeff *= this->expo;
				if (this->expo != 0) {
					this->expo -= 1;
				}
			}
			void MY_LIB definite_integral(function *&) noexcept {
				this->expo += 1;
				this->coeff /= this->expo;
			}
			f_pow_x *MY_LIB copy()noexcept {
				return new f_pow_x(coeff.Copy(coeff), base.Copy(base), expo.Copy(expo));
			}
			value MY_LIB estimate()const noexcept {
				return coeff.estim() * std::pow(base.estim(), expo.GetValue<value>());
			}
			std::ostream &MY_LIB Print(std::ostream &o) const noexcept {
				return (coeff == 0 ? o << "0" : o << coeff << "*" << "(" << base << ")^(" << expo << ")");
			}
		private:
			LargeInteger::Q coeff, base;
			LargeInteger::N expo;
		};


		class f_pow_x_ln_x :public function {
		public:
			template<typename...pack>MY_LIB f_pow_x_ln_x(pack...in)noexcept :pow(in...) { }
			MY_LIB ~f_pow_x_ln_x()noexcept {
				if (pow != nullptr) {
					delete this->pow;
				}
			}
			function *MY_LIB copy()noexcept {
				auto &&temp = new f_pow_x_ln_x(this->pow->copy());
				return temp;
			}
			INLINED void MY_LIB diff(function *&f) noexcept {
				assert(this == p);
				f = new f_power(f, new num(false, 1));
				f = nullptr;
				delete this;
				return;
			}
			void MY_LIB definite_integral(function *&f) noexcept {
				assert(this == f);
				assert(false);
				return;
			}
			value MY_LIB estimate()const noexcept {
				return std::log(this->pow->estimate());
			}
			std::ostream &MY_LIB Print(std::ostream &o)const noexcept {
				return o << "ln(" << *pow << ')';
			}
		private:
			f_pow_x* pow;
		};

		//function sin()
		class f_sin :public Integralable {
			using Q = LargeInteger::Q;
		public:
			MY_LIB f_sin(function *in)noexcept
				:inner(in) { }
			MY_LIB ~f_sin()noexcept {
				delete this->inner;
			}
			function *MY_LIB copy()noexcept {
				auto &&temp = new f_sin(this->inner->copy());
				return temp;
			}
			void MY_LIB diff(function *&f) noexcept {
				assert(this == f);
				inner = new sum<2>(inner, new num(1, 2));
				function *temp = inner->copy();
				temp->diff(temp);
				f = new product<2>(temp, f);
				return;
			}
			void MY_LIB definite_integral(function *&f, constant* begin, constant* end) noexcept override{
				assert(this == f);
				assert(false);
				return;
			}
			value MY_LIB estimate()const noexcept {
				return std::sin(PI * this->inner->estimate());
			}
			std::ostream &MY_LIB Print(std::ostream &o)const noexcept {
				return o << "sin(pi * " << *inner << ')';
			}
		private:
			function *inner;
		};
	public:
		MY_LIB ptrHolder(function* f)noexcept:func_ptr(f) { }
		MY_LIB ~ptrHolder() { if (func_ptr)delete func_ptr; }
		void MY_LIB diff()noexcept {
			if (this->func_ptr) this->func_ptr->diff(this->func_ptr);
		}
		void MY_LIB definite_integral(constant *begin, constant *end)noexcept {
			if (this->func_ptr) dynamic_cast<Integralable *>(this->func_ptr)->definite_integral(this->func_ptr, begin, end);
		}
		friend std::ostream &MY_LIB operator<<(std::ostream &o, ptrHolder holder)noexcept {
			if (holder.func_ptr) holder.func_ptr->Print(o); else o << '0';
			return o;
		}
	private:
		function *func_ptr;
	};

	INLINED std::ostream &MY_LIB operator<<(std::ostream &o, gener var)noexcept {
		switch (var) {
		case gener::i:
			o << 'i';
			break;
		case gener::j:
			o << 'j';
			break;
		case gener::k:
			o << 'k';
			break;
		case gener::l:
			o << 'l';
			break;
		default:
			assert(false);
			break;
		}
	}
	INLINED std::ostream &MY_LIB operator<<(std::ostream &o, vari var)noexcept {
		switch (var) {
		case vari::DEF:
			assert(false);
			break;
		case vari::x:
			o << 'x';
			break;
		case vari::y:
			o << 'y';
			break;
		case vari::z:
			o << 'z';
			break;
		case vari::w:
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