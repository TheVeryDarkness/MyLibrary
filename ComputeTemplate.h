#pragma once

//The _Traits must give these functions:
//	Iterator GetNext(Iterator);
//	Data& GetData(Iterator);
//	void Add(Data&, bool, const Data, const Data);
//	void Subtract(Data&, bool, const Data, const Data)
//	size_t GetRemainedNonNullLength(const Iterator);
//	void InsertAfter(Iterator*);//However, it doen't need to insert an element after it
//When an element doesn't a next element, GetNext(Iterator) should return NullIterator.
//
//The _Traits must give these definition:
//	NullIterator;
//NullIterator must have 0 data, and not have an next element.
namespace LongCompute {
	template<typename Iterator, typename Data, class _Traits>
	inline bool Iterate(const Iterator const &NullIterator, Iterator& That, Iterator& This, const Data& CarryBit) {
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
		Iterator OprtPtrA = a;
		Iterator OprtPtrB = b;
		bool Carry = false;
		while (true)
		{
			//This element
			_Traits::Add(_Traits::GetData(OprtPtrB), Carry, _Traits::GetData(OprtPtrA), _Traits::GetData(OprtPtrB));
			if (!Iterate<Iterator, Data, _Traits>(_Traits::NullIterator, OprtPtrA, OprtPtrB, Carry))
			{
				break;
			}
		}
	}
	template<typename Iterator, typename Data, class _Traits>
	inline void SubtractFrom(const Iterator& a, Iterator& b) {
		const Iterator OprtPtrA = &a;
		Iterator OprtPtrB = &b;
		bool Carry = false;
		while (true)
		{
			//This element
			_Traits::SubTract(_Traits::GetData(OprtPtrB), Carry, _Traits::GetData(OprtPtrA), _Traits::GetData(OprtPtrB));
			if (!Iterate<Iterator, Data, _Traits>(_Traits::NullIterator, OprtPtrA, OprtPtrB, Carry)) {
				break;
			}
		}
	}
	template<typename Iterator, typename Data, class _Traits>
	inline void DivideInto(Iterator& Res,const Iterator& a, Iterator& b) {
		{
			size_t LenA = _Traits::GetRemainedNonNullLength(&a), LenB = _Traits::GetRemainedNonNullLength(&b);
			if (a>b){
				return;
			}
			else{
				DivideInto(Res,a, _Traits::GetNext(b));
				Res <<= 1;
			}
		}
		do
		{
			//Regarding of the compatibility, we didn't use any majorization.
			b -= a;
		} while (a<b);
	}
}