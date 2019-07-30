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

template<typename Ptr> INLINED void PtrDelete(Ptr ptr) { if (ptr != nullptr)delete ptr; }

#ifdef _DEBUG
#define NOMINMAX
#include <crtdbg.h>
#ifdef max
#undef max
#endif // max
#ifdef min
#undef min
#endif // min
#ifndef DBG_NEW
#define DBG_NEW ::new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif 
#else
#ifndef DBG_NEW
#define DBG_NEW new
#endif // !DBG_NEW
#endif  // _DEBUG