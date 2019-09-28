#pragma once

#include <intrin.h>
namespace Math {
	enum class Align :size_t {
		_512 = 64, _256 = 32, _128 = 16, _64 = 8
	};
	constexpr bool supported(Align align)noexcept {
		return align == Align::_64 || align == Align::_128 || align == Align::_256 || align == Align::_512;
	}
	template<Align align>
	constexpr size_t alignToSize()noexcept {
		static_assert(supported(align));
		return static_cast<size_t>(align);
	}
	constexpr size_t alignToSize(Align align)noexcept {
		assert(supported(align));
		return static_cast<size_t>(align);
	}

	template<typename T, Align align>
	class base {
	public:
		using type=T;
		static constexpr size_t getNum()noexcept { 
			static_assert(std::is_arithmetic_v<T>, "No matched type");
			return 1; 
		}
		static constexpr type store(T t)noexcept {
			return t;
		}
		static constexpr T* depack(type& t)noexcept {
			return &t;
		}
		static constexpr const T* depack(const type& t)noexcept {
			return &t;
		}
	};

	//__int8
	template<>class base<__int8,Align::_64> {
	public:
		using type=__m64;
		static constexpr size_t getNum()noexcept { return 8; }
	};
	template<>class base<__int8, Align::_128> {
	public:
		using type=__m128i;
		static constexpr size_t getNum()noexcept { return 16; }
	};
	template<>class base<__int8, Align::_256> {
	public:
		using type=__m256i;
		static constexpr size_t getNum()noexcept { return 32; }
	};
	template<>class base<__int8, Align::_512> {
	public:
		using type=__m512i;
		static constexpr size_t getNum()noexcept { return 64; }
	};


	//unsigned __int8
	template<>class base<unsigned __int8,Align::_64> {
	public:
		using type=__m64;
		static constexpr size_t getNum()noexcept { return 8; }
	};
	template<>class base<unsigned __int8, Align::_128> {
	public:
		using type=__m128i;
		static constexpr size_t getNum()noexcept { return 16; }
	};
	template<>class base<unsigned __int8, Align::_256> {
	public:
		using type=__m256i;
		static constexpr size_t getNum()noexcept { return 32; }
	};
	template<>class base<unsigned __int8, Align::_512> {
	public:
		using type=__m512i;
		static constexpr size_t getNum()noexcept { return 64; }
	};


	//__int16
	template<>class base<__int16,Align::_64> {
	public:
		using type=__m64;
		static constexpr size_t getNum()noexcept { return 4; }
	};
	template<>class base<__int16, Align::_128> {
	public:
		using type=__m128i;
		static constexpr size_t getNum()noexcept { return 8; }
		template<typename Induct>
		static constexpr type store(Induct& ind)noexcept {
			static_assert(std::is_same_v<decltype(ind()), __int16>, "Type not matched.");
			return _mm_set_epi16(ind(), ind(), ind(), ind(), ind(), ind(), ind(), ind());
		}
		static constexpr __int16* depack(type& t)noexcept {
			return t.m128i_i16;
		}
		static constexpr const __int16* depack(const type& t)noexcept {
			return t.m128i_i16;
		}
	};
	template<>class base<__int16,Align::_256> {
	public:
		using type=__m256i;
		static constexpr size_t getNum()noexcept { return 16; }
	};
	template<>class base<__int16, Align::_512> {
	public:
		using type=__m512i;
		static constexpr size_t getNum()noexcept { return 32; }
	};


	//unsigned __int16
	template<>class base<unsigned __int16,Align::_64> {
	public:
		using type=__m64;
		static constexpr size_t getNum()noexcept { return 4; }
	};
	template<>class base<unsigned __int16, Align::_128> {
	public:
		using type=__m128i;
		static constexpr size_t getNum()noexcept { return 8; }
	};
	template<>class base<unsigned __int16,Align::_256> {
	public:
		using type=__m256i;
		static constexpr size_t getNum()noexcept { return 16; }
	};
	template<>class base<unsigned __int16, Align::_512> {
	public:
		using type=__m512i;
		static constexpr size_t getNum()noexcept { return 32; }
	};


	//__int32
	template<>class base<__int32,Align::_64> {
	public:
		using type=__int64;
		static constexpr size_t getNum()noexcept { return 2; }
		static __int32* depack(type& t)noexcept {
			return reinterpret_cast<__int32*>(&t);
		}
		static const __int32* depack(const type& t)noexcept {
			return reinterpret_cast<const __int32*>(&t);
		}
		static type add(const type& a, const type& b)noexcept {
			return (a + b);
		}
		static type load(const type& that)noexcept {
			return that;
		}
	};
	template<>class base<__int32, Align::_128> {
	public:
		using type=__m128i;
		static constexpr size_t getNum()noexcept { return 4; }
		template<typename Induct>
		static constexpr type store(Induct& ind)noexcept {
			static_assert(std::is_same_v<decltype(ind()), __int32>, "Type not matched.");
			return _mm_set_epi32(ind(), ind(), ind(), ind());
		}
		static constexpr __int32* depack(type& t)noexcept {
			return t.m128i_i32;
		}
		static constexpr const __int32* depack(const type& t)noexcept {
			return t.m128i_i32;
		}
	};
	template<>class base<__int32, Align::_256> {
	public:
		using type=__m256i;
		static constexpr size_t getNum()noexcept { return 8; }
		template<typename Induct>
		static constexpr type store(Induct& ind)noexcept {
			static_assert(std::is_same_v<decltype(ind()), __int32>, "Type not matched.");
			return _mm256_set_epi32(
				ind(), ind(), ind(), ind(),
				ind(), ind(), ind(), ind()
			);
		}
		static type store(__int32 E0, __int32 E1, __int32 E2, __int32 E3, __int32 E4, __int32 E5, __int32 E6, __int32 E7)noexcept {
			return _mm256_set_epi32(
				E0, E1, E2, E3, E4, E5, E6, E7
			);
		}
		static constexpr __int32* depack(type& t)noexcept {
			return t.m256i_i32;
		}
		static constexpr const __int32* depack(const type& t)noexcept {
			return t.m256i_i32;
		}
		static type add(const type& a, const type& b)noexcept { 
			return _mm256_add_epi32(a, b);
		}
		static type load(const type& that)noexcept {
			return _mm256_load_si256(&that);
		}
	};
	template<>class base<__int32, Align::_512> {
	public:
		using type=__m512i;
		static constexpr size_t getNum()noexcept { return 16; }
		template<typename Induct>
		static constexpr type store(Induct& ind)noexcept {
			static_assert(std::is_same_v<std::remove_cv_t<decltype(ind())>, __int32>, "Type not matched.");
			return _mm512_set_epi32(
				ind(), ind(), ind(), ind(),
				ind(), ind(), ind(), ind(),
				ind(), ind(), ind(), ind(),
				ind(), ind(), ind(), ind()
			);
		}
		static constexpr __int32* depack(type& t)noexcept {
			return t.m512i_i32;
		}
		static constexpr const __int32* depack(const type& t)noexcept {
			return t.m512i_i32;
		}
		static type add(const type& a, const type& b)noexcept {
			return _mm512_add_epi32(a, b);
		}
		static type load(const type& that)noexcept {
			return _mm512_load_si512(&that);
		}
	};


	//unsigned __int32
	template<>class base<unsigned __int32,Align::_64> {
	public:
		using type=__m64;
		static constexpr size_t getNum()noexcept { return 2; }
	};
	template<>class base<unsigned __int32, Align::_128> {
	public:
		using type=__m128i;
		static constexpr size_t getNum()noexcept { return 4; }
	};
	template<>class base<unsigned __int32, Align::_256> {
	public:
		using type=__m256i;
		static constexpr size_t getNum()noexcept { return 8; }
	};
	template<>class base<unsigned __int32, Align::_512> {
	public:
		using type=__m512i;
		static constexpr size_t getNum()noexcept { return 16; }
	};


	//__int64
	template<>class base<__int64,Align::_128> {
	public:
		using type=__m128i;
		static constexpr size_t getNum()noexcept { return 2; }
		template<typename Induct>
		static constexpr type store(Induct& ind)noexcept {
			static_assert(std::is_same_v<decltype(ind()), __int64>, "Type not matched.");
			return _mm_set_epi64(ind(), ind());
		}
		static constexpr __int64* depack(type& t)noexcept {
			return t.m128i_i64;
		}
		static constexpr const __int64* depack(const type& t)noexcept {
			return t.m128i_i64;
		}
	};
	template<>class base<__int64,Align::_256> {
	public:
		using type=__m256i;
		static constexpr size_t getNum()noexcept { return 4; }
		template<typename Induct>
		static constexpr type store(Induct& ind)noexcept {
			static_assert(std::is_arithmetic_v<std::remove_cv_t<decltype(ind())>>, "Type not matched.");
			return _mm256_set_epi64x(ind(), ind(), ind(), ind());
		}
		static type store(__int64 E0,__int64 E1,__int64 E2,__int64 E3)noexcept {
			return _mm256_set_epi64x(E0, E1, E2, E3);
		}
		static constexpr __int64* depack(type& t)noexcept {
			return t.m256i_i64;
		}
		static constexpr const __int64* depack(const type& t)noexcept {
			return t.m256i_i64;
		}
		static type add(const type& a, const type& b)noexcept {
			return _mm256_add_epi64(a, b);
		}
		static type load(const type& that)noexcept {
			return _mm256_load_si256(&that);
		}
	};
	template<>class base<__int64,Align::_512> {
	public:
		using type=__m512i;
		static constexpr size_t getNum()noexcept { return 8; }
		template<typename Induct>
		static constexpr type store(Induct& ind)noexcept {
			static_assert(std::is_same_v<std::remove_cv_t<decltype(ind())>, __int64>, "Type not matched.");
			type mm512;
			for (auto &i : mm512.m512i_i64) {
				i = ind();
			}
			return mm512;
		}
		static constexpr __int64* depack(type& t)noexcept {
			return t.m512i_i64;
		}
		static constexpr const __int64* depack(const type& t)noexcept {
			return t.m512i_i64;
		}
		static type add(const type& a, const type& b)noexcept {
			return _mm512_add_epi64(a, b);
		}
		static type load(const type& that)noexcept {
			return _mm512_load_si512(&that);
		}
	};


	//unsigned __int64
	template<>class base<unsigned __int64,Align::_128> {
	public:
		using type=__m128i;
		static constexpr size_t getNum()noexcept { return 2; }
	};
	template<>class base<unsigned __int64,Align::_256> {
	public:
		using type=__m256i;
		static constexpr size_t getNum()noexcept { return 4; }
	};
	template<>class base<unsigned __int64,Align::_512> {
	public:
		using type=__m512i;
		static constexpr size_t getNum()noexcept { return 8; }
	};


	//double
	template<>class base<double,Align::_128> {
	public:
		using type=__m128d;
		static constexpr size_t getNum()noexcept { return 2; }
	};
	template<>class base<double,Align::_256> {
	public:
		using type=__m256d;
		static constexpr size_t getNum()noexcept { return 4; }
	};
	template<>class base<double,Align::_512> {
	public:
		using type=__m512d;
		static constexpr size_t getNum()noexcept { return 8; }
	};


	//float
	template<>class base<float, Align::_128> {
	public:
		using type=__m128;
		static constexpr size_t getNum()noexcept { return 4; }
	};
	template<>class base<float, Align::_256> {
	public:
		using type=__m256;
		static constexpr size_t getNum()noexcept { return 8; }
	};
	template<>class base<float, Align::_512> {
	public:
		using type=__m512;
		static constexpr size_t getNum()noexcept { return 16; }
	};

	template<Align align, typename T>
	class alignas(alignToSize(align)) _mm_cpp:public base<T ,align> {
	public:
		using Basic=base<T, align>;
		using type=typename Basic::type;
		type data;
		static constexpr size_t lenArray(size_t len)noexcept {
			return ((len - 1) / Basic::getNum() + 1);
		}
		/*[[deprecated("Not initialized")]]*/ _mm_cpp()noexcept { }
		_mm_cpp(const _mm_cpp& m) :data(m.data) { }
		_mm_cpp(const type& m) :data(m) { }
		_mm_cpp(_mm_cpp&& m) :data(m..data) { }
		template<typename Induct>_mm_cpp(Induct& ind)noexcept
			:data(Basic::store(ind)) { }
		static void* operator new(size_t sz) {
			return
			#ifdef _DEBUG
				_aligned_malloc_dbg(sz, alignToSize<align>(), __FILE__, __LINE__)
			#else
				_aligned_malloc(sz, alignToSize<align>())
			#endif // _DEBUG
				;
		}
		static void operator delete(void* ptr) {
			return
			#ifdef _DEBUG
				_aligned_free_dbg(ptr)
			#else
				_aligned_free(ptr)
			#endif // _DEBUG
				;
		}
		~_mm_cpp() { }

		_mm_cpp& operator=(const _mm_cpp & that)noexcept{
			this->data = Basic::load(that.data);
			return *this;
		}


		_mm_cpp operator+(const _mm_cpp& that) const noexcept{
			return Basic::add(this->data, that.data);
		}
		_mm_cpp& operator+=(const _mm_cpp& that) noexcept{
			return (*this = (*this + that));
		}

		template<typename out>
		friend out& MY_LIB operator<<(out& o, const _mm_cpp& m)noexcept {
			for (size_t i = 0;; ++i) {
				o << Basic::depack(m.data)[i];
				if (i < Basic::getNum() - 1)
					o << ' ';
				else break;
			}
			return o;
		}
	private:

	};

}