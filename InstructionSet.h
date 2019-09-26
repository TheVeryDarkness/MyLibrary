#pragma once

#include <intrin.h>
namespace Math {
	enum class Align {
		_512, _256, _128, _64
	};

	template<typename T, Align align>
	class base {
	public:
		using type=T;
		static constexpr size_t getNum()noexcept { 
			static_assert(false, "No matched type");
			return 1; 
		}
		static constexpr type store(T t)noexcept {
			return t;
		}
		static constexpr T* depack(type& t)noexcept {
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
		using type=__m64;
		static constexpr size_t getNum()noexcept { return 2; }
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
	};
	template<>class base<__int32, Align::_512> {
	public:
		using type=__m512i;
		static constexpr size_t getNum()noexcept { return 16; }
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
	};
	template<>class base<__int64,Align::_512> {
	public:
		using type=__m512i;
		static constexpr size_t getNum()noexcept { return 8; }
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




	template<typename T, Align align>
	class Compute {
	public:
		Compute() = delete;
		~Compute() = delete;
		using target=typename base<T, align>::type;
		using source=T;



	private:

	};


	template<Align align, typename T>
	class _mm_cpp {
	public:
		using Basic=base<T, align>;
		typename Basic::type data;
		template<typename Induct>_mm_cpp(Induct ind)noexcept:data(Basic::store(ind)) { }
		static void* operator new(size_t sz) {

		}
		~_mm_cpp() { }

		template<typename out>
		friend out& MY_LIB operator<<(out& o, const _mm_cpp& m)noexcept {
			for (size_t i = 0; i < Basic::getNum() && o << ' '; ++i) {
				o << Basic::depack(m.data)[i];
			}
			return o;
		}
	private:

	};

}