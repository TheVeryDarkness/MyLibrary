#pragma once


#ifdef _DEBUG
#define DEBUG_FLAG 0
#else 
#define DEBUG_FLAG 1
#endif // _DEBUG


#ifndef MY_LIBRARY
#define MY_LIBRARY __stdcall
#endif // !MY_LIBRARY

#ifndef INLINED
#define INLINED inline
#endif

#include <cassert>
#ifndef ERR
#ifdef _DEBUG
#define ERR(x) std::cerr << x
#else
#define ERR(x) 
#endif // _DEBUG
#ifndef ASSERT
#define ASSERT(x,y) if(!!(y)) { std::cerr << x << std::endl; } assert(y);
#endif // !ASSERT
#endif // !ERR

template<typename Ptr> INLINED void PtrDelete(Ptr ptr) { if (ptr != nullptr)delete ptr; }


template<typename type>
class Depack
{
public:
	Depack() = delete;
	~Depack() = delete;
	using TRUE_TYPE=type;
private:

};