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


template<typename type>
class Depack
{
public:
	Depack() = delete;
	~Depack() = delete;
	using TRUE_TYPE=type;
private:

};