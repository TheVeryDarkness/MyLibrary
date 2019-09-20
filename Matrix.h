#include <amp.h>

namespace Math {
	using concurrency::array;
	using concurrency::array_view;
	using concurrency::index;
	using concurrency::parallel_for_each;

	template<typename Data, size_t... pack>class Matrix;

	template<typename Data, size_t... pack>
	class Matrix {
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
};