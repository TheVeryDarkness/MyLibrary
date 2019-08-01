#pragma once


#ifdef _DEBUG
#ifndef DEBUG_OUT
#define NOMINMAX
#include <Windows.h>
#define DEBUG_OUT OutputDebugStringA((std::string(__FILE__) + " " + std::to_string(__LINE__) + "\n").c_str())
#endif // !DEBUG_OUT
#else
#define DEBUG_OUT
#define OutputDebugStringA(x)
#define OutputDebugStringW(x)
#define OutputDebugString(x)
#endif // _DEBUG

#ifdef _DEBUG
#define NOMINMAX
#include <crtdbg.h>
#ifndef DBG_NEW
#if 1
#define DBG_NEW ::new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif //1
#endif  //DBG_NEW
#else
#ifndef DBG_NEW
#define DBG_NEW new
#endif // !DBG_NEW
#endif  // _DEBUG


#include <cassert>
#ifdef _DEBUG
#define MY_ASSERT(x,y) if(!(x))OutputDebugStringA(#y);assert(x);
#else
#define MY_ASSERT(x,y); 
#endif