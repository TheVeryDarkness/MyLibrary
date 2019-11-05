#pragma once

#include <sstream>
#include "PreciseMath.h"
#include "Exception.h"
#include "CustomizedRadixCharSet.h"



inline namespace Function {
#define PI acos(-1)
	using value = double;
	class function;
	class constant;
	template<size_t count>class sum;
	template<size_t count>class product;
	class f_pow_x_ln_x;
	class f_pow_x;
	class f_sin;

	class function {
	public:
		MY_LIB function()noexcept { }
		MY_LIB function(const function &that)noexcept = delete;

		virtual MY_LIB ~function()noexcept { }

		virtual void MY_LIB diff(function *&) noexcept = 0;
		virtual bool MY_LIB integralable()noexcept = 0;
		virtual void MY_LIB undefinite_integral(function *&) noexcept = 0;
		virtual function *MY_LIB copy()noexcept = 0;
		virtual value MY_LIB estimate(const constant&)const noexcept = 0;
		virtual std::ostream &MY_LIB Print(std::ostream &) const noexcept = 0;
		friend std::ostream &MY_LIB operator<<(std::ostream &o, const function &fun)noexcept {
			return fun.Print(o);
		}
	};

	class constant final{
	public:
		MY_LIB constant(const std::string &str) noexcept :a(str.c_str()) { }
		MY_LIB constant(LargeInteger::Q &&q) noexcept :a(q) { }
		MY_LIB ~constant() noexcept { }
		constant MY_LIB copy()noexcept {
			return constant(a.Copy(a));
		}
		value MY_LIB estimate()const noexcept {
			return a.estim();
		}
		std::ostream &MY_LIB Print(std::ostream &o) const noexcept {
			return o << a;
		}
	private:
		LargeInteger::Q a;
	};

	template<size_t count = 2>
	class sum : public function {
		MY_LIB sum()noexcept = default;
		friend class product<count>;
	public:
		sum(function *pack...)noexcept :p{ pack... } {
			static_assert(sizeof...(pack) == count, "Parameter not matched");
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
		void MY_LIB undefinite_integral(function *&f) noexcept {
			assert(this == f);
			for (auto ptr : p) {
				ptr->undefinite_integral(ptr);
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

	template<size_t count = 2>
	class [[deprecated("Unfinished work")]] product :public function{
		MY_LIB product()noexcept = default;
	public:
		product(function * p...) noexcept :p{p...} { }
		MY_LIB ~product() noexcept {
			for (auto i : p) {
				if (i) {
					delete i;
				}
			}
		}
		std::ostream &MY_LIB Print(std::ostream & o)const noexcept {
			const function *const *const end = p + count;
			function *const *ptr = p;
			o << '(' << **ptr;
			for (++ptr; ptr != end; ++ptr) {
				o << " * " << *ptr;
			}
			o << ')';
			return o;
		}
		value MY_LIB estimate()const noexcept {
			value res = 0;
			for (auto ptr : p) {
				res *= ptr->estimate();
			}
			return res;
		}
		product *MY_LIB copy()noexcept {
			sum *res = new product;
			function *p1 = this->p[0], *p2 = res->p[0];
			for (size_t i = 0; i < count; ++i) {
				p2 = p1->copy();
				++p1, ++p2;
			}
			return res;
		}
		void MY_LIB diff(function * &f) noexcept {
			assert(this == f);

			return;
		}
	private:
		function *p[count];
	};

	class f_pow_x :public function {
	public:
		template<typename Val1, typename Val2>
		MY_LIB f_pow_x(Val1 &&coeff1, Val2 &&coeff2,  size_t expo) noexcept :
			coeff(true, std::move(coeff1), std::move(coeff2)),
			expo(expo) { }
		MY_LIB f_pow_x(LargeInteger::Q &&coeff, LargeInteger::N &&expo) noexcept
			:coeff(coeff), expo(expo) { }
		MY_LIB ~f_pow_x()noexcept {
			coeff.destruct();
			expo.destruct();
		}
		void MY_LIB diff(function *&) noexcept {
			this->coeff *= this->expo;
			if (this->expo != 0) {
				this->expo -= 1;
			}
		}
		bool MY_LIB integralable()noexcept override {
			return true;
		}
		void MY_LIB undefinite_integral(function *&) noexcept override{
			this->expo += 1;
			this->coeff /= this->expo;
		}
		f_pow_x *MY_LIB copy()noexcept override{
			return new f_pow_x(coeff.Copy(coeff), expo.Copy(expo));
		}
		value MY_LIB estimate(const constant& con)const noexcept {
			return coeff.estim() * std::pow(con.estimate(), expo.GetValue<value>());
		}
		std::ostream &MY_LIB Print(std::ostream &o) const noexcept {
			return (coeff == 0 ? o << "0" : o << coeff << " * x" << "^(" << expo << ")");
		}
	private:
		LargeInteger::Q coeff;
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
		function *MY_LIB copy() noexcept override {
			auto &&temp = new f_pow_x_ln_x(this->pow->copy());
			return temp;
		}
		INLINED void MY_LIB diff(function *&f) noexcept {
			assert(this == f);
			//f = new sum<2>(new f_pow_x(pow));
			delete this;
			return;
		}
		bool MY_LIB integralable()noexcept {
			return true;
		}
		void MY_LIB undefinite_integral(function *&f) noexcept override {
			assert(this == f);
			assert(false);
			return;
		}
		value MY_LIB estimate(const constant &con)const noexcept override {
			return std::log(this->pow->estimate(con));
		}
		std::ostream &MY_LIB Print(std::ostream &o)const noexcept {
			return o << "ln(" << *pow << ')';
		}
	private:
		f_pow_x *pow;
	};


	class ptrHolder {
	private:
	public:
		MY_LIB ptrHolder(function *f)noexcept :func_ptr(f) { }
		MY_LIB ptrHolder(ptrHolder &&rv)noexcept :func_ptr(rv.func_ptr) { rv.func_ptr = nullptr; }
		MY_LIB ptrHolder(const ptrHolder &rvalue)noexcept = delete;
		MY_LIB ~ptrHolder() { if (func_ptr)delete func_ptr; }
		void MY_LIB diff()noexcept {
			if (this->func_ptr) this->func_ptr->diff(this->func_ptr);
		}
		void MY_LIB undefinite_integral()noexcept {
			if (this->func_ptr) {
				assert(this->func_ptr->integralable());
				this->func_ptr->undefinite_integral(this->func_ptr);
			}
		}
		friend std::ostream &MY_LIB operator<<(std::ostream &o, ptrHolder holder)noexcept {
			if (holder.func_ptr) holder.func_ptr->Print(o); else o << '0';
			return o;
		}
	private:
		function *func_ptr;
	};

	class funEngine {
	public:
		funEngine() { }
		~funEngine() { }
		static ptrHolder produce(const std::string &definition) {
			std::istringstream sin(definition);
			LargeInteger::ignore_if<' ', 'x', 'y', 'z'>(sin);
			std::string input;
			LargeInteger::getline<' '>(sin, input);
			if (input == "t") {
				return ptrHolder(new f_pow_x(1, 2, 2));
			}
			throw UnknownFunction("This function is not supported yet.", definition);
		}
	private:

	};

}
#undef new