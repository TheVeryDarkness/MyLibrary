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


#include <cassert>
#ifdef _DEBUG
#define MY_ASSERT(x,y); if(!(x))OutputDebugStringA(y);assert(x);
#else
#define MY_ASSERT(x,y); 
#endif