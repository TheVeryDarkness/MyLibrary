#include "Statistics.h"
#include "VariableParameterTemplate.h"
#include "normalArray.h"
#include "InstructionSet.h"
#include <amp.h>
#include <omp.h>

namespace Math {
	enum class Occupation {
		CPU,GPU
	};

	//using concurrency::array;
	//using concurrency::array_view;
	//using concurrency::index;
	//using concurrency::parallel_for_each;



	template<typename Data, Occupation, size_t... pack>class Matrix;

	template<typename Data, size_t... pack>
	class Matrix<Data, Occupation::GPU, pack...> {
	public:
		using size=int;
		Concurrency::array<Data, sizeof...(pack)> Element;
		MY_LIB Matrix(size E0, Data* E) :Element(E0, E) { 

		}

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
	class alignas(Data) Matrix<Data, Occupation::CPU, pack...>  {
	protected:
		using product=Template::product<pack...>;
		constexpr MY_LIB Matrix()noexcept { }
		Data Element[product::value()];
	public:
		using size=size_t;
		using ELEMENT_TYPE=Data;
		static_assert(sizeof...(pack) != 0, "The length of parameter pack should not be 0");

		constexpr static size_t numElems()noexcept {
			return product::value();
		}

		void* operator new(size_t sz) {
			return
			#ifdef _DEBUG
				_aligned_malloc_dbg(sz, alignof(Data), __FILE__, __LINE__)
			#else
				_aligned_malloc(sz, alignof(Data))
			#endif // _DEBUG
				;
		}
		void operator delete(void* ptr) {
			return 
			#ifdef _DEBUG
				_aligned_free_dbg(ptr)
			#else
				_aligned_free(ptr)
			#endif // _DEBUG
				;
		}

		template<class induce>
		constexpr MY_LIB Matrix(induce ind) noexcept {
			for (size_t i = 0; i < numElems(); ++i) {
				this->Element[i] = Data(ind());
			}
		}

		template<class induce>
		static constexpr Matrix MY_LIB ParallelMake(induce ind)noexcept {
			Matrix m;
		#pragma omp parallel for
			for (size_t i = 0; i < numElems(); ++i) {
				m.Element[i] = Data(ind(i));
			}
			return m;
		}

		MY_LIB ~Matrix()noexcept = default;

		Matrix& MY_LIB operator+=(const Matrix& that)noexcept {
		#pragma omp parallel for
			for (size_t i = 0; i < numElems(); ++i) {
				this->Element[i] += that.Element[i];
			}
			return *this;
		}
		Matrix& MY_LIB operator-=(const Matrix& that)noexcept {
		#pragma omp parallel for
			for (size_t i = 0; i < numElems(); ++i) {
				this->Element[i] -= that.Element[i];
			}
			return *this;
		}
		Matrix& MY_LIB operator*=(const Matrix& that)noexcept {
		#pragma omp parallel for
			for (size_t i = 0; i < numElems(); ++i) {
				this->Element[i] *= that.Element[i];
			}
			return *this;
		}
		Matrix& MY_LIB operator/=(const Matrix& that)noexcept {
		#pragma omp parallel for
			for (size_t i = 0; i < numElems(); ++i) {
				this->Element[i] /= that.Element[i];
			}
			return *this;
		}
		Matrix& MY_LIB operator%=(const Matrix& that)noexcept {
			static_assert(std::is_integral_v<Data>,"Integral type required.");
		#pragma omp parallel for
			for (size_t i = 0; i < numElems(); ++i) {
				this->Element[i] %= that.Element[i];
			}
			return *this;
		}

		template<typename out>
		friend out& MY_LIB operator<<(out& o, const Matrix& m)noexcept {
			return o << m.Element;
		}
	private:
	};
};