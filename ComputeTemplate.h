#pragma once

#include <cstdlib>
#include <iterator>

//The _Traits must give these functions:
//	Linear* GetNext(Linear*);
//	Data& GetData(Linear*);
//	short Flow(Data[3]);
//	short InterFlow(Data[3]);
//	Data Sum(Data[3]);
//	Data InterSum(Data[3]);
//	void InsertAfter(Linear**);//However, it doen't need to insert an element after it
//When an element doesn't a next element, GetNext(Linear*) should return nullptr.
//
//The _Traits must give these definition:
//	Null;
//An object constructed with Null must have 0 data, and not have an next element.
namespace LongCompute {
	template<typename Linear, typename Data,class _Traits>
	inline bool Iterate(const Linear& NullObject,const Linear*& That, Linear*& This,const Data& Flow) {
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
			if (Flow == 0)
			{
				return false;
			}
			else
			{
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
		Linear NullObject(_Traits::Null);
		Data Flow = _Traits::Null;
		while (true)
		{
			//This element
			Data temp = _Traits::Flow(_Traits::GetData(OprtPtrA), _Traits::GetData(OprtPtrB), Flow);
			_Traits::GetData(OprtPtrB) = _Traits::Sum( _Traits::GetData(OprtPtrA) ,_Traits::GetData(OprtPtrB),Flow);
			Flow = temp;
			if (!Iterate<Linear, Data, _Traits>(NullObject, OprtPtrA, OprtPtrB, Flow))
			{
				break;
			}
			}
		}
		template<typename Linear, typename Data, class _Traits>
		inline void SubtractFrom(const Linear& a, Linear& b) {
			const Linear* OprtPtrA = &a;
			Linear* OprtPtrB = &b;
			Linear NullObject(_Traits::Null);
			Data Flow = _Traits::Null;
			while (true)
			{
				//This element
				Data temp = _Traits::InterFlow(_Traits::GetData(OprtPtrA), _Traits::GetData(OprtPtrB), Flow);
				_Traits::GetData(OprtPtrB) = _Traits::InterSum(_Traits::GetData(OprtPtrA), _Traits::GetData(OprtPtrB), Flow);
				Flow = temp;
				Iterate<Linear, Data, _Traits>(NullObject, OprtPtrA, OprtPtrB, Flow);
			}
		}
	
}