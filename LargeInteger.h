#pragma once

#include "Shared.h"
#include "Statistics.h"
#include <cassert>

namespace LargeInteger {
	template<typename LL, auto radix>
	class LargeUnsigned
	{
		using radix_t=decltype(radix);
	public:
		template<typename val>
		LargeUnsigned(val Val)noexcept	{
			for (auto& i = LinkedList.begin();; ++i)
			{
				i = Val % radix;
				if ((Val /= radix) == static_cast<val>(0)) {
					break;
				}
			}
			assert(Val == static_cast<val>(0));
			return;
		}


		//重载
		INLINED void MY_LIBRARY operator*=(Data times) noexcept {
			if (times == Data(0))
			{
				this->destruct();
				return;
			}
			else if (times == Data(1))
			{
				return;
			}
			LargeInteger::LongCmpt<LLComputeTraits<OLL, _Data, Radix>>::MultiplyTo<OLL*, Data>(times, this->next);
		}
		//重载
		/*INLINED*/OLL MY_LIBRARY operator*(Data times)const noexcept {
			OLL Res(*this, true);
			Res *= times;
			return Res;
		}
		//重载
		/*INLINED*/void MY_LIBRARY operator*=(const OLL& b) noexcept {
			if (b.next == nullptr) { return; }
			this->data = ((b.data > 0) ? (this->data) : (!this->data));
			OLL This(*this, true);
			this->destruct();
			this->insert();
			for (const OLL* OprtPtr = b.next; OprtPtr != nullptr; OprtPtr = OprtPtr->next) {
				OLL temp(This * OprtPtr->data, false);
				LargeInteger::LongCmpt<typename LL::LLComputeTraits<OLL, _Data, Radix>>::AppositionComputeTo<typename LargeInteger::StdCmptTraits<Data>::Add, OLL*, Data>(temp.next, this->next);
				temp.destruct();
				This <<= 1;
			}
			This.destruct();
		}
		//重载
		/*INLINED*/OLL MY_LIBRARY operator*(const OLL& b)const noexcept {
			OLL Res(*this, true);
			Res *= b;
			return Res;
		}
		//重载
		INLINED void MY_LIBRARY operator-=(const OLL& b) noexcept {
			*this += (-b);
		}
		//重载OLL链表负号
		INLINED OLL MY_LIBRARY operator-(
			)const noexcept {
			OLL res(*this);
			res.data = !res.data;
			return res;
		}
		//重载OLL链表+=
		INLINED void MY_LIBRARY operator+=(const OLL& that) noexcept {
			if (that.next == nullptr)
			{
				return;
			}
			if (this->next == nullptr)
			{
				*this = OLL(that, true);
				return;
			}
			if ((this->data > 0 && that.data > 0) || (this->data == 0 && that.data == 0))
			{
				LargeInteger::LongCmpt::AppositionComputeTo<typename LargeInteger::LongCmpt::StdCmptTraits<Data>::Add, OLL*, Data, LLComputeTraits<OLL, _Data, Radix>>(that.next, this->next);
			}
			else {
				LargeInteger::Compare Cmpr = LargeInteger::LongCmpt::CompareTo<OLL*, Data, LLComputeTraits<OLL, _Data, Radix>>(this->next, that.next);
				if (Cmpr == LargeInteger::Compare::Equal)
				{
					this->destruct();
				}
				if (Cmpr == LargeInteger::Compare::Larger)
				{
					LargeInteger::LongCmpt::AppositionComputeTo<typename LargeInteger::StdCmptTraits<Data>::SubtractFrom, OLL*, Data, LLComputeTraits<OLL, _Data, Radix>>(that.next, this->next);
				}
				else
				{
					OLL temp(that, true);
					LargeInteger::LongCmpt::AppositionComputeTo<typename LargeInteger::StdCmptTraits<Data>::SubtractFrom, OLL*, Data, LLComputeTraits<OLL, _Data, Radix>>(this->next, temp.next);
					*this = temp;
				}
			}
			this->Simplify();
		}
		//重载OLL链表加号
		INLINED OLL MY_LIBRARY operator+(
			const OLL& b//操作数
			)  const noexcept {
			OLL Result(*this, true);//存储结果
			Result += b;
			return Result;
		}
		//重载OLL链表减号
		/*INLINED*/OLL MY_LIBRARY operator-(
			const OLL& b//操作数
			)const noexcept {
			return (*this + (-b));
		}
		void MY_LIBRARY operator++() {
			OLL temp(true, 1);
			*this += temp;
			temp.destruct();
		}


		INLINED void MY_LIBRARY operator+=(const Data& that)noexcept {
			OLL temp(that, nullptr);
			if (that != Data(0) && this->next == nullptr)
			{
				this->data = Data(1);
				this->insert(that);
				return;
			}
			if (this->data == 0)
			{
				LargeInteger::LongCmpt::AppositionComputeTo<typename StdCmptTraits<Data>::SubtractFrom, OLL*, Data, LLComputeTraits<OLL, _Data, Radix>>(&temp, this->next);
			}
			else
			{

				LargeInteger::LongCmpt::AppositionComputeTo<typename StdCmptTraits<Data>::Add, OLL*, Data, LLComputeTraits<OLL, _Data, Radix>>(&temp, this->next);
			}
		}
		INLINED void MY_LIBRARY operator-=(const Data& that)noexcept {
			OLL temp(that, nullptr);
			if (that != Data(0) && this->next == nullptr)
			{
				this->data = Data(0);
				this->insert(that);
				return;
			}
			if (this->data > 0)
			{
				LargeInteger::LongCmpt::AppositionComputeTo<typename LargeInteger::StdCmptTraits<Data>::SubtractFrom, OLL*, Data, LLComputeTraits<OLL, _Data, Radix>>(&temp, this->next);
			}
			else
			{

				LargeInteger::LongCmpt::AppositionComputeTo<typename LargeInteger::StdCmptTraits<Data>::Add, OLL*, Data, LLComputeTraits<OLL, _Data, Radix>>(&temp, this->next);
			}
		}
		INLINED void MY_LIBRARY operator+(const Data& that)const noexcept {
			OLL res(*this, true);
			return (res += that);
		}
		INLINED void MY_LIBRARY operator-(const Data& that)const noexcept {
			OLL res(*this, true);
			return (res -= that);
		}



		//覆盖赋值
		/*INLINED*/void MY_LIBRARY SetValue(
			long num, const Data* data
		) noexcept {
			OLL* OprtPtr = this;//操作当前对象
			OprtPtr->data = data;
			long count = 1;
			while (true)
			{
				if (count >= num)
				{
					break;
				}
				if (OprtPtr->next == nullptr)
				{
					OprtPtr->insert();
				}
				else
				{
					break;
				}
				OprtPtr = OprtPtr->next;
				OprtPtr->data = &data[count - 1];
				count++;
			}
			return;
		}
		//覆盖赋值
		//因为不知名的原因，对SBOLL禁用
		/*INLINED*/void __cdecl SetValue(
			long num, Data data, ...
		) noexcept {
			OLL* OprtPtr = this;//操作当前对象
			OprtPtr->data = data;
			long count = 1;
			while (true)
			{
				if (count >= num)
				{
					break;
				}
				if (OprtPtr->next == nullptr)
				{
					OprtPtr->insert();
				}
				else
				{
					break;
				}
				OprtPtr = OprtPtr->next;
				OprtPtr->data = ((&data)[count - 1]);
				count++;
			}
			return;
		}

		//位移运算
		//按独立进制而非二进制
		//左移时用默认值补齐
		/*INLINED*/OLL& operator<<=(
			unsigned int bits) noexcept {
			for (unsigned int i = 0; i < bits; i++)
			{
				this->insert();
			}
			return *this;
		}

		bool MY_LIBRARY operator<(const OLL& that)const noexcept {
			if (this->data == 0 && that.data > 0)
			{
				return true;
			}
			if (this->data > 0 && that.data == 0)
			{
				return false;
			}
			if (this->data > 0 && that.data > 0)
			{
				if (LargeInteger::LongCmpt::CompareTo<OLL*, Data, LLComputeTraits<OLL, _Data, Radix>>(this->next, that.next) == LongCmpt::Compare::Smaller)
					return true;
				else return false;
			}
			else
			{
				if (LargeInteger::LongCmpt::CompareTo<OLL*, Data, LLComputeTraits<OLL, _Data, Radix>>(that.next, this->next) == LongCmpt::Compare::Smaller)
					return true;
				else return false;
			}
		}
		bool MY_LIBRARY operator>(const OLL& that)const noexcept {
			if (this->data == 0 && that.data > 0)
			{
				return false;
			}
			if (this->data > 0 && that.data == 0)
			{
				return true;
			}
			if (this->data > 0 && that.data > 0)
			{
				if (LargeInteger::LongCmpt::CompareTo<OLL*, Data, LLComputeTraits<OLL, _Data, Radix>>(this->next, that.next) == LongCmpt::Larger)
					return true;
				else return false;
			}
			else
			{
				if (LargeInteger::LongCmpt::CompareTo<OLL*, Data, LLComputeTraits<OLL, _Data, Radix>>(that.next, this->next) == LongCmpt::Larger)
					return true;
				else return false;
			}
		}
		bool MY_LIBRARY operator<=(const OLL& that)const noexcept {
			return !(*this > that);
		}
		bool MY_LIBRARY operator>=(const OLL& that)const noexcept {
			return !(*this < that);
		}
		void MY_LIBRARY operator%=(const OLL& that)noexcept {
			if (this->next != nullptr && that.next != nullptr)
			{
				LargeInteger::LongCmpt::DivideInto<OLL*, Data, LLComputeTraits<OLL, _Data, Radix>>(that.next, this->next);
			}
			else return;
			this->Simplify();
		}
		void MY_LIBRARY operator/=(const OLL& that)noexcept {
			if (this->next != nullptr && that.next != nullptr)
			{
				OLL Res(true);
				LargeInteger::LongCmpt::DivideInto<OLL, OLL*, Data, LLComputeTraits<OLL, _Data, Radix>>(Res, that.next, this->next);
				this->destruct();
				this->next = Res.next;
			}
			else return;
			this->Simplify();
		}

		//位移运算
		//按独立进制而非二进制
		//右移时第一位销毁
		/*INLINED*/OLL& operator>>=(unsigned int bits) noexcept {
			for (unsigned int i = 0; i < bits; i++)
			{
				this->cut();
			}
			return *this;
		}


		//覆盖赋值
		/*INLINED*/OLL& MY_LIBRARY operator=(
			long value
			) noexcept {
			if constexpr (Radix == _Data(1))
			{
				this->data = (Data)value;
			}
			else
			{
				OLL* OprtPtr = this;//操作当前对象
				if (value >= 0)
				{
					OprtPtr->data = Data(true);
				}
				else
				{
					OprtPtr->data = Data(false);
					value = -value;
				}
				while (true)
				{
					if (value == 0)
					{
						return *this;
					}
					if constexpr (Radix == 0) {
						OprtPtr->insert(static_cast<Data>(value));
						if (sizeof(Data) >= sizeof(value))
						{
							return *this;
						}
						value >>= (LargeInteger::BitsPerByte * sizeof(Data));
					}
					else {
						OprtPtr->insert((Data)(value % Radix));
						value = value / Radix;
					}
					OprtPtr = OprtPtr->next;
				}
			}
			return *this;
		}


		//获取存储的值
		//可能溢出
		/*INLINED*/long long MY_LIBRARY GetValue()const noexcept {
			bool sign = (this->data > 0) ? true : false;
			long long value = 0;
			long n = 0;
			const OLL* OprtPtr = this;
			if (OprtPtr == nullptr)
			{
				return 0;
			}
			while (true)
			{
				if (OprtPtr->next != nullptr)
				{
					OprtPtr = OprtPtr->next;
					value += ((unsigned long long)(OprtPtr->data())) * Power((unsigned long long)Radix, n);
					n++;
				}
				else
				{
					break;
				}
			}
			return (sign) ? value : (-value);
		}

		~LargeUnsigned(){
		}

	private:
		LL LinkedList;
	};
	template<typename LL, auto radix>
	class LargeSigned:private LargeUnsigned<LL, radix>
	{
	public:
		template<typename val>
		LargeSigned(val Val)noexcept
			:PosSign(Val > 0), LargeUnsigned<LL, radix>(ABS(val)) {}

		~LargeSigned()
		{
		}

	private:
		bool PosSign;
	};

}