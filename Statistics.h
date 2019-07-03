#pragma once

#include <forward_list>

#define Posi(a) (((a)>0)?(a):0)//取正值
template<typename Data>constexpr bool sign(Data data) { return ((data >= 0) ? true : false); }//取符号，0视为正
#define Sign(a) ((a)>0)?(1):((((a)<0)?(-1):(0)))//取符号
#define ABS(a) (((a)>0)?(a):(-(a)))//取绝对值


inline long __stdcall Max(long[], long);
inline long __stdcall Min(long[], long);
inline long __stdcall Average(long[], long);
inline long __cdecl Max(long, long, ...);
inline long __cdecl Min(long, long, ...);
inline long __cdecl Average(long, long, ...);
constexpr inline unsigned __stdcall GetPowerTimes(unsigned long long,unsigned int);
template<typename Data>inline void __stdcall Swap(Data& a,Data& b);
constexpr inline bool IsPrime(int val) {
	for (int i = 1; i*i <= val; i++)
	{
		if (val%i==0)
		{
			return false;
		}
	}
	return true;
}


template<typename Data,Data dt> inline constexpr bool CheckUnsigned() { 
	std::is_convertible<Data, unsigned long long> Cvt; 
	return(Cvt.value && static_cast<long long>(static_cast<Data>(-1)) != -1LL); 
}

//数组版求最大值
inline long __stdcall Max(
	long argument[],
	long MaxIndex//数组最大索引值
) {
	long MaxNumber = argument[0];
	if (MaxIndex == 0)
	{
		return MaxNumber;
	}
	for (long i = 1; i < MaxIndex; i++)
	{
		MaxNumber =
			(MaxNumber > argument[i]) ?
			(MaxNumber) : (argument[i]);
	}
	return MaxNumber;
}


//数组版求最小值
inline long __stdcall Min(
	long argument[],
	long MaxIndex//数组最大索引值
) {
	long MinNumber = argument[0];
	if (MaxIndex == 0)
	{
		return MinNumber;
	}
	for (long i = 1; i < MaxIndex; i++)
	{
		MinNumber =
			(MinNumber < argument[i]) ?
			(MinNumber) : (argument[i]);
	}
	return MinNumber;
}


//数组版求平均值
inline long __stdcall Average(
	long argument[],
	long Account//参数数目
)
{
	long average = 0;
	for (long i = 0; i < Account; i++)
	{
		average += (long)(argument[i] / Account);
	}
	return (long)average;
}

//不定参数
inline long __cdecl Max(
	long num, long data, ...
) {
	return Max(&data, num);
}

//不定参数
inline long __cdecl Min(
	long num, long data, ...
) {
	return Min(&data, num);
}

//不定参数
inline long __cdecl Average(
	long num, long data, ...
) {
	return Average(&data, num);
}

template<typename Type, typename SubData>
//Remember to delete this array
inline SubData* __stdcall GetSubArray(
	const SubData& __stdcall GetFunc(const Type&),
	unsigned int argc,
	const Type * Array)
{
	SubData* SubArray = new SubData[argc]();
	for (unsigned int i = 0; i < argc; i++)
	{
		SubArray[i] = GetFunc(Array[i]);
	}
	return SubArray;
}


//0 for not.
constexpr unsigned inline __stdcall GetPowerTimes(
	unsigned long long val,
	const unsigned int base
) {
	unsigned times = 0;
	while (true)
	{
		if (val % base == 0)
		{
			if (val == base)
			{
				return (++times);
			}
			val /= base;
			times++;
		}
		else return 0;
	}
	return 0;
}

//不判断两数相等
template<typename Data>inline void __stdcall Swap(Data& a, Data& b) {
	a = a ^ b;
	b = a ^ b;
	a = a ^ b;
}