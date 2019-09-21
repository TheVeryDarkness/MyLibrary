#include <amp.h>
#include <omp.h>
#include "Statistics.h"

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
		__stdcall Matrix(size E0, Data* E) :Element(E0, E) { }

		void operator+=(const Matrix& that)noexcept {
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
			Add(arr& a, const arr & b) :a(a), b(b) { }
			~Add() = default;
			void operator()(concurrency::index<sizeof...(pack)> idx)const restrict(amp) {
				a[idx] += b[idx];
			}
		private:
			arr& a;
			const arr&b;
		};
	};


	template<typename Data, size_t... pack>
	class Matrix<Data, Occupation::CPU, pack...> {
	public:
		using size=size_t;
		Data Element[product<pack...>()];
		__stdcall Matrix(size E0, Data* E) :Element(E0, E) { }

		template<class induce>
		__stdcall Matrix

		void operator+=(const Matrix& that)noexcept {
		#pragma omp parallel for
			for (size_t i = 0; i < product<pack...>(); i++) {
				Element[i] += that.Element[i];
			}
		}

		template<typename out>
		friend out& operator<<(out& o, const Matrix& m)noexcept {
			for (auto i : Element) {
				o << m;
			}
			return o;
		}
	private:
	};
};