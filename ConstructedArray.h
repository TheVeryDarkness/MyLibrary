#pragma once

namespace Math {
	template<typename T, size_t sz>class Array :Array<T, sz - 1> {
		T data;
		static_assert(sz != 0, "Wrong size");
		static_assert(sz <= 10000, "Not supported");
		using super=Array<T, sz - 1>;
	public:
		Array() = delete;
		~Array() = default;
		template<typename... pack_t>
		constexpr Array(T head, pack_t... pack)noexcept:data(head), super(pack...) {}

		template<class induce>
		constexpr Array(induce ind, size_t index):data(ind(index)),super(ind, index + 1) {}

		template<class induce>
		constexpr Array(induce ind):data(ind(index)){
		#pragma omp parallel for
			for (size_t i = 0; i < numElems(); ++i) {
				(&data)[i] = ind(i);
			}
		}


		template<size_t index>
		constexpr T& get()noexcept {
			static_assert(index < sz, "The index is out of range");
			return ((index == 0) ? (this->data) : (get<index - 1>()));
		}
		constexpr T& operator[](size_t index)noexcept {
			assert(index < sz);
			return ((index == 0) ? (this->data) : (this->super::operator[](index - 1)));
		}
		constexpr explicit operator T* ()noexcept {
			return this;
		}
		template<size_t index>
		constexpr const T& get()const noexcept {
			static_assert(index < sz, "The index is out of range");
			return ((index == 0) ? (this->data) : (get<index - 1>()));
		}
		constexpr const T& operator[](size_t index)const noexcept {
			assert(index < sz);
			return ((index == 0) ? (this->data) : (this->super::operator[](index - 1)));
		}
		constexpr explicit operator const T* ()const noexcept {
			return this;
		}

		template<typename out>
		friend out& MY_LIB operator<<(out& o, const Array& m)noexcept {
			return o << m.data << ',' << ' ' << *static_cast<const super*>(&m);
		}
	};
	template<typename T>class Array<T, 1>{
		T data;
	public:
		Array() = delete;
		~Array() = default;
		constexpr Array(T head)noexcept :data(head) { }

		template<class induce>
		constexpr Array(induce ind, size_t index) : data(ind(index)) { }

		template<size_t index>
		constexpr T& get()noexcept {
			static_assert(index == 0, "The index is out of range");
			return (this->data);
		}
		constexpr T& operator[](size_t index)noexcept {
			assert(index == 0);
			return (this->data);
		}
		constexpr explicit operator T* ()noexcept {
			return this;
		}
		template<size_t index>
		constexpr const T& get()const noexcept {
			static_assert(index == 0, "The index is out of range");
			return (this->data);
		}
		constexpr const T& operator[](size_t index)const noexcept {
			assert(index == 0);
			return (this->data);
		}
		constexpr explicit operator const T* ()const noexcept {
			return this;
		}

		template<typename out>
		friend out& MY_LIB operator<<(out& o, const Array& m)noexcept {
			return o << m.data;
		}
	};
}