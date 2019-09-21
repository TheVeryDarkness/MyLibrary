#include "Statistics.h"
#include "VariableParameterTemplate.h"
#include <amp.h>
#include <omp.h>

namespace Math {
	enum class Occupation {
		CPU,GPU
	};

	using concurrency::array;
	using concurrency::array_view;
	using concurrency::index;
	using concurrency::parallel_for_each;



	template<typename Data, Occupation, size_t... pack>class Matrix;

	template<typename Data, size_t... pack>
	class Matrix<Data, Occupation::GPU, pack...> {
	public:
		using size=int;
		Concurrency::array<Data, sizeof...(pack)> Element;
		MY_LIB Matrix(size E0, Data* E) :Element(E0, E) { }

		void MY_LIB operator+=(const Matrix& that)noexcept {
			Add add(this->Element, that.Element);
			Concurrency::parallel_for_each(
				this->Element.extent,
				add
			);
		}
	private:
		class Add {
		public:
			using arr=Concurrency::array<Data, sizeof...(pack)>;
			MY_LIB Add(arr& a, const arr & b) :a(a), b(b) { }
			MY_LIB ~Add() = default;
			void MY_LIB operator()(concurrency::index<sizeof...(pack)> idx)const restrict(amp) {
				a[idx] += b[idx];
			}
		private:
			arr& a;
			const arr&b;
		};
	};

	template<typename Data, size_t... pack>
	class Matrix<Data, Occupation::CPU, pack...>  {
	public:
		Data Element[product<pack...>::value()];
		using size=size_t;
		static_assert(sizeof...(pack) != 0, "The length of parameter pack should not be 0");


		MY_LIB Matrix(size E0, Data* E) :Element(E0, E) { }

		constexpr static size_t numElems()noexcept {
			return product<pack...>::value();
		}

		template<class induce>
		MY_LIB Matrix(induce ind) {
		#pragma omp parallel for
			for (size_t i = 0; i < numElems(); ++i) {
				Element[i] = ind(i);
			}
		}

		void MY_LIB operator+=(const Matrix& that)noexcept {
		#pragma omp parallel for
			for (size_t i = 0; i < numElems(); i++) {
				this->Element[i] += that.Element[i];
			}
		}

		template<typename out>
		friend out& MY_LIB operator<<(out& o, const Matrix& m)noexcept {
			for (auto i : m.Element) {
				o << i << ',' << ' ';
			}
			return o;
		}
	private:
	};
};