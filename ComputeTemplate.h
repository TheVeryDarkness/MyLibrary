#pragma once

#include <cstdlib>
#include <iterator>

//The _Traits must give these functions:
//	Linear* GetNext(Linear*);
//	Data& GetData(Linear*);
//	void Add(Data&,bool,const Data,const Data);
//	void Subtract(Data&,bool,const Data, const Data)
//	void InsertAfter(Linear**);//However, it doen't need to insert an element after it
//When an element doesn't a next element, GetNext(Linear*) should return nullptr.
//
//The _Traits must give these definition:
//	NullObject;
//NullObject must have 0 data, and not have an next element.
namespace LongCompute {
	template<typename Linear, typename Data, class _Traits>
	inline bool Iterate(const Linear& NullObject, const Linear*& That, Linear*& This, const Data& CarryBit) {
		//Next element
		if (((_Traits::GetNext(That)) != nullptr) && ((_Traits::GetNext(This)) != nullptr))
		{
			That = (_Traits::GetNext(That));
			This = (_Traits::GetNext(This));
		}
		else if (((_Traits::GetNext(That)) == nullptr) && ((_Traits::GetNext(This)) != nullptr))
		{
			That = &NullObject;
			This = (_Traits::GetNext(This));
		}
		else if (((_Traits::GetNext(That)) != nullptr) && ((_Traits::GetNext(This)) == nullptr))
		{
			_Traits::InsertAfter(&This);
			That = (_Traits::GetNext(That));
			This = (_Traits::GetNext(This));
		}
		else if (((_Traits::GetNext(That)) == nullptr) && ((_Traits::GetNext(This)) == nullptr))
		{
			if (CarryBit == 0){
				return false;
			}
			else{
				That = &NullObject;
				_Traits::InsertAfter(&This);
				This = (_Traits::GetNext(This));
			}
		}
		return true;
	}

	template<typename Linear, typename Data, class _Traits>
	inline void AddTo(const Linear* a, Linear* b) {
		const Linear* OprtPtrA = a;
		Linear* OprtPtrB = b;
		bool Carry = false;
		while (true)
		{
			//This element
			_Traits::Add(_Traits::GetData(OprtPtrB), Carry, _Traits::GetData(OprtPtrA), _Traits::GetData(OprtPtrB));
			if (!Iterate<Linear, Data, _Traits>(_Traits::NullObject, OprtPtrA, OprtPtrB, Carry))
			{
				break;
			}
		}
	}
	template<typename Linear, typename Data, class _Traits>
	inline void SubtractFrom(const Linear& a, Linear& b) {
		const Linear* OprtPtrA = &a;
		Linear* OprtPtrB = &b;
		bool Carry = false;
		while (true)
		{
			//This element
			_Traits::SubTract(_Traits::GetData(OprtPtrB), Carry, _Traits::GetData(OprtPtrA), _Traits::GetData(OprtPtrB));
			if (!Iterate<Linear, Data, _Traits>(_Traits::NullObject, OprtPtrA, OprtPtrB, Carry)) {
				break;
			}
		}
	}

}