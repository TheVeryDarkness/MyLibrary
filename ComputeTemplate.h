#pragma once

#include "Shared.h"

namespace LongCompute {
	constexpr short Larger = 0x1;
	constexpr short Equal = 0x0;
	constexpr short Smaller = -0x1;



	//The _Traits must give these functions:
	//	Iterator GetNext(Iterator);
	//	Data& GetData(Iterator);
	//	void Add(Data&, bool, Data, Data);
	//	void Subtract(Data&, bool, Data, Data)
	//	void assign(Linear*, size_t);
	//	void InsertAfter(Iterator*, Data);//However, it doen't need to insert an element after it
	//When an element doesn't a next element, GetNext(Iterator) should return NullIterator.
	//
	//The _Traits must give these definition:
	//	NullIterator;
	//NullIterator must have 0 data, and not have an next element.

	template<typename Iterator, typename Data, class _Traits>
	inline bool MY_LIBRARY Iterate(Iterator& That, Iterator& This, Data& CarryBit) {
		//Next element
		Iterator ThatNext = (_Traits::GetNext(That)), ThisNext = (_Traits::GetNext(This));
		if ((ThatNext != _Traits::NullIterator) && (ThisNext != _Traits::NullIterator))
		{
			That = ThatNext;
			This = ThisNext;
		}
		else if ((ThatNext == _Traits::NullIterator) && (ThisNext != _Traits::NullIterator))
		{
			That = _Traits::NullIterator;
			This = ThisNext;
		}
		else if ((ThatNext != _Traits::NullIterator) && (ThisNext == _Traits::NullIterator))
		{
			_Traits::InsertAfter(&This, CarryBit);
			CarryBit = 0;
			That = ThatNext;
			This = (_Traits::GetNext(This));
		}
		else if ((ThatNext == _Traits::NullIterator) && (ThisNext == _Traits::NullIterator))
		{
			if (CarryBit == 0) {
				return false;
			}
			else {
				That = _Traits::NullIterator;
				_Traits::InsertAfter(&This, CarryBit);
				CarryBit = 0;
				return false;
				This = (_Traits::GetNext(This));
			}
		}
		return true;
	}

	template<typename Iterator, typename Data, class _Traits>
	inline void MY_LIBRARY AddTo(Iterator a, Iterator b) {
		Data Carry = 0;
		while (true)
		{
			//This element
			Data temp;
			_Traits::AddTo(temp, Carry, _Traits::GetData(a), _Traits::GetData(b));
			_Traits::GetData(b) = temp;
			if (!Iterate<Iterator, Data, _Traits>(a, b, Carry))
			{
				break;
			}
		}
	}
	template<typename Iterator, typename Data, class _Traits>
	inline void MY_LIBRARY SubtractFrom(Iterator a, Iterator b) {
		Data Carry = 0;
		while (true)
		{
			//This element
			Data temp;
			_Traits::SubTractFrom(temp, Carry, _Traits::GetData(a), _Traits::GetData(b));
			_Traits::GetData(b) = temp;
			//Carry a bit can never solve it, so we don't pass it to the function.
			if (!Iterate<Iterator, Data, _Traits>(a, b, Carry)) {
				break;
			}
		}
	}
	template<typename Call, typename Iterator, typename Data, class _Traits>
	inline void MY_LIBRARY VirtualMultiplyTo(Call call, Data Carry,Iterator a, Iterator b) {
		Data Res;
		while (true)
		{
			_Traits::Multiply(Res, Carry, _Traits::GetData(a), _Traits::GetData(b));
			if (!Iterate<Iterator, Data, _Traits>(a, b, Carry)) {
				break;
			}
		}
	}

	template<typename Accumulation, typename Recursion, typename Iterator, typename Data, class _Traits>
	inline void MY_LIBRARY __DivideInto(Accumulation Func1, Recursion Func2, Iterator _a, Iterator _b) {
		{
			switch (CompareTo<Iterator, Data, _Traits>(_a, _b))
			{
			case Larger:
				return;
			case Equal:
				Func1();
				return;
			case Smaller:
				DivideInto<Iterator, Data, _Traits>(_a, _Traits::GetNext(_b));
				Func2();
				break;
			default:
				break;
			}
		}
		while (true)
		{
			auto [res, cmpr] = _CompareTo<Iterator, Data, _Traits>(_b, _a);
			if (cmpr == Smaller)
			{
				return;
			}
			else if (cmpr == Equal)
			{
				Func1();
				return;
			}
			for (Data i = 0; i < res; i++)
			{
				Func1();
			}
		}
	}
	template<typename Linear, typename Iterator, typename Data, class _Traits>
	inline void MY_LIBRARY DivideInto(Linear& Res, Iterator a, Iterator b) {
		//Regarding of the compatibility, we didn't use any majorization.
		auto func1 = [&a, &b, &Res]()->void {SubtractFrom<Iterator, Data, _Traits>(a, b); Res++; };
		auto func2 = [&Res]()->void {_Traits::assign(&Res, 1); };
		__DivideInto<decltype(func1), decltype(func2), Iterator, Data, _Traits>(func1, func2, a, b);
	}
	template<typename Iterator, typename Data, class _Traits>
	inline void MY_LIBRARY DivideInto(Iterator a, Iterator b) {
		//Regarding of the compatibility, we didn't use any majorization.
		auto func = [&a, &b]()->void {SubtractFrom<Iterator, Data, _Traits>(a, b); };
		auto null = []()->void {};
		__DivideInto<decltype(func), decltype(null), Iterator, Data, _Traits>(func, null, a, b);
	}
	//Compare a to b.
	template<typename Iterator, typename Data, class _Traits>
	inline short MY_LIBRARY CompareTo(const Iterator& a, const Iterator& b) {
		if ((a == _Traits::NullIterator) && (b == _Traits::NullIterator))
		{
			return Equal;
		}
		short PreRes = CompareTo<Iterator, Data, _Traits>(_Traits::GetNext(a), _Traits::GetNext(b));
		if (PreRes != Equal)
		{
			return PreRes;
		}
		else
		{
			return (
				(_Traits::GetData(a) > _Traits::GetData(b))
				?
				Larger
				:
				(
				(_Traits::GetData(a) < _Traits::GetData(b))
					?
					Smaller
					:
					Equal
					)
				);
		}
	}
	//Extension for Compare()
	template<typename Iterator, typename Data, class _Traits>
	inline std::pair<Data, short> MY_LIBRARY _CompareTo(const Iterator& a, const Iterator& b) {
		if (!((_Traits::GetNext(a) == _Traits::NullIterator) && (_Traits::GetNext(b) == _Traits::NullIterator)))
		{
			auto PreRes = _CompareTo<Iterator, Data, _Traits>(_Traits::GetNext(a), _Traits::GetNext(b));
			if (PreRes.second != Equal)
			{
				return PreRes;
			}
		}
		{
			return (
				(_Traits::GetData(a) > _Traits::GetData(b))
				?
				std::pair(1, Larger)
				:
				(
				(_Traits::GetData(a) < _Traits::GetData(b))
					?
					std::pair(1, Smaller)
					:
					std::pair(0, Equal)
					)
				);
		}
	}
	template<typename Data>
	class StandardComputeTraits
	{
	public:
		StandardComputeTraits() = delete;
		~StandardComputeTraits() = delete;
		static void AddTo(Data& Res, Data& Carry, Data a, Data b) {
			Res = a + b + Carry;
			if (Carry > 0)
			{
				Carry = ((a > Data(~Data(b + 1))) || (b > Data(~Data(1))) ? 1 : 0);
			}
			else
			{
				Carry = ((a > Data(~b)) ? 1 : 0);
			}
		}
		static void SubTractFrom(Data& Res, Data& Carry, Data a, Data b) {
			Res = a - b - Carry;
			if (Carry > 0)
			{
				Carry = ((a <= b) ? 1 : 0);
			}
			else
			{
				Carry = ((a < b) ? 1 : 0);
			}
		}
	private:

	};

}