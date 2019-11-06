#include "Statistics.h"
#include "VariableParameterTemplate.h"
#include "InstructionSet.h"
#include <amp.h>
#include <omp.h>

inline namespace Math {
	enum class Occupation {
		CPU,GPU
	};

	//using concurrency::array;
	//using concurrency::array_view;
	//using concurrency::index;
	//using concurrency::parallel_for_each;



	template<typename Data, size_t... pack>class MatrixGPU;
	template<typename Data, Align align, size_t... pack>class MatrixCPU;

	template<typename Data, size_t... pack>
	class MatrixGPU {
	public:
		using size=int;
		Concurrency::array<Data, sizeof...(pack)> Element;
		MY_LIB MatrixGPU(size E0, Data* E) :Element(E0, E) {

		}

		void MY_LIB operator+=(const MatrixGPU& that)noexcept {
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

	template<typename Data, Align align, size_t... pack>
	class alignas(alignToSize<align>()) MatrixCPU  {
	public:
		using product=Template::product<pack...>;
		constexpr static size_t numRawData()noexcept {
			return product::value();
		}
		constexpr static size_t numArrayElem()noexcept {
			return ElemType::lenArray(product::value());
		}
		using ElemType=_mm_cpp<align, Data>;
	protected:
		constexpr MY_LIB MatrixCPU()noexcept { }
		ElemType Element[numArrayElem()];
	public:
		static constexpr Align getAlign()noexcept { return align; }
		using size=size_t;
		static_assert(sizeof...(pack) != 0, "The length of parameter pack should not be 0");
		static_assert(std::is_arithmetic_v<Data>, "Arithmetic type required");

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

		constexpr MY_LIB MatrixCPU(const MatrixCPU& that)noexcept :Element(that.Element) { }
		constexpr MY_LIB MatrixCPU(MatrixCPU&&)noexcept = default;

		template<class induce>
		constexpr explicit MY_LIB MatrixCPU(induce& ind) noexcept {
			for (size_t i = 0; i < numRawData(); ++i) {
				reinterpret_cast<Data*>(this->Element)[i] = Data(ind());
			}
		}
		template<class induce>
		constexpr explicit MY_LIB MatrixCPU(size_t start, induce& ind) noexcept {
		#pragma omp parallel for
			for (size_t i = 0; i < numRawData(); ++i) {
				reinterpret_cast<Data*>(this->Element)[i] = Data(ind(i + start));
			}
		}

		template<class induce>
		static constexpr void MY_LIB ParallelMake(MatrixCPU& m,induce& ind)noexcept {
		#pragma omp parallel for
			for (size_t i = 0; i < numRawData(); ++i) {
				reinterpret_cast<Data*>(m.Element)[i] = Data(ind(i));
			}
			return m;
		}
		template<class induce>
		static constexpr MatrixCPU MY_LIB ParallelMake(induce& ind)noexcept { 
			MatrixCPU m;
			ParallelMake(m, ind);
			return m;
		}

		MY_LIB ~MatrixCPU()noexcept = default;

		template<size_t index>constexpr const ElemType& get()const noexcept {
			return this->Element[index];
		}


		MatrixCPU& MY_LIB operator+=(const MatrixCPU& that)noexcept {
		#pragma omp parallel for
			for (size_t i = 0; i < numArrayElem(); ++i) {
				this->Element[i] += that.Element[i];
			}
			return *this;
		}
		MatrixCPU& MY_LIB operator-=(const MatrixCPU& that)noexcept {
		#pragma omp parallel for
			for (size_t i = 0; i < numArrayElem(); ++i) {
				this->Element[i] -= that.Element[i];
			}
			return *this;
		}
		MatrixCPU& MY_LIB operator*=(const MatrixCPU& that)noexcept {
		#pragma omp parallel for
			for (size_t i = 0; i < numArrayElem(); ++i) {
				this->Element[i] *= that.Element[i];
			}
			return *this;
		}
		MatrixCPU& MY_LIB operator/=(const MatrixCPU& that)noexcept {
		#pragma omp parallel for
			for (size_t i = 0; i < numArrayElem(); ++i) {
				this->Element[i] /= that.Element[i];
			}
			return *this;
		}
		MatrixCPU& MY_LIB operator%=(const MatrixCPU& that)noexcept {
			static_assert(std::is_integral_v<Data>,"Integral type required.");
		#pragma omp parallel for
			for (size_t i = 0; i < numArrayElem(); ++i) {
				this->Element[i] %= that.Element[i];
			}
			return *this;
		}

		template<typename out>
		friend out& MY_LIB operator<<(out& o, const MatrixCPU& m)noexcept {
			for (size_t i = 0; i < numArrayElem(); ++i) {
				o << m.Element[i] << ' ';
			}
			return o;
		}
	private:
	};
};