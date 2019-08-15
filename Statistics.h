#pragma once

#include "Shared.h"
#include <forward_list>

#define Posi(a) (((a)>0)?(a):0)//取正值
template<typename Data>constexpr bool sign(Data data) noexcept{ return ((data >= 0) ? true : false); }//取符号，0视为正
#define Sign(a) ((a)>0)?(1):((((a)<0)?(-1):(0)))//取符号
template<typename Data>
constexpr inline Data ABS(Data a) noexcept {
	if constexpr (Data(-1) > 0) {
		return a;
	}
	else {
		if (a >= 0)
			return a;
		else return (-a);
	}
}
//#define ABS(a) (((a)>=0)?(a):(-(a)))//取绝对值

namespace LargeInteger {
	constexpr unsigned char BitsPerByte = 8;
}
template<typename value_type>size_t getBits(const value_type& that)noexcept {
	value_type temp = 1;
	size_t i = 0;
	for (; i < sizeof(value_type) * LargeInteger::BitsPerByte && ((temp & that) == 0); i++) {
		temp <<= 1;
	}
	return i;
}

inline long __stdcall Max(long[], long);
inline long __stdcall Min(long[], long);
inline long __stdcall Average(long[], long);
inline long __cdecl Max(long, long, ...);
inline long __cdecl Min(long, long, ...);
inline long __cdecl Average(long, long, ...);
constexpr inline unsigned MY_LIBRARY GetPowerTimes(unsigned long long,const unsigned int&);
template<typename Data>inline void __stdcall Swap(Data& a,Data& b);
template<typename Data>constexpr inline bool IsPrime(Data val); 
template<typename Data>inline std::forward_list<Data> PrimeList(Data Max);



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
template<typename VAL,typename BASE>
constexpr unsigned inline MY_LIBRARY GetPowerTimes(
	VAL val,
	const BASE& base
) {
	if (val == 0)
	{
		return 0;
	}
	BASE times = 0;
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

template<typename Data>
constexpr Data Power(Data in,size_t times) noexcept{
	if (times == 0)return Data(1);
	Data In = in;
	for (size_t i = 1; i < times; i++)
	{
		in *= In;
	}
	return in;
}

//不判断两数相等
template<typename Data>inline void __stdcall Swap(Data& a, Data& b) {
	a ^= b;
	b ^= a;
	a ^= b;
}

template<typename Data>constexpr inline bool IsPrime(Data val){
	for (Data i = 2; i * i <= val; i++)
	{
		if (val % i == 0)
		{
			return false;
		}
	}
	return true;
}

template<typename Data>inline std::forward_list<Data> PrimeList(Data Max){
	std::forward_list<Data> List;
	for (Data i = 1; i < Max; i++)
	{
		if (IsPrime(i))
			List.push_front(i);
	}
	return List;
}

template<typename Data>inline std::forward_list<Data> PrimeFactorList(Data Max) {
	std::forward_list<Data> List;
	for (Data i = Max; i >1; i--)
	{
		if (IsPrime(i) && (Max % i == 0))
			List.push_front(i);
	}
	return List;
}

template<typename Data>inline Data MinConti(const std::forward_list<Data>& BigToSmallSorted) {
	if (BigToSmallSorted.empty())
	{
		return static_cast<Data>(1);
	}
	Data PreData = *BigToSmallSorted.begin();
	for (auto i:BigToSmallSorted){
		if (i != (++PreData))
		{
			return (--PreData);
		}
	}
	return PreData;
}