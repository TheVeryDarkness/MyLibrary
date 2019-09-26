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
		constexpr Array(T head, pack_t... pack)noexcept:super(pack...), data(head) {
			static_assert(sizeof...(pack) == sz - 1, "Parameter is not enough");
		}
		template<class induce>
		constexpr Array(induce& ind):data(ind()), super(ind){ }


		constexpr static auto getSize()noexcept {
			return sz;
		}

		template<size_t index>
		constexpr T& get()noexcept {
			static_assert(index < sz, "The index is out of range");
			return ((index == 0) ? (this->data) : (get<index - 1>()));
		}
		template<size_t index>
		constexpr const T& get()const noexcept {
			static_assert(index < sz, "The index is out of range");
			return ((index == 0) ? (this->data) : (get<index - 1>()));
		}

		constexpr T& operator[](size_t index)noexcept {
			assert(index < sz);
			return ((index == 0) ? (this->data) : (this->super::operator[](index - 1)));
		}
		constexpr const T& operator[](size_t index)const noexcept {
			assert(index < sz);
			return ((index == 0) ? (this->data) : (this->super::operator[](index - 1)));
		}

		constexpr explicit operator T* ()noexcept {
			return reinterpret_cast<T*>(this);
		}
		constexpr explicit operator const T* ()const noexcept {
			return reinterpret_cast<const T*>(this);
		}

		constexpr auto decay()noexcept {
			return this->operator T * ();
		}
		constexpr auto decay()const noexcept {
			return this->operator const T * ();
		}

		template<typename out>
		friend out& MY_LIB operator<<(out& o, const Array& m)noexcept {
			return o << *static_cast<const super*>(&m) << ',' << ' ' << m.data;
		}
	};
	//********* 1 *********
	template<typename T>class Array<T, 1>{
		T data;
	public:
		Array() = delete;
		~Array() = default;
		constexpr Array(T head)noexcept :data(head) { }

		template<class induce>
		constexpr Array(induce& ind) : data(ind()) { }


		constexpr static auto getSize()noexcept {
			return sz;
		}

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
			return reinterpret_cast<T*>(this);
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
			return reinterpret_cast<const T*>(this);
		}

		template<typename out>
		friend out& MY_LIB operator<<(out& o, const Array& m)noexcept {
			return o << m.data;
		}
	};
}