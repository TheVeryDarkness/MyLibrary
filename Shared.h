#pragma once

#ifndef _DEBUG
#ifndef NDEBUG

#define NDEBUG

#endif // !NDEBUG
#endif // !_DEBUG


#ifdef _DEBUG
#define DEBUG_FLAG 0
#else 
#define DEBUG_FLAG 1
#endif // _DEBUG

#ifdef _DEBUG
#define _free_dbg(x,_NORMAL_BLOCK) free(x)
#else
#undef _free_dbg
#endif // _DEBUG


#if !defined(MY_LIB)
#if defined(_WIN32)
#define MY_LIB __stdcall
//#endif // _WIN32
#elif defined(linux)
#define MY_LIB __attribute__((__stdcall__))
#else
#define MY_LIB
#endif // linux
#endif // !MY_LIB

#ifndef INLINED
#define INLINED inline
#endif

#ifndef ERR
#ifdef _DEBUG
#define ERR(x) std::cerr << x
#else
#define ERR(x) 

#endif // _DEBUG
#endif // !ERR

#ifndef ASSERT
#ifdef _DEBUG
#define ASSERT(x,y) {if(!(y)) { std::cerr << x << std::endl; } assert(y);}
#else
#define ASSERT(x, y)

#endif // _DEBUG
#endif // !ASSERT


#ifdef __GNUC__
#ifndef _GLIBCXX_HAVE_EOVERFLOW
#define _GLIBCXX_HAVE_EOVERFLOW 1
#endif // !_GLIBCXX_HAVE_EOVERFLOW
#ifndef EOVERFLOW
#define EOVERFLOW 18533
#endif // !EOVERFLOW
#endif // __GNUC__

#define RV_DISPLAY_ON 0

template<typename T>
constexpr inline const T &RV_DISPLAY(const T &t)noexcept {
#if _DEBUG && RV_DISPLAY_ON
	return std::cout << t << std::endl, t;
#else
	return t;
#endif // _DEBUG && RV_DISPLAY_ON
}

template<typename T>
constexpr inline const T &RV_PAIR_DISPLAY(const T &t)noexcept {
#if _DEBUG && RV_DISPLAY_ON
	return std::cout << t.first << ' ' << t.second << std::endl, t;
#else
	return t;
#endif // _DEBUG && RV_DISPLAY_ON
}

#ifndef NSHOW
#define wSHOW(x) std::wcout << #x << " = " << (x) << std::endl;
#define SHOW(x) std::cout << #x << " = " << (x) << std::endl;
#define DO(x) {x; std::cout << #x << std::endl << (x) << std::endl;}
#endif // !NSHOW


template<typename type>
class Depack {
public:
	Depack() = delete;
	~Depack() = delete;
	using TRUE_TYPE=type;
private:

};
template<typename _Ty>
using Depack_t=typename Depack<_Ty>::TRUE_TYPE;