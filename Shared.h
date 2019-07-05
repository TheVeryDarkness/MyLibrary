#pragma once

template<typename Ptr> inline void PtrDelete(Ptr ptr) { if (ptr != nullptr)delete ptr; }

#ifndef MY_LIBRARY
#define MY_LIBRARY __stdcall
#endif // !MY_LIBRARY