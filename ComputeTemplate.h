#pragma once

namespace LongCompute {
	constexpr short Larger = 0x1;
	constexpr short Equal = 0x0;
	constexpr short Smaller = -0x1;



	//The _Traits must give these functions:
	//	Iterator GetNext(Iterator);
	//	Data& GetData(Iterator);
	//	void Add(Data&, bool, const Data, const Data);
	//	void Subtract(Data&, bool, const Data, const Data)
	//	Linear operator*(Iterator*);
	//	void InsertAfter(Iterator*);//However, it doen't need to insert an element after it
	//When an element doesn't a next element, GetNext(Iterator) should return NullIterator.
	//
	//The _Traits must give these definition:
	//	NullIterator;
	//NullIterator must have 0 data, and not have an next element.

	template<typename Iterator, typename Data, class _Traits>
	inline bool Iterate(const Iterator & NullIterator, Iterator& That, Iterator& This, const Data& CarryBit) {
		//Next element
		if (((_Traits::GetNext(That)) != NullIterator) && ((_Traits::GetNext(This)) != NullIterator))
		{
			That = (_Traits::GetNext(That));
			This = (_Traits::GetNext(This));
		}
		else if (((_Traits::GetNext(That)) == NullIterator) && ((_Traits::GetNext(This)) != NullIterator))
		{
			That = NullIterator;
			This = (_Traits::GetNext(This));
		}
		else if (((_Traits::GetNext(That)) != NullIterator) && ((_Traits::GetNext(This)) == NullIterator))
		{
			_Traits::InsertAfter(&This);
			That = (_Traits::GetNext(That));
			This = (_Traits::GetNext(This));
		}
		else if (((_Traits::GetNext(That)) == NullIterator) && ((_Traits::GetNext(This)) == NullIterator))
		{
			if (CarryBit == 0) {
				return false;
			}
			else {
				That = NullIterator;
				_Traits::InsertAfter(&This);
				This = (_Traits::GetNext(This));
			}
		}
		return true;
	}

	template<typename Iterator, typename Data, class _Traits>
	inline void AddTo(Iterator a, Iterator b) {
		Data Carry = 0;
		while (true)
		{
			//This element
			_Traits::Add(_Traits::GetData(b), Carry, _Traits::GetData(a), _Traits::GetData(b));
			if (!Iterate<Iterator, Data, _Traits>(_Traits::NullIterator, a, b, Carry))
			{
				break;
			}
		}
	}
	template<typename Iterator, typename Data, class _Traits>
	inline void SubtractFrom(Iterator a, Iterator b) {
		Data Carry = 0;
		while (true)
		{
			//This element
			_Traits::SubTract(_Traits::GetData(b), Carry, _Traits::GetData(a), _Traits::GetData(b));
			if (!Iterate<Iterator, Data, _Traits>(_Traits::NullIterator, a, b, Carry)) {
				break;
			}
		}
	}
	template<typename Linear,typename Iterator, typename Data, class _Traits>
	inline void DivideInto(Linear& Res,Iterator a, Iterator b) {
		{
			if (Compare<Iterator, Data, _Traits>(a, b) == Larger) {
				return;
			}
			else{
				DivideInto<Linear,Iterator, Data, _Traits>(Res,a, _Traits::GetNext(b));
				Res <<= 1;
			}
		}
		do
		{
			//Regarding of the compatibility, we didn't use any majorization.
			SubtractFrom<Iterator, Data, _Traits>(a, b);
			++Res;
		} while (Compare<Iterator, Data, _Traits>(a, b) == Smaller);
	}
	template<typename Iterator, typename Data, class _Traits>
	short __stdcall Compare(const Iterator& a, const Iterator& b) {
		short PreRes = Compare<Iterator, Data,_Traits>(_Traits::GetNext(a), _Traits::GetNext(a));
		if (PreRes!=0)
		{
			return PreRes;
		}
		else
		{
			return (
				(_Traits::GetData(a) > _Traits::GetData(a))
				?
				Larger
				:
				(
				(_Traits::GetData(a) < _Traits::GetData(a))
					?
					Smaller
					:
					Equal
					)
				);
		}
	}
}