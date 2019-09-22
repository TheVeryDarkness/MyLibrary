#pragma once

#include <intrin.h>
namespace Math {
	enum class Align {
		_512, _256, _128, _64
	};

	template<typename T, Align align>
	class Store {
	public:
		using type=T;
		static constexpr size_t getNum()noexcept { return 1; }
	};

	//__int8
	class Store<__int8,Align::_64> {
	public:
		using type=__m64;
		static constexpr size_t getNum()noexcept { return 8; }
	};
	class Store<__int8, Align::_128> {
	public:
		using type=__m128i;
		static constexpr size_t getNum()noexcept { return 16; }
	};
	class Store<__int8, Align::_256> {
	public:
		using type=__m256i;
		static constexpr size_t getNum()noexcept { return 32; }
	};
	class Store<__int8, Align::_512> {
	public:
		using type=__m512i;
		static constexpr size_t getNum()noexcept { return 64; }
	};


	//__int16
	class Store<__int16,Align::_64> {
	public:
		using type=__m64;
		static constexpr size_t getNum()noexcept { return 4; }
	};
	class Store<__int16, Align::_128> {
	public:
		using type=__m128i;
		static constexpr size_t getNum()noexcept { return 8; }
	};
	class Store<__int16,Align::_256> {
	public:
		using type=__m256i;
		static constexpr size_t getNum()noexcept { return 16; }
	};
	class Store<__int16, Align::_512> {
	public:
		using type=__m512i;
		static constexpr size_t getNum()noexcept { return 32; }
	};


	//__int32
	class Store<__int32,Align::_64> {
	public:
		using type=__m64;
		static constexpr size_t getNum()noexcept { return 2; }
	};
	class Store<__int32, Align::_128> {
	public:
		using type=__m128i;
		static constexpr size_t getNum()noexcept { return 4; }
	};
	class Store<__int32, Align::_256> {
	public:
		using type=__m256i;
		static constexpr size_t getNum()noexcept { return 8; }
	};
	class Store<__int32, Align::_512> {
	public:
		using type=__m512i;
		static constexpr size_t getNum()noexcept { return 16; }
	};


	//__int64
	class Store<__int64,Align::_128> {
	public:
		using type=__m128i;
		static constexpr size_t getNum()noexcept { return 2; }
	};
	class Store<__int64,Align::_256> {
	public:
		using type=__m256i;
		static constexpr size_t getNum()noexcept { return 4; }
	};
	class Store<__int64,Align::_512> {
	public:
		using type=__m512i;
		static constexpr size_t getNum()noexcept { return 8; }
	};


	//float
	class Store<float, Align::_128> {
	public:
		using type=__m64;
		static constexpr size_t getNum()noexcept { return 2; }
	};
	class Store<float, Align::_128> {
	public:
		using type=__m64;
		static constexpr size_t getNum()noexcept { return 2; }
	};
	class Store<float, Align::_128> {
	public:
		using type=__m64;
		static constexpr size_t getNum()noexcept { return 2; }
	};
	class Store<float, Align::_128> {
	public:
		using type=__m64;
		static constexpr size_t getNum()noexcept { return 2; }
	};

	template<typename T, size_t lengthInBits = 512>
	class _m__helper {
	public:
		_m__helper() = delete;
		~_m__helper() = delete;
		
	private:
	};
}