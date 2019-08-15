#pragma once

#ifndef _DEBUG
#define NDEBUG
#endif // !_DEBUG


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
#include <iostream>

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


template<typename type>
class Depack
{
public:
	Depack() = delete;
	~Depack() = delete;
	using TRUE_TYPE=type;
private:

};