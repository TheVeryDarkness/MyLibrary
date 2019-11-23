#pragma once

#include "PreciseMath.h"
#include "Exception.h"
#include "CustomizedRadixCharSet.h"
#include <sstream>
#include <cmath>
#include <stack>
#include <queue>



namespace Darkness {
#define PI acos(-1)
	using rough_value = double;
	inline namespace Constant {
		class constant final {
		public:
			constant()noexcept :a(true, 0, 1) { };
			template<typename Int>constant(Int i)noexcept :a(i >= 0, ABS(i), 1) {
				static_assert(std::is_integral_v<Int>, "Integral type required.");
			};
			MY_LIB constant(const char *str) noexcept :a(str) { }
			MY_LIB constant(const char *begin, const char *end) noexcept :a(begin, end) { }
			template<typename val1,typename val2>
			explicit MY_LIB constant(bool sign, const val1 &&v1, const val2 &&v2) noexcept 
				:a(sign, std::move(v1), std::move(v2)) { }
			constant &MY_LIB operator=(const constant &that) {
				//this->a.destruct();
				this->a = that.a.Copy(that.a);
				return *this;
			}
			template<typename Int>constant &MY_LIB operator=(Int data) {
				this->a = data;
				return *this;
			}
			bool MY_LIB operator==(const constant &that)const noexcept { return this->a == that.a; }
			bool MY_LIB operator!=(const constant &that)const noexcept { return this->a != that.a; }
			template<typename Int>bool MY_LIB operator==(const Int &that)const noexcept { return this->a == that; }
			template<typename Int>bool MY_LIB operator!=(const Int &that)const noexcept { return this->a != that; }


			MY_LIB constant(const constant &that) : a(std::move(that.a.Copy(that.a))) { }
			MY_LIB constant(constant &&that) noexcept : a(std::move(that.a)) { }
			MY_LIB constant(const LargeInteger::Q &q) noexcept :a(std::move(q.Copy(q))) { }
			MY_LIB constant(LargeInteger::Q &&q) noexcept :a(std::move(q)) { }
			MY_LIB ~constant() noexcept { /*a.destruct();*/ }
			constant MY_LIB copy() {
				return constant(a.Copy(a));
			}
			rough_value MY_LIB estimate()const noexcept {
				return a.estim();
			}
			friend std::ostream &MY_LIB operator<<(std::ostream &o, const constant &con)noexcept {
				return o << con.a;
			}

			constant MY_LIB operator*(const constant& that)const {
				return constant(std::move(this->a * that.a));
			}
			constant& MY_LIB operator*=(const constant &that) {
				this->a *= that.a;
				return *this;
			}
			constant MY_LIB operator+(const constant& that)const {
				return constant(std::move(this->a + that.a));
			}
			constant& MY_LIB operator+=(const constant& that) {
				this->a += that.a;
				return *this;
			}
			constant MY_LIB operator-(const constant& that)const {
				return constant(std::move(this->a - that.a));
			}
			constant& MY_LIB operator-=(const constant& that) {
				this->a -= that.a;
				return *this;
			}
			friend constant MY_LIB operator-(constant&& that) {
				return constant(-std::move(that.a));
			}
			constant MY_LIB operator-()const {
				return constant(-std::move(this->a.Copy(a)));
			}
			constant MY_LIB operator/(const constant &that)const noexcept {
				return constant(std::move(this->a / that.a));
			}
			constant &MY_LIB operator/=(const constant &that)noexcept {
				this->a /= that.a;
				return *this;
			}
			constant& MY_LIB operator^=(const constant &times) {
				return this->a.pow(times.a), *this;
			}
			constant MY_LIB operator^(const constant &times) {
				constant tmp(*this); 
				tmp.a.pow(times.a);
				return std::move(tmp);
			}
		private:
			Q a;
		};
		constant INLINED MY_LIB sind(size_t degree) {
			degree %= 180;
			if (degree > 90)degree = 180 - degree;
			switch (degree) {
			case 0:
				return constant(true, 0, 1);
			case 30:
				return constant(true, 1, 2);
			case 37:
				return constant(true, 3, 5);
			case 53:
				return constant(true, 4, 5);
			case 90:
				return constant(true, 1, 1);
			default:
				break;
			}
			throw "Unsupported degree.";
		}
		constant INLINED MY_LIB cosd(size_t degree) {
			degree %= 180;
			if (degree <= 90) {
				return sind(90 - degree);
			}
			else {
				return -sind(degree - 90);
			}
		}
	}
	inline namespace Function {
		class function;
		template<size_t count>class sum;
		template<size_t count>class product;
		class f_pow_x_ln_x;
		class f_pow_x;
		class f_sin;

		class operation {
		public:
			MY_LIB operation()noexcept { }
			MY_LIB operation(const function &that)noexcept = delete;
			virtual MY_LIB ~operation() { }

			virtual operation *MY_LIB copy() const= 0;
			virtual rough_value MY_LIB estimate(const constant &)const noexcept = 0;
			virtual constant MY_LIB getValue(const constant &con)const noexcept = 0;
			virtual std::ostream &MY_LIB Print(std::ostream &) const noexcept = 0;
			friend std::ostream &MY_LIB operator<<(std::ostream &o, const operation &fun)noexcept {
				return fun.Print(o);
			}

		private:

		};


		class function:public operation {
		public:
			MY_LIB function()noexcept { }
			MY_LIB function(const function &that)noexcept = delete;

			virtual MY_LIB ~function()noexcept { }

			virtual void MY_LIB diff(function *&) noexcept = 0;
			virtual bool MY_LIB integralable()noexcept = 0;
			virtual void MY_LIB undefinite_integral(function *&) noexcept = 0;
			virtual function *MY_LIB copy() const override = 0;
			virtual rough_value MY_LIB estimate(const constant &)const noexcept override = 0;
			virtual constant MY_LIB getValue(const constant &con)const noexcept override = 0;
			virtual std::ostream &MY_LIB Print(std::ostream &) const noexcept override = 0;
			friend std::ostream &MY_LIB operator<<(std::ostream &o, const function &fun)noexcept {
				return fun.Print(o);
			}
		};

		template<size_t count = 2>
		class sum : public function {
			MY_LIB sum()noexcept = default;
			friend class product<count>;
		public:
			template<typename... Pack>sum(Pack &&...pack)noexcept :p{ std::move(pack)... } {
				static_assert(sizeof...(pack) == count, "Parameter not matched");
			}
			MY_LIB ~sum() noexcept {
				for (auto ptr : p) {
					if (ptr) {
						delete ptr;
						//ptr = nullptr;
					}
				}
			}
			sum *MY_LIB copy() const override {
				sum *res = new sum;
				function *p1 = this->p[0], *p2 = res->p[0];
				for (size_t i = 0; i < count; ++i) {
					p2 = p1->copy();
					++p1, ++p2;
				}
				return res;
			}
			constant MY_LIB getValue(const constant &con)const noexcept override {
				return con;
			}
			bool MY_LIB integralable()noexcept override {
				return true;
			}
			void MY_LIB undefinite_integral(function *&f) noexcept override {
				assert(this == f);
				for (auto ptr : p) {
					ptr->undefinite_integral(ptr);
				}
				return;
			}
			void MY_LIB diff(function *&f) noexcept override {
				assert(this == f);
				for (auto ptr : p) {
					ptr->diff(ptr);
				}
				return;
			}
			rough_value MY_LIB estimate(const constant& that)const noexcept override {
				rough_value res = 0;
				for (auto ptr : p) {
					res += ptr->estimate(that);
				}
				return res;
			}
			std::ostream &MY_LIB Print(std::ostream &o)const noexcept override {
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
			template<typename... Pack>product(Pack &&...pack)noexcept :p{ std::move(pack)... } {
				static_assert(sizeof...(pack) == count, "Parameter not matched");
			}
			MY_LIB ~product() noexcept {
				for (auto i : p) {
					if (i) {
						delete i;
					}
				}
			}
			std::ostream &MY_LIB Print(std::ostream &o)const noexcept {
				const function *const *const end = p + count;
				function *const *ptr = p;
				o << '(' << **ptr;
				for (++ptr; ptr != end; ++ptr) {
					o << " * " << *ptr;
				}
				o << ')';
				return o;
			}
			rough_value MY_LIB estimate()const noexcept {
				rough_value res = 0;
				for (auto ptr : p) {
					res *= ptr->estimate();
				}
				return res;
			}
			product *MY_LIB copy() override {
				product *res = new product;
				function *p1 = this->p[0], *p2 = res->p[0];
				for (size_t i = 0; i < count; ++i) {
					p2 = p1->copy();
					++p1, ++p2;
				}
				return res;
			}
			void MY_LIB diff(function *&f) noexcept {
				assert(this == f);

				return;
			}
		private:
			function *p[count];
		};

		class f_pow_x :public function {
		public:
			template<typename Val1, typename Val2>
			MY_LIB f_pow_x(Val1 &&coeff1, Val2 &&coeff2, size_t expo) noexcept :
				coeff(true, std::move(coeff1), std::move(coeff2)),
				expo(expo) { }
			MY_LIB f_pow_x(Q &&coeff, N &&expo) noexcept
				:coeff(std::move(coeff)), expo(std::move(expo)) { }
			MY_LIB ~f_pow_x()noexcept = default;
			void MY_LIB diff(function *&) noexcept override {
				this->coeff *= this->expo;
				if (this->expo != 0) {
					this->expo -= 1;
				}
			}
			bool MY_LIB integralable()noexcept override {
				return true;
			}
			void MY_LIB undefinite_integral(function *&) noexcept override {
				this->expo += 1;
				this->coeff /= this->expo;
			}
			f_pow_x *MY_LIB copy() const override {
				return new f_pow_x(coeff.Copy(coeff), expo.Copy(expo));
			}
			rough_value MY_LIB estimate(const constant &con)const noexcept override{
				return coeff.estim() * std::pow(con.estimate(), expo.GetValue<rough_value>());
			}
			constant MY_LIB getValue(const constant &con)const noexcept override{
				return con;
			}
			std::ostream &MY_LIB Print(std::ostream &o) const noexcept override {
				return (coeff == 0 ? o << "0" : o << coeff << " * x" << "^(" << expo << ")");
			}
		private:
			Q coeff;
			N expo;
		};

		class f_pow_x_ln_x :public function {
		public:
			template<typename...pack>MY_LIB f_pow_x_ln_x(pack...in)noexcept :pow(in...) { }
			MY_LIB ~f_pow_x_ln_x()noexcept {
				if (pow != nullptr) {
					delete this->pow;
				}
			}
			function *MY_LIB copy() const override {
				auto &&temp = new f_pow_x_ln_x(this->pow->copy());
				return temp;
			}
			inline void MY_LIB diff(function *&f) noexcept override {
				assert(this == f);
				//f = new sum<2>(new f_pow_x(pow));
				delete this;
				return;
			}
			bool MY_LIB integralable() noexcept override {
				return true;
			}
			void MY_LIB undefinite_integral(function *&f) noexcept override {
				assert(this == f);
				assert(false);
				return;
			}
			rough_value MY_LIB estimate(const constant &con)const noexcept override {
				return std::log(this->pow->estimate(con));
			}
			constant MY_LIB getValue(const constant &con)const noexcept override {
				return con;
			}
			std::ostream &MY_LIB Print(std::ostream &o)const noexcept override {
				return o << "ln(" << *pow << ')';
			}
		private:
			f_pow_x *pow;
		};


		template<typename foo = operation>class ptrHolder;

		template<>
		class ptrHolder<function> {
		private:
			function *func_ptr;
		public:
			MY_LIB ptrHolder(function *f)noexcept :func_ptr(f) { }
			MY_LIB ptrHolder(ptrHolder &&rv)noexcept :func_ptr(rv.func_ptr) { rv.func_ptr = nullptr; }
			MY_LIB ptrHolder(const ptrHolder &lvalue) : func_ptr(lvalue.func_ptr ? lvalue.func_ptr->copy() : nullptr) { }
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
			constant MY_LIB getValue(const constant& c)noexcept {
				return this->func_ptr ? this->func_ptr->getValue(c) : constant(true, 0, 1);
			}
			constexpr bool MY_LIB operator!()noexcept {
				return this->func_ptr == nullptr;
			}
			constexpr bool MY_LIB operator==(nullptr_t)noexcept {
				return this->func_ptr == nullptr;
			}
			constexpr bool MY_LIB operator!=(nullptr_t)noexcept {
				return this->func_ptr != nullptr;
			}
			friend std::ostream &MY_LIB operator<<(std::ostream &o, ptrHolder holder)noexcept {
				if (holder.func_ptr) holder.func_ptr->Print(o); else o << '0';
				return o;
			}
		};

		template<>
		class ptrHolder<operation> {
		private:
			operation *func_ptr;
		public:
			MY_LIB ptrHolder(operation *f)noexcept :func_ptr(f) { }
			MY_LIB ptrHolder(ptrHolder &&rv)noexcept :func_ptr(rv.func_ptr) { rv.func_ptr = nullptr; }
			MY_LIB ptrHolder(const ptrHolder &lvalue) : func_ptr(lvalue.func_ptr ? lvalue.func_ptr->copy() : nullptr) { }
			MY_LIB ~ptrHolder() { if (func_ptr)delete func_ptr; }
			constant MY_LIB getValue(const constant& c)noexcept {
				return this->func_ptr ? this->func_ptr->getValue(c) : constant(true, 0, 1);
			}
			constexpr bool MY_LIB operator!()noexcept {
				return this->func_ptr == nullptr;
			}
			constexpr bool MY_LIB operator==(nullptr_t)noexcept {
				return this->func_ptr == nullptr;
			}
			constexpr bool MY_LIB operator!=(nullptr_t)noexcept {
				return this->func_ptr != nullptr;
			}
			friend std::ostream &MY_LIB operator<<(std::ostream &o, ptrHolder holder)noexcept {
				if (holder.func_ptr) holder.func_ptr->Print(o); else o << '0';
				return o;
			}
		};

		class Expression {
		private:
			std::string subsequent;
		public:
			MY_LIB Expression(const char *Subsequent) :subsequent(Subsequent) { }
			MY_LIB ~Expression()noexcept = default;
			static auto MY_LIB getPriorityOutside(char c)noexcept {
				switch (c) {
				case ')':
					return 8;
				case '^':
					return 6;
				case '*':
				case '/':
				case '%':
					return 5;
				case '+':
				case '-':
					return 3;
				case '(':
					return 1;
				case '#':
					return 0;
				default:
					assert(false);
					return 0;
				}
			}
			static auto MY_LIB getPriorityInside(char c)noexcept {
				switch (c) {
				case '(':
					return 8;
				case '^':
					return 7;
				case '*':
				case '/':
				case '%':
					return 4;
				case '+':
				case '-':
					return 2;
				case ')':
					return 1;
				case '#':
					return 0;
				default:
					assert(false);
					return 0;
				}
			}
			static std::string MY_LIB transform(const char* s) {
				std::string st;
				std::stack<char> ops;//‘ÀÀ„∑˚’ª
				for (; *s != 0; ++s) {
					char ch = *s;
					if (ch == ' ')continue;
					else if (exist<Set<10>, HigherCharSet,LowerCharSet>(ch)) {
						if (!st.empty() && exist<Set<10>, HigherCharSet, LowerCharSet>(st.back())) {
							st.push_back(' ');
						}
						do {
							st.push_back(ch);
						} while ((ch = *++s) != 0 && exist<Set<10>, HigherCharSet, LowerCharSet>(ch));
						--s;
					}
					else if (ch == '(') {
						ops.push(ch);
					}
					else if (ch == ')') {
						while (true) {
							if (ops.empty()) {
								throw "No matched brackets.";
							}
							ch = ops.top();
							ops.pop();
							if (ch == '(') {
								break;
							}
							else {
								st.push_back(ch);
							}
						}
					}
					else {
						while (true) {
							if (ops.empty() || ops.top() == '('|| (getPriorityInside(ops.top()) < getPriorityOutside(ch))) {
								ops.push(ch);
								break;
							}
							st.push_back(ops.top());
							ops.pop();
						}
					}
				}
				while (!ops.empty()) {
					st.push_back(ops.top());
					ops.pop();
				}
				return st;
			}
			static std::unique_ptr<Expression>MY_LIB MakeFromMiddleOrder(const char *MidOrder) {
				try {
					auto &&str = transform(MidOrder);
					auto *res = new Expression(str.c_str());
					return std::unique_ptr<Expression>(res);
				}
				catch (const char* e) {
					throw e;
				}
			}
			Expression* MY_LIB copy()const{ 
				return new Expression(subsequent.c_str());
			}
			rough_value MY_LIB estimate(const constant& t)const noexcept{
				return t.estimate();
			}
			constant MY_LIB getValue(const constant &con)const noexcept {
				std::deque<constant> num;
				for (auto p = subsequent.cbegin(); p != subsequent.cend(); ++p) {
					if (Set<10>::exist(*p)) {
						auto end = p;
						while (++end, end != subsequent.cend() && Set<10>::exist(*end));
						if (end > p)--end;
						num.push_back(constant(&*p, &*end));
						p = end;
					}
					else {
						switch (*p) {
						case ' ': break;
						case 't': {
							num.push_back(con);
							break;
						}
						case '+': {
							auto b = num.back();
							num.pop_back();
							num.back() += b;
							break;
						}
						case '-': {
							auto b = num.back();
							num.pop_back();
							num.back() -= b;
							break;
						}
						case '*': {
							auto b = num.back();
							num.pop_back();
							num.back() *= b;
							break;
						}
						case '/': {
							auto b = num.back();
							num.pop_back();
							num.back() /= b;
							break;
						}
						case '^': {
							auto b = num.back();
							num.pop_back();
							num.back() ^= b;
							break;
						}
						default: break;
						}
					}
				}
				assert(num.size() == 1);
				return num.back();
			}
			std::ostream &MY_LIB Print(std::ostream &o) const noexcept {
				return o << subsequent;
			}
			friend std::ostream &MY_LIB operator<<(std::ostream &o, const Expression &fun)noexcept {
				return fun.Print(o);
			}
		};




		class funEngine {
		public:
			using holder = std::unique_ptr<Expression>;
			funEngine() = default;
			~funEngine()noexcept = default;
			static holder produce(nullptr_t)noexcept { return holder(nullptr); }
			static holder produce(const char *definition) {
				try {
					return holder(Expression::MakeFromMiddleOrder(definition));
				}
				catch (const char* e) {
					throw e;
				}
			}
		private:

		};

	}
}