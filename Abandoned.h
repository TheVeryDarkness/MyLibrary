#pragma once
#include "Shared.h"

namespace LL {
	/*
#define NoNext(a) (a->next==nullptr||a==&NullObject)
	//重载
	template<class Class, typename Data, unsigned long Radix>
	inline void MY_LIBRARY add(
		Class& a, const Class& b
	) noexcept {
		Class NullObject(0, nullptr);
		NullObject.next = &NullObject;
		Class* OprtPtr_a = &a;//a的操作指针
		const Class* OprtPtr_b = &b;//b的操作指针
		if constexpr (Radix == 0)
		{
			while (true)
			{
				OprtPtr_a->data += OprtPtr_b->data;
				if (
					OprtPtr_a->next == nullptr
					&& NoNext(OprtPtr_b)
					)
				{
					OprtPtr_a = OprtPtr_b = nullptr;
					return;
				}
				else if (OprtPtr_a->next == nullptr)
				{
					OprtPtr_a->insert();
				}
				else if (NoNext(OprtPtr_b))
				{
					OprtPtr_b = &NullObject;
				}
				OprtPtr_a = OprtPtr_a->next;
				OprtPtr_b = OprtPtr_b->next;
			}
		}
		//二进制
		else if constexpr (Radix == 2)
		{
			bool Bit = false;//true表示进位或退位
			if (a.next == nullptr || b.next == nullptr)
			{
				return;
			}
			Class CopyThis(a, true);//a的拷贝
			if (OprtPtr_a->data == OprtPtr_b->data)
			{//加法模式

				OprtPtr_a = OprtPtr_a->next;
				OprtPtr_b = OprtPtr_b->next;
				//处理主要数据部分
				do
				{
					//Processing this bit
					if (
						OprtPtr_a->data
						&& OprtPtr_b->data
						&& Bit
						) {
						OprtPtr_a->data = true;
						Bit = true;
					}
					//
					else if (
						(OprtPtr_a->data && OprtPtr_b->data)
						|| (OprtPtr_a->data && Bit)
						|| (OprtPtr_b->data && Bit)
						) {
						OprtPtr_a->data = false;
						Bit = true;
					}
					//
					else if (
						Bit
						|| OprtPtr_b->data
						|| OprtPtr_a->data
						) {
						OprtPtr_a->data = true;
						Bit = false;
					}
					//
					else
					{
						OprtPtr_a->data = false;
						Bit = false;
					}
					//analyzing the next bit.
					if (OprtPtr_a->next == nullptr
						&& NoNext(OprtPtr_b)
						&& !Bit)
					{
						a.data = true;
						break;
					}
					else if (
						OprtPtr_a->next == nullptr
						&& NoNext(OprtPtr_b)
						) {
						OprtPtr_a->insert();
						OprtPtr_b = &NullObject;
					}
					else if (OprtPtr_a->next == nullptr)
					{
						OprtPtr_a->insert();
					}
					else if (NoNext(OprtPtr_b))
					{
						OprtPtr_b = &NullObject;
					}
					OprtPtr_a = OprtPtr_a->next;
					OprtPtr_b = OprtPtr_b->next;
				} while (true);
			}
			else//减法模式
			{
				if (a.data == false)
				{
					OprtPtr_a = OprtPtr_b = nullptr;
					a = b + (CopyThis);
					CopyThis.destruct();
					return;
				}
				OprtPtr_a = OprtPtr_a->next;
				OprtPtr_b = OprtPtr_b->next;
				do
				{
					//处理当前位
					if (OprtPtr_a->data == OprtPtr_b->data && Bit)
					{
						Bit = true;
						OprtPtr_a->data = true;
					}
					else if (OprtPtr_a->data && Bit)
					{
						Bit = false;
						OprtPtr_a->data = false;
					}
					else if (OprtPtr_b->data && Bit)
					{
						Bit = true;
						OprtPtr_a->data = false;
					}
					else if (OprtPtr_a->data == OprtPtr_b->data)
					{
						Bit = false;
						OprtPtr_a->data = false;
					}
					else if (OprtPtr_a->data)
					{
						Bit = false;
						OprtPtr_a->data = true;
					}
					else if (OprtPtr_b->data)
					{
						Bit = true;
						OprtPtr_a->data = true;
					}
					//处理下一位
					if (OprtPtr_a->next == nullptr && NoNext(OprtPtr_b) && !Bit)
					{
						a.data = true;
						break;
					}
					else if (OprtPtr_a->next == nullptr)
					{
						OprtPtr_a = OprtPtr_b = nullptr;
						CopyThis.data = !CopyThis.data;
						Class _b = -b;
						a = -(_b + CopyThis);
						CopyThis.destruct();
						return;
					}
					else if (NoNext(OprtPtr_b))
					{
						OprtPtr_b = &NullObject;
						OprtPtr_a = OprtPtr_a->next;
					}
					else
					{
						OprtPtr_b = OprtPtr_b->next;
						OprtPtr_a = OprtPtr_a->next;
					}
				} while (true);
			}
			CopyThis.destruct();
			OprtPtr_a = OprtPtr_b = nullptr;
			return;
		}
		//普遍情形
		else
		{
			bool Bit = false;//true表示进位或退位
			if (b.next == nullptr)//b为0
			{
				return;
			}
			else if (a.next == nullptr)//a为0，直接将b拷贝到a
			{
				a = Class(b, true);
				return;
			}
			Class CopyThis(a, true);//a的拷贝
			if (OprtPtr_a->data == OprtPtr_b->data)//加法模式
			{//处理符号位
				if (OprtPtr_a->data)//a，b均为正数
				{
					a.data = true;
				}
				else//a，b均为负数
				{
					a.data = false;;
				}
				//后移
				OprtPtr_a = OprtPtr_a->next;
				OprtPtr_b = OprtPtr_b->next;
				//处理数值部分
				do
				{//处理指针指向的当前位
					if (
						(static_cast<unsigned long long>(OprtPtr_a->data)
							+ static_cast<unsigned long long>(OprtPtr_b->data)
							)
						>= Radix - (Bit ? 1ULL : 0ULL)
						)//需要进位
					{
						OprtPtr_a->data =
							Data(
							(
								(unsigned long long)OprtPtr_a->data
								+ (unsigned long long)OprtPtr_b->data
								+ (Bit ? 1ULL : 0ULL)) - Radix);
						Bit = true;
					}
					else//不需要进位
					{
						OprtPtr_a->data =
							(OprtPtr_a->data
								+ OprtPtr_b->data
								+ static_cast<Data>(Bit));
						Bit = false;
					}
					//根据下一位的存在情况，决定所选操作
					if (OprtPtr_a->next == nullptr
						&&
						NoNext(OprtPtr_b)
						&&
						Bit == false)//无需进位且a，b均已结束
					{
						break;
					}
					else if (
						OprtPtr_a->next == nullptr
						&&
						NoNext(OprtPtr_b)
						)//a，b已经结束，但仍需进位
					{
						OprtPtr_a->insert(0);
						OprtPtr_b = &NullObject;
					}
					else if (OprtPtr_a->next == nullptr)//a已经结束，但b未结束
					{
						OprtPtr_a->insert(0);
					}
					else if (NoNext(OprtPtr_b))//
					{
						OprtPtr_b = &NullObject;
					}
					OprtPtr_a = OprtPtr_a->next;
					OprtPtr_b = OprtPtr_b->next;
				} while (true);
			}
			else//减法模式
			{
				if (a.data == false)
				{
					//OprtPtr_a = OprtPtr_b = nullptr;
					a = b + CopyThis;
					CopyThis.destruct();
					return;
				}
				OprtPtr_a = OprtPtr_a->next;
				OprtPtr_b = OprtPtr_b->next;
				do
				{
					//处理当前位
					if (OprtPtr_a->data >= OprtPtr_b->data + Data(Bit))
					{
						OprtPtr_a->data =
							static_cast<unsigned long long>(OprtPtr_a->data)
							- static_cast<unsigned long long>(static_cast<Data>(Bit))
							- static_cast<unsigned long long>(OprtPtr_b->data);
						Bit = false;
					}
					else
					{
						OprtPtr_a->data = Data(
							(unsigned long long)OprtPtr_a->data
							+ Radix
							- (unsigned long long)Data(Bit)
							- (unsigned long long)OprtPtr_b->data);
						Bit = true;
					}
					//处理下一位
					if (OprtPtr_a->next == nullptr && NoNext(OprtPtr_b) && !Bit)
					{
						a.data = 1;
						break;
					}
					else if (OprtPtr_a->next == nullptr)
					{
						CopyThis.data = !CopyThis.data;//取反
						Class _b = -b;
						a = CopyThis + _b;
						a.data = !a.data;
						_b.destruct();
						CopyThis.destruct();
						return;
					}
					else if (NoNext(OprtPtr_b))
					{
						OprtPtr_b = &NullObject;
						OprtPtr_a = OprtPtr_a->next;
					}
					else
					{
						OprtPtr_b = OprtPtr_b->next;
						OprtPtr_a = OprtPtr_a->next;
					}
				} while (true);
			}
			CopyThis.destruct();
			OprtPtr_a = nullptr;
			return;
		}
	}
#ifdef NoNext
#undef NoNext
#endif // NoNext

	
	template<class Class, typename Data, unsigned long Radix>
	void MY_LIBRARY multiply(Class& a, const Class& b) noexcept {
		a.data = (b.data) ? (a.data) : (!a.data);
		Class temp(a, true), _temp(false);
		a.destruct();
		const Class* OprtPtr_b = b.next;
		while (true)
		{
			if (OprtPtr_b == nullptr)
			{
				break;
			}
			_temp = temp * OprtPtr_b->data;
			a += _temp;
			temp <<= 1;
			OprtPtr_b = OprtPtr_b->next;
		}
		_temp.destruct();
		temp.destruct();
	}
	template<class Class, typename Data, unsigned long Radix>
	void MY_LIBRARY multiply(Class& a, int times) noexcept {
		if constexpr (Radix == 0) {
			Class* OprtPtr = &a;
			while (true)
			{
				if (OprtPtr != nullptr)
				{
					OprtPtr->data *= times;
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			a.data = (times >= 0) ?
				a.data : (!(a.data));
			times = (times >= 0) ? times : (-times);
			if (a.next == nullptr)
			{
				return;
			}
			Class* OprtPtr = a.next;
			unsigned long onbit = 0, temp = 0;
			while (true)
			{
				temp = (static_cast<unsigned long long>(OprtPtr->data) * times + onbit)
					/ Radix;
				OprtPtr->data =
					static_cast<unsigned long long>((static_cast<unsigned long long>(OprtPtr->data) * times + onbit) % Radix);
				onbit = temp;
				if (OprtPtr->next == nullptr && onbit == 0) break;
				else if (OprtPtr->next == nullptr)
				{
					OprtPtr->insert(0);
				}
				OprtPtr = OprtPtr->next;
			}
		}
		return;
	}
}*/