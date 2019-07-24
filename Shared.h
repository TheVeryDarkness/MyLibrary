#pragma once

template<typename Ptr> inline void PtrDelete(Ptr ptr) { if (ptr != nullptr)delete ptr; }

#ifdef _DEBUG
#define DEBUG_FLAG 0
#endif // _DEBUG


#ifndef MY_LIBRARY
#define MY_LIBRARY __stdcall
#endif // !MY_LIBRARY

#ifdef _DEBUG
#include <crtdbg.h>
#undef max
#undef min
#ifndef DBG_NEW
#define DBG_NEW ::new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif 
#else
#ifndef DBG_NEW
#define DBG_NEW new
#endif // !DBG_NEW
#endif  // _DEBUG

//#undef max
//#undef min