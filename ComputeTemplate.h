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
	//	void InsertAfter(Iterator*);//However, it doen't need to insert an element after it
	//When an element doesn't a next element, GetNext(Iterator) should return NullIterator.
	//
	//The _Traits must give these definition:
	//	NullIterator;
	//NullIterator must have 0 data, and not have an next element.

	template<typename Iterator, typename Data, class _Traits>
	inline bool MY_LIBRARY Iterate(Iterator& That, Iterator& This, const Data& CarryBit) {
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
			_Traits::InsertAfter(&This);
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
				_Traits::InsertAfter(&This);
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
			if (!Iterate<Iterator, Data, _Traits>(a, b, 0)) {
				break;
			}
		}
	}
	template<typename Linear, typename Iterator, typename Data, class _Traits>
	inline void MY_LIBRARY DivideInto(Linear& Res, Iterator a, Iterator b) {
		{
			switch (Compare<Iterator, Data, _Traits>(a, b))
			{
			case Larger:
				return;
			case Equal:
				SubtractFrom<Iterator, Data, _Traits>(a, b);
				++Res;
				return;
			case Smaller:
				DivideInto<Iterator, Data, _Traits>(a, _Traits::GetNext(b));
				_Traits::assign(&Res, 1);
				break;
			default:
				break;
			}
		}
		while (Compare<Iterator, Data, _Traits>(a, b) != Larger)
		{
			//Regarding of the compatibility, we didn't use any majorization.
			SubtractFrom<Iterator, Data, _Traits>(a, b);
			++Res;
		}
	}
	template<typename Iterator, typename Data, class _Traits>
	inline void MY_LIBRARY DivideInto(Iterator a, Iterator b) {
		{
			switch (Compare<Iterator, Data, _Traits>(a, b))
			{
			case Larger:
				return;
			case Equal:
				SubtractFrom<Iterator, Data, _Traits>(a, b);
				return;
			case Smaller:
				DivideInto<Iterator, Data, _Traits>(a, _Traits::GetNext(b));
				break;
			default:
				break;
			}
		}
		while (Compare<Iterator, Data, _Traits>(a, b) != Larger)
		{
			//Regarding of the compatibility, we didn't use any majorization.
			SubtractFrom<Iterator, Data, _Traits>(a, b);
		}
	}
	//Compare a to b.
	template<typename Iterator, typename Data, class _Traits>
	inline short MY_LIBRARY Compare(const Iterator& a, const Iterator& b) {
		if ((a == _Traits::NullIterator) && (b == _Traits::NullIterator))
		{
			return Equal;
		}
		short PreRes = Compare<Iterator, Data, _Traits>(_Traits::GetNext(a), _Traits::GetNext(b));
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
}