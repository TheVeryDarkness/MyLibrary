#pragma once

#ifdef _DEBUG
#include "VisualStudioDebug.h"
#endif // _DEBUG
#include <iterator>
#include <iostream>
#include <iomanip>
#include <string>
#include <charconv>
#include "Exception.h"
#include "Statistics.h"
#include "Shared.h"
#ifdef max
#undef max
#endif // max
#ifdef min
#undef min
#endif // min

//���ݹ淶����Ȼ�����Լ����ߵĹ淶����
//Simplify()��Fresh()����Ӧ�ڼӼ��˳�������ɺ󷵻�ʱ��������
//�Ա�������ʹ�λ
#pragma pack(push)
#pragma pack(2)
#define LL_LENGTH(type) const type* OprtPtr=this;size_t s=0;while(OprtPtr!=nullptr){OprtPtr=OprtPtr->next;s++;}return s;
#define LL_SIMPLIFY(type) {type* Flag = this;type* OprtPtr = this;while (true){if (OprtPtr->data!=Data(0U)){Flag = OprtPtr;}if (OprtPtr->next == nullptr){break;}OprtPtr = OprtPtr->next;}while (Flag->next != nullptr){Flag->cut();}return Flag;}
namespace LL {
	//ǰ������
	template <typename Data, unsigned long Radix>
	class OLL;
	template<typename Data, unsigned long Radix>
	class DLL;
	template<
		typename Data,
		typename SubData,
		typename Type,
		typename SubType>
		SubType GetSubList(
			const SubData& (__stdcall* GetFunction)(const Data&),
			const Type& that
		)noexcept;


	//����


	//DataΪ�������ͣ�������Ϊָ��
	template <
		typename Data, unsigned long Radix = 0
	>
		//����oneway������linked list�������ࣩ
		//Notice:
		//The head of the linked list should be in the stack or heap,
		//however, the other sections should be in the heap.
		class OLL
	{
		template<class node, typename Data, Data _Max>
		friend class LinkedListComputeTraits;
		template<class Class, typename Data, unsigned long Radix>
		//����
		inline friend void __stdcall add(
			Class& a,
			const Class& b
		)noexcept;
		template<class Class, typename Data, unsigned long Radix>
		//����
		inline friend void __stdcall multiply(
			Class& a,
			const Class& b
		)noexcept;
		template<class Class, typename Data, unsigned long Radix>
		inline friend void __stdcall multiply(
			Class& a,
			int times)noexcept;
		template<typename Type, unsigned long Radix>
		/*inline*/friend std::ostream& __stdcall out(
			std::ostream& out, const Type& b
		)noexcept;
		template<typename Type>
		inline friend void __stdcall SinglePrint(
			const Type& that,
			std::ostream& out,
			bool ShowComma,
			unsigned MinLength,
			unsigned base
		)noexcept;
		template<typename Type, unsigned long Radix>
		/*inline*/friend std::ostream& __stdcall Print(
			const Type& that,
			std::ostream& out
		)noexcept;
		template<
			typename Data,
			typename SubData,
			typename Type,
			typename SubType>
			friend SubType GetSubList(
				const SubData& (__stdcall* GetFunction)(const Data&),
				const Type& that
			)noexcept;
	public:
		//����
		/*inline*/void __stdcall operator*=(int times) noexcept {
			LL::multiply<OLL, Data, Radix>(*this, times);
		}
		//����
		/*inline*/OLL __stdcall operator*(int times)const noexcept {
			OLL Res(*this, true);
			Res *= times;
			return Res;
		}
		//����
		/*inline*/void __stdcall operator*=(const OLL& b) noexcept {
			LL::multiply<OLL, Data, Radix>(*this, b);
		}
		//����
		/*inline*/OLL __stdcall operator*(const OLL& b)const noexcept {
			OLL Res(*this, true);
			Res *= b;
			return Res;
		}
		//����
		inline void __stdcall operator-=(const OLL& b) noexcept {
			*this += (-b);
		}
		//����OLL������
		inline OLL __stdcall operator-(
			)const noexcept {
			OLL res(*this);
			res.data = !res.data;
			return res;
		}
		//����OLL����Ӻ�
		inline OLL __stdcall operator+(
			const OLL& b//������
			)  const noexcept {
			OLL Result(*this, true);//�洢���
			Result += b;
			return Result;
		}
		//����OLL����+=
		inline void __stdcall operator+=(const OLL& that) noexcept {
			LL::add<OLL, Data, Radix>(*this, that);
			this->Simplify();
		}
		//����OLL�������
		/*inline*/OLL __stdcall operator-(
			const OLL& b//������
			)const noexcept {
			return (*this + (-b));
		}
		inline __stdcall ~OLL() {
			this->next = nullptr;
		}
	protected:
		//ָ����һ�ڵ�ָ��
		OLL* next = nullptr;
		//����
		Data data;
		//���ƹ��캯��
		//Ĭ��Ϊ���
		inline __stdcall OLL(
			const OLL& that,
			bool DeepCopy
		) noexcept {
			this->data = that.data;
			this->destruct();
			if (DeepCopy)
			{
				OLL* OprtPtr = this;
				const OLL* PreOprtPtr = &that;
				while (true)
				{
					if (PreOprtPtr->next != nullptr)
					{
						OprtPtr->insert(PreOprtPtr->next->data);
						OprtPtr = OprtPtr->next;
						PreOprtPtr = PreOprtPtr->next;
					}
					else
					{
						OprtPtr->next = nullptr;
						break;
					}
				}
			}
			else
			{
				this->next = that.next;
			}
			return;
		}
	public:
		inline unsigned long __stdcall GetRadix()const noexcept {
			return Radix;
		}
		inline size_t __stdcall RawLength()const noexcept {
			LL_LENGTH(OLL);
		}
		//���ƹ��캯��
		//Ĭ��Ϊǳ����
		inline __stdcall OLL(
			const OLL& that
		) noexcept {
			this->data = that.data;
			this->destruct();
			this->next = that.next;
			return;
		}
		//����ʼ������ͷ�Ĺ��캯��
		explicit inline __stdcall OLL(
			Data HeadData,
			OLL* NextPtr = nullptr
		) noexcept {
			if constexpr (Radix == 0)
			{
				this->data = HeadData;
				this->next = NextPtr;
			}
			else if constexpr (Radix == 1)
			{
				throw RadixError("Radix can't be 1.");
			}
			else if constexpr (
				(Radix - 1)
				!=
				(unsigned long)((Data)(Radix - 1))
				)
				throw RadixError("Radix overflow.");
			this->data = Data(HeadData);
			this->next = NextPtr;
			return;
		}
		//�ͷ�����ͷ���Ӧ���ڵ�ָ��
		inline void __stdcall destruct() noexcept {
			OLL* OprtPtr = this->next;
			this->next = nullptr;
			while (OprtPtr != nullptr)
			{
				OLL* temp = OprtPtr->next;
				delete OprtPtr;
				OprtPtr = temp;
			}
			return;
		}
		//�ͷ�����ͷ���Ӧ���ڵ�ָ��
		inline void __stdcall operator~() noexcept {
			this->destruct();
			return;
		}
		//����������ֵ
		/*inline*/Data __stdcall Max() const noexcept {
			const OLL* OprtPtr = this;
			Data MaxNumber = OprtPtr->data;
			do
			{
				MaxNumber =
					(MaxNumber > OprtPtr->data) ?
					(MaxNumber) : (OprtPtr->data);
				if (OprtPtr->next != nullptr)
				{
					OprtPtr = OprtPtr->next;
				}
				else
				{
					break;
				}
			} while (true);
			OprtPtr = nullptr;
			return MaxNumber;
		}
		//���������Сֵ
		/*inline*/Data __stdcall Min()const noexcept {
			const OLL* OprtPtr = this;
			Data MinNumber = OprtPtr->data;
			do
			{
				MinNumber =
					(MinNumber < OprtPtr->data) ?
					(MinNumber) : (OprtPtr->data);
				if (OprtPtr->next != nullptr)
				{
					OprtPtr = OprtPtr->next;
				}
				else
				{
					break;
				}
			} while (true);
			OprtPtr = nullptr;
			return MinNumber;
		}
		//�������ƽ��ֵ
		//�����Ȳ��ó���LONG_MAX
		/*inline*/Data __stdcall Average()const noexcept
		{
			const OLL* OprtPtr = this;
			double average = 0;
			unsigned long Account = 1;
			while (OprtPtr->next != nullptr)
			{
				Account++;
				OprtPtr = OprtPtr->next;
			}
			OprtPtr = this;
			do {
				average += (OprtPtr->data / Account);
				if (OprtPtr->next != nullptr)
				{
					OprtPtr = OprtPtr->next;
				}
				else
				{
					break;
				}
			} while (true);
			OprtPtr = nullptr;
			return (long)average;
		}
		//����
		inline void __stdcall swap(
			OLL& that
		) noexcept {
			Data TempData = that.data;
			that.data = this->data;
			this->data = TempData;
			OLL* TempPtr = that.next;
			that.next = this->next;
			this->next = TempPtr;
			return;
		}
		//���Ǹ�ֵ
		/*inline*/void __stdcall SetValue(
			long num, const Data* data
		) noexcept {
			OLL* OprtPtr = this;//������ǰ����
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
		//���Ǹ�ֵ
		//��Ϊ��֪����ԭ�򣬶�SBOLL����
		/*inline*/void __cdecl SetValue(
			long num, Data data, ...
		) noexcept {
			OLL* OprtPtr = this;//������ǰ����
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
		//���Ǹ�ֵ
		//ǳ����
		//���������ֵ����ԭ������
		inline OLL& __stdcall operator=(
			const OLL& that
			) noexcept {
			if (&that == this)
			{
				return *this;
			}
			this->destruct();
			this->data = that.data;
			this->next = that.next;
			return *this;
		}
		//λ������
		//���������ƶ��Ƕ�����
		//����ʱ��Ĭ��ֵ����
		/*inline*/OLL& operator<<=(
			unsigned int bits) noexcept {
			for (unsigned int i = 0; i < bits; i++)
			{
				this->insert();
			}
			return *this;
		}
		/*inline*/bool __stdcall operator==(
			const OLL& that
			)const noexcept {
			OLL* OprtPtr = this, * PreOprtPtr = &that;
			while (true)
			{
				if (OprtPtr == nullptr
					&&
					PreOprtPtr == nullptr)
				{
					return true;
				}
				else if (OprtPtr == nullptr
					||
					PreOprtPtr == nullptr)
				{
					return false;
				}
				if (OprtPtr.data != PreOprtPtr.data)
				{
					return false;
				}
				OprtPtr = OprtPtr->next;
				PreOprtPtr = PreOprtPtr->next;
			}
		}
		//λ������
		//���������ƶ��Ƕ�����
		//����ʱ��һλ����
		/*inline*/OLL& operator>>=(unsigned int bits) noexcept {
			for (unsigned int i = 0; i < bits; i++)
			{
				this->cut();
			}
			return *this;
		}
		//������ͷ������������ͷ����ʼ������֮�������
		//ʹ��new����������
		/*inline*/OLL __stdcall invert(const OLL& b) const noexcept {
			OLL Result(b.data);
			const OLL* OprtPtr = &b;
			while (OprtPtr->next != nullptr)
			{
				Result.insert(OprtPtr->next->data);
				OprtPtr = (OprtPtr->next);
			}
			return Result;
		}
		//�ڵ�ǰλ�ú�����µ�һ��
		inline void insert(Data New = Data(false)) noexcept {
			OLL* temp = this->next;
			this->next = new OLL(New, temp);
			return;
		}
		//ɾ����ǰλ�ú��һλ
		inline void __stdcall cut() noexcept {
			if (this->next == nullptr)
			{
#ifdef _DEBUG
				DEBUG_OUT;
				throw std::domain_error("No next node");
#else
				return;
#endif // _DEBUG
			}
			else
			{
				OLL* temp = this->next->next;
				delete this->next;
				this->next = temp;
			}
			return;
		}
		inline OLL* __stdcall Simplify() noexcept {
			LL_SIMPLIFY(OLL);
		}
		//���Ǹ�ֵ
		/*inline*/OLL& __stdcall operator=(
			long value
			) noexcept {
			if constexpr (Radix == 0)
			{
				this->data = (Data)value;
			}
			else
			{
				OLL* OprtPtr = this;//������ǰ����
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
						break;
					}
					else if (OprtPtr->next == nullptr)
					{
						OprtPtr->insert();
					}
					OprtPtr = OprtPtr->next;
					OprtPtr->data = (Data)(value % Radix);
					value = value / Radix;
				}
			}
			return *this;
		}
		//�ж�that�����Ƿ�Ϊ�����ĩβ
		//��ʹ��ȱʡ��������ʾ��ȡ��ǰ�����Ƿ�Ϊ�����ĩβ
		inline bool __stdcall IsEnding(
			const OLL* that = nullptr
		)const noexcept {
			return ((that == nullptr)
				?
				(this->next == nullptr)
				:
				(that->next == nullptr));
		}
		//��ȡ�洢��ֵ
		//�������
		/*inline*/long  __stdcall GetValue()const noexcept {
			bool sign = (this->data > 0) ? true : false;
			long value = 0;
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
					value += ((unsigned long long)(OprtPtr->data)) * (unsigned long long)pow(Radix, n);
					n++;
				}
				else
				{
					break;
				}
			}
			return (sign) ? value : (-value);
		}
		inline friend std::ostream& __stdcall operator<<(
			std::ostream& out, const OLL& that) noexcept {
			return LL::out<OLL, Radix>(out, that);
		}
		void __stdcall SinglePrint(
			std::ostream& out = std::cout,
			bool Comma = true
		)const noexcept {
			return LL::SinglePrint<OLL>(*this, out);
		}
		//���������������̨����
		/*inline*/std::ostream& __stdcall Print(
			std::ostream& out = std::cout
		) const noexcept {
			return LL::Print<OLL, Radix>(*this, out);
		}
	protected:
		//��ȡthat���ڴ洢������
		//��ʹ��ȱʡ��������ʾ��ȡ��ǰ���ڴ洢������
		inline const Data& __stdcall GetThisData(
			const OLL* that = nullptr
		)const noexcept {
			return ((that == nullptr) ?
				this->data
				:
				that->data);
		}
		//��ȡthat���ڵ���һ����
		//��ʹ��ȱʡ��������ʾ��ȡ��ǰ���ڵ���һ����
		inline const OLL* __stdcall GetNext(
			const OLL* that = nullptr
		)const noexcept {
			return ((that == nullptr) ?
				this->next
				:
				that->next);
		}
	};
	template<typename Data, unsigned long Radix>
	class DLL
	{
		static const inline std::forward_list<Data> Factor = PrimeFactorList(static_cast<Data>(Radix));
		static const inline Data MaxFactor = MinConti(Factor);

		template<class node, typename Data, Data _Max>
		friend class LinkedListComputeTraits;
		//��Ԫ��������

		template<class Class, typename Data, unsigned long Radix>
		//����
		inline friend void __stdcall add(
			Class& a,
			const Class& b
		)noexcept;
		template<class Class, typename Data, unsigned long Radix>
		//����
		inline friend void __stdcall multiply(
			Class& a,
			const Class& b
		)noexcept;
		template<class Class, typename Data, unsigned long Radix>
		inline friend void __stdcall multiply(
			Class& a,
			int times)noexcept;
		template<typename Type, unsigned long Radix>
		/*inline*/friend std::ostream& __stdcall out(
			std::ostream& out, const Type& b
		)noexcept;
		template<typename Type>
		inline friend void __stdcall SinglePrint(
			const Type& that,
			std::ostream& out,
			bool ShowComma,
			unsigned MinLength,
			unsigned base
		)noexcept;
		template<typename Type, unsigned long Radix>
		/*inline*/friend std::ostream& __stdcall Print(
			const Type& that,
			std::ostream& out
		)noexcept;
		template<
			typename Data,
			typename SubData,
			typename Type,
			typename SubType>
			friend SubType GetSubList(
				const SubData& (__stdcall* GetFunction)(const Data&),
				const Type& that
			)noexcept;
		friend class Q;
	public:
		//����
		inline void __stdcall operator*=(int times) {
			LL::multiply<DLL, Data, Radix>(*this, times);
		}
		//����
		inline DLL __stdcall operator*(int times)const {
			DLL Res(*this, true);
			Res *= times;
			return Res;
		}
		//����
		/*inline*/void __stdcall operator*=(const DLL& b) {
			LL::multiply<DLL, Data, Radix>(*this, b);
		}
		//����
		inline DLL __stdcall operator*(const DLL& b)const {
			DLL Res(*this, true);
			Res *= b;
			return Res;
		}
		//���ؾ���ֵ
		inline const DLL __stdcall abs()const noexcept {
			return ((this->data > 0) ? *this : (-*this));
		}
		//������
		inline bool __stdcall IsPositive()const noexcept {
			return (this->data > 0);
		}
		//���ظ�
		inline void __stdcall SetToContradict()noexcept {
			this->data = ((this->data == 0) ? 1 : 0);
		}
		//����DLL����Ӻ�
		inline DLL __stdcall operator+(
			const DLL& b//������
			)  const noexcept {
			DLL Result(*this, true);//�洢���
			Result += b;
			return Result;
		}
		//����DLL����+=
		inline void __stdcall operator+=(const DLL& that)noexcept {
			LL::add<DLL, Data, Radix>(*this, that);
#ifdef _DEBUG
			this->Fresh();
#endif // _DEBUG
			this->Simplify();
		}
		//����DLL�������
		inline DLL __stdcall operator-(
			const DLL& b//������
			)const noexcept {
			return (*this + (-b));
		}
		//����
		inline void __stdcall operator-=(const DLL& b)noexcept {
			DLL _b = -b;
			*this += _b;
			_b.destruct();
		}
		//����
		inline void __stdcall operator-=(DLL& b)noexcept {
			Data Orig = b.data;
			b.data = !b.data;
			*this += b;
			b.data = Orig;
		}
		//����DLL������
		inline DLL __stdcall operator-(
			)const noexcept {
			DLL res(*this, true);
			res.data = !res.data;
			return res;
		}
		inline _stdcall ~DLL() {
			this->next = nullptr;
			this->last = nullptr;
		}
	protected:
		Data data;
		DLL* next = nullptr;
		DLL* last = nullptr;
		//�����ǳ�����ɲ���DeepCopyָ��
		explicit inline __stdcall DLL(
			const DLL& that,
			bool DeepCopy
		) noexcept :data(that.data) {
			this->destruct();
			if (DeepCopy)
			{
				this->last = nullptr;
				DLL* OprtPtr = this;
				const DLL* PreOprtPtr = that.next;
				while (PreOprtPtr != nullptr)
				{
					OprtPtr->insert(PreOprtPtr->data);
					OprtPtr = OprtPtr->next;
					PreOprtPtr = PreOprtPtr->next;
				}
			}
			else
			{
				this->next = that.next;
				this->last = that.last;
				if (this->next != nullptr)
				{
					this->next->last = this;
				}
			}
			return;
		}
	public:
		//��data��next��last����
		explicit inline __stdcall DLL(
			Data HeadData = Data(false),
			DLL* NextPtr = nullptr,
			DLL* LastPtr = nullptr
		)noexcept :
			data(HeadData),
			next(NextPtr),
			last(LastPtr) {}
		//ǳ����
		inline __stdcall DLL(
			const DLL& that
		) noexcept :data(that.data) {
			this->destruct();
			this->next = that.next;
			this->last = that.last;
			if (this->next != nullptr)
			{
				//�˲���ǳ���Ҫ
				this->next->last = this;
			}
		}
		//���Ǹ�ֵ
		//ǳ����
		//���������ֵ����ԭ������
		inline void __stdcall operator=(
			const DLL& that
			) noexcept {
			if (&that == this)
			{
				return;
			}
			this->destruct();
			this->data = that.data;
			this->next = that.next;
			this->last = that.last;
			if (this->next != nullptr)
			{
				this->next->last = this;
			}
			return;
		}
		inline bool __stdcall operator==(
			int that
			)const noexcept {
			if (this->next == nullptr && that == 0)
			{
				return true;
			}
			else if (that == 0)
			{
				return false;
			}
			if (
				(this->data > 0 && that >= 0)
				||
				(this->data == 0 && that <= 0)
				) {
				that = std::abs(that);
			}
			else return false;
			DLL* OprtPtr = this->next;
			while (OprtPtr != nullptr)
			{
				if (OprtPtr->data != (that % Radix))
					return false;
				else
				{
					that /= Radix;
					if (that == 0)
					{
						if (OprtPtr->next == nullptr || OprtPtr->next->data == 0)
						{
							return true;
						}
						else return false;
					}
					OprtPtr = OprtPtr->next;
				}
			}
			return false;
		}
		inline bool __stdcall operator==(
			const DLL& that
			)const noexcept {
			const DLL* OprtPtr = this, * PreOprtPtr = &that;
			while (true)
			{
				if (OprtPtr == nullptr && PreOprtPtr == nullptr)
				{
					return true;
				}
				else if (OprtPtr == nullptr || PreOprtPtr == nullptr)
				{
					return false;
				}
				if (OprtPtr->data == PreOprtPtr->data)
				{
					OprtPtr = OprtPtr->next;
					PreOprtPtr = PreOprtPtr->next;
					continue;
				}
				return false;
			}
		}
		inline bool __stdcall operator<(const DLL& that)const noexcept {
			if (this->data > 0 && that.data == 0)
				return false;
			if (this->data == 0 && that.data > 0)
				return true;
			DLL* OprtPtr = this->GetEnd(), * PreOprtPtr = that.GetEnd();
			if (this->data == 0 && that.data == 0)
			{
				if (this->RawLength() < that.RawLength())
				{
					return false;
				}
				if (this->RawLength() > that.RawLength())
				{
					return true;
				}
				DLL* temp = OprtPtr;
				OprtPtr = PreOprtPtr;
				PreOprtPtr = temp;
			}
			else
			{
				if (this->RawLength() < that.RawLength())
				{
					return true;
				}
				if (this->RawLength() > that.RawLength())
				{
					return false;
				}
				//*this �� that ��Ϊ��
			}
			//��OprtPtr��Ӧ����С��PreOprtPtr��Ӧ����
			//	������
			while (true)
			{
				if (OprtPtr->data == PreOprtPtr->data)
				{
					OprtPtr = OprtPtr->last;
					PreOprtPtr = PreOprtPtr->last;
				}
				else return (OprtPtr->data < PreOprtPtr->data);
				//���
				if (OprtPtr->last == nullptr && PreOprtPtr->last == nullptr)
				{
					return false;
				}
			}
		}
		inline bool __stdcall operator<=(const DLL& that)const noexcept {
			if (this->data > 0 && that.data == 0)
				return false;
			if (this->data == 0 && that.data > 0)
				return true;
			DLL* OprtPtr = this->GetEnd(), * PreOprtPtr = that.GetEnd();
			if (this->data == 0 && that.data == 0)
			{
				if (this->RawLength() < that.RawLength())
				{
					return false;
				}
				if (this->RawLength() > that.RawLength())
				{
					return true;
				}
				DLL* temp = OprtPtr;
				OprtPtr = PreOprtPtr;
				PreOprtPtr = temp;
			}
			else
			{
				if (this->RawLength() < that.RawLength())
				{
					return true;
				}
				if (this->RawLength() > that.RawLength())
				{
					return false;
				}
				//*this �� that ��Ϊ��
			}
			//��OprtPtr��Ӧ����С��PreOprtPtr��Ӧ����
			//	������
			while (true)
			{
				if (OprtPtr->data == PreOprtPtr->data)
				{
					OprtPtr = OprtPtr->last;
					PreOprtPtr = PreOprtPtr->last;
				}
				else return (OprtPtr->data < PreOprtPtr->data);
				//���
				if (OprtPtr->last == nullptr && PreOprtPtr->last == nullptr)
				{
					return true;
				}
			}
		}
		inline bool __stdcall operator>(const DLL& that)const noexcept {
			return (that < *this);
		}
		inline bool __stdcall operator>=(const DLL& that)const noexcept {
			return (that <= *this);
		}
		inline void __stdcall destruct() noexcept {
			while (this->next != nullptr)
			{
				this->cut();
			}
			return;
		}
		inline void __stdcall operator~() noexcept {
			this->destruct();
			return;
		}
		//�ڵ�ǰλ�ú�����µ�һ��
		inline void __stdcall insert(
			Data New = Data(false)
		) noexcept {
			DLL* temp = this->next;
			this->next = new DLL(New, temp, this);
			if (temp != nullptr)
			{
				temp->last = this->next;
			}
			return;
		}
		//ɾ����ǰλ�ú��һλ
		inline void __stdcall cut() noexcept {
			if (this->next == nullptr)
			{
#ifdef _DEBUG
				DEBUG_OUT;
				throw std::domain_error("No next node.");
#else
				return;
#endif // _DEBUG
			}
			else
			{
				DLL* temp = this->next->next;
				delete this->next;
				this->next = temp;
				if (temp != nullptr)
				{
					temp->last = this;
				}
			}
			return;
		}
		//��ȥ�հ׸�λ
		//����ָ��ǿ����λ��ָ��
		inline DLL* __stdcall Simplify() noexcept {
			LL_SIMPLIFY(DLL);
		}
	protected:
		//ˢ��lastָ��
		//��ָ֤����ȷ
		//����ĩβָ��
		inline DLL* __stdcall Fresh() noexcept {
#ifdef _DEBUG
			DLL* OprtPtr = this;
			while (OprtPtr->next != nullptr) {
				if (OprtPtr->next->last != OprtPtr)
				{
					DEBUG_OUT;
					throw std::out_of_range("Unexpected");
				}
				OprtPtr->next->last = OprtPtr;
				OprtPtr = OprtPtr->next;
			}
			this->last = nullptr;
			return OprtPtr;
#else
			return this;
#endif // _DEBUG
		}
		inline DLL* __stdcall GetEnd()const noexcept {
			if (this->next == nullptr)
			{
				return const_cast<DLL*>(this);
			}
			DLL* OprtPtr = this->next;
			while (OprtPtr->next != nullptr) {
				OprtPtr = OprtPtr->next;
			}
			return OprtPtr;
		}
	public:
		//Head included
		inline size_t __stdcall RawLength()const noexcept {
			LL_LENGTH(DLL);
		}
		explicit inline __stdcall DLL(
			bool positive,
			unsigned long value
		) noexcept {
			if constexpr (Radix == 0)
			{
				this->data = static_cast<Data>(value * ((positive) ? 1 : (-1)));
				return;
			}
			{
				this->data = static_cast<Data>(positive);
				this->destruct();
				DLL* OprtPtr = this;//������ǰ����
				while (true)
				{
					if (value == 0)
					{
						break;
					}
					if (OprtPtr->next == nullptr)
					{
						OprtPtr->insert(
							(Data)(value % Radix)
						);
						OprtPtr = OprtPtr->next;
						value /= Radix;
					}
				}
			}
		}
		//ǳ����
		//���Ǹ�ֵ
		inline DLL& __stdcall operator=(
			long value
			) noexcept {
			this->destruct();
			*this = DLL(sign(value), std::abs(value));
			return *this;
		}
		//λ������
		//��������λ
		//���������ƶ��Ƕ�����
		//����ʱ��Ĭ��ֵ����
		inline DLL& operator<<=(
			size_t bits
			) noexcept {
			for (size_t i = 0; i < bits; i++)
			{
				this->insert();
			}
			return *this;
		}
		//λ������
		//��������λ
		//���������ƶ��Ƕ�����
		//����ʱ��һλ����
		inline DLL& operator>>=(
			size_t bits
			) noexcept {
			for (size_t i = 0; i < bits; i++)
			{
				this->cut();
			}
			return *this;
		}
		//����
		inline void __stdcall operator/=(
			unsigned long that
			) noexcept {
			if (that == 0)
			{
#ifdef _DEBUG
				DEBUG_OUT;
				throw std::out_of_range("Denominator can't be 0.(from operator%=(const DLL&))");
#else
				return;
#endif // _DEBUG
			}
			if (this->next == nullptr)
			{
				return *this;
			}
			if constexpr (Radix == 0)
			{
				return *this;
			}
			if constexpr (Radix == 0)throw std::invalid_argument();
			if (that % Radix == 0) {
				return ((*this >>= 1) /= (that / Radix));
			}
			else
			{
				DLL* OprtPtr = this;
				while (true)
				{
					if (OprtPtr->next != nullptr)
					{
						OprtPtr = OprtPtr->next;
					}
					else {
						break;
					}
				}
				Data Upper = 0;
				while (true)
				{
					unsigned long temp = OprtPtr->data + Upper;
					OprtPtr->data += Upper;
					Upper = temp % that;
					OprtPtr->data = temp / that;
					if (OprtPtr != this->next
						&&
						OprtPtr->last != this)
					{
						OprtPtr = OprtPtr->last;
					}
					else break;
				}
				return *this;
			}
		}
		inline void __stdcall operator%=(unsigned long that) noexcept {
			if (that == 0)
			{
#ifdef _DEBUG
				DEBUG_OUT;
				throw std::out_of_range("Denominator can't be 0.(from operator%=(const DLL&))");
#else
				return;
#endif // _DEBUG
			}
			if (this->next == nullptr)
			{
				return *this;
			}
			if constexpr (Radix == 0)
			{
				return *this;
			}
			if (Radix % that == 0) {
				this->next->destruct();
				this->next->data %= that;
				return *this;
			}
			DLL* OprtPtr = this;
			while (true)
			{
				if (OprtPtr->next != nullptr)
				{
					OprtPtr = OprtPtr->next;
				}
				else {
					OprtPtr->data %= that;
					break;
				}
				OprtPtr->data %= that;
			}
			unsigned long Upper = OprtPtr->data;
			while (true)
			{
				if (OprtPtr != this->next && OprtPtr->last != this
					)
				{
					OprtPtr = OprtPtr->last;
				}
				else break;
				OprtPtr->data = (OprtPtr->data + Upper * Radix) % that;
				Upper = OprtPtr->data;
			}
			OprtPtr->destruct();
			return *this;
		}
		inline void __stdcall operator%=(const DLL& that) noexcept {
			if (that == 0)
			{
#ifdef _DEBUG
				DEBUG_OUT;
				throw std::out_of_range("Denominator can't be 0.(from operator%=(const DLL&))");
#else
				return;
#endif // _DEBUG
			}
			bool ThisSign = (this->data);
			this->data = 1;
			size_t ThatLength = that.RawLength();
			long long d = (static_cast<long long>(this->RawLength()) - ThatLength);
			if (d < 0LL)
			{
				return;
			}
			size_t bit = static_cast<size_t>(d);
			DLL That(that, true);
			That.data = 1;
			That <<= bit;
			while (true)
			{
				//size_t ThisLength = this->RawLength();
				if (this->RawLength() < ThatLength)
				{
					That.destruct();
					break;
				}
				DLL* OprtPtr = this->GetEnd(), * PreOprtPtr = That.GetEnd();
				if (*this >= That)
				{
					if (((OprtPtr->data) / (PreOprtPtr->data + 1)) >= 2)
					{
						DLL temp(That, true);
						temp *= ((OprtPtr->data) / (PreOprtPtr->data + 1));
						*this -= temp;
						temp.destruct();
					}
					else
					{
						*this -= That;
					}
				}
				else {
					bit = static_cast<size_t>(static_cast<long long>(this->RawLength()) - that.RawLength());
					if (bit == 0)
					{
						That.destruct();
						this->data = ThisSign;
						return;
					}
					else
					{
						bit--;
						That >>= 1U;
					}
				}
			}
		}
		inline void __stdcall operator/=(const DLL& that) noexcept {
			if (that == 0)
			{
#ifdef _DEBUG
				DEBUG_OUT;
				throw std::out_of_range("Denominator can't be 0.(from operator%=(const DLL&))");
#else
				return;
#endif // _DEBUG
			}
			size_t l1 = this->RawLength(), l2 = that.RawLength();
			if (l1 < l2)
			{
				*this = 0;
				return;
			}
			unsigned int bit = static_cast<unsigned int>(static_cast<long long>(l1) - l2);
			DLL That(that, true);
			That.data = 1;
			That <<= bit;
			Data ThisSign = this->data;
			this->data = 1;
			DLL Res(true);
			while (true)
			{
				if (*this > That)
				{
					DLL* _a = this->GetEnd(), * _b = that.GetEnd();
					if ((_a->data) / (_b->data + 1) >= 2)
					{
						Data Ratio = (_a->data) / (_b->data + 1) - 1;
						DLL temp(That * Ratio, false);
						*this -= temp;
						temp = Ratio;
						Res += temp;
						temp.destruct();
					}
					else
					{
						That.data = 0;
						*this += That;
						That.data = 1;
						DLL one(true, 1);//�˴����Ż�
						Res += one;
						one.destruct();
					}
				}
				else if (*this == That)
				{
					//���δ�������
					//Ϊ���Ч����ȥ
					//*this -= that;

					DLL one(true, 1);//�˴����Ż�
					Res += one;
					one.destruct();
					Res <<= bit;
					*this = Res;
					this->data = ThisSign;
					That.destruct();
					return;
				}
				else
				{
					if (bit > 0)
					{
						Res <<= 1U;
						That >>= 1U;
						bit--;
					}
					else
					{
						*this = Res;
						this->data = ThisSign;
						That.destruct();
						return;
					}
				}
			}
		}
		inline friend std::ostream& __stdcall operator<<=(
			std::ostream& out, const DLL& that
			) noexcept {
			return LL::out<DLL, Radix>(out, that);
		}
		inline void __stdcall SinglePrint(
			std::ostream& out = std::cout,
			bool Comma = true
		)const noexcept {
			return LL::SinglePrint<DLL>(*this, out);
		}
		//���������������̨����
		inline std::ostream& __stdcall Print(
			std::ostream& out = std::cout
		)const noexcept {
			return LL::Print<DLL, Radix>(*this, out);
		}
	};
#define NoNext(a) (a->next==nullptr||a==&NullObject)
	//����
	template<class Class, typename Data, unsigned long Radix>
	inline void __stdcall add(
		Class& a, const Class& b
	) noexcept {
		Class NullObject(0, nullptr);
		NullObject.next = &NullObject;
		Class* OprtPtr_a = &a;//a�Ĳ���ָ��
		const Class* OprtPtr_b = &b;//b�Ĳ���ָ��
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
		//������
		else if constexpr (Radix == 2)
		{
			bool Bit = false;//true��ʾ��λ����λ
			if (a.next == nullptr || b.next == nullptr)
			{
				return;
			}
			Class CopyThis(a, true);//a�Ŀ���
			if (OprtPtr_a->data == OprtPtr_b->data)
			{//�ӷ�ģʽ

				OprtPtr_a = OprtPtr_a->next;
				OprtPtr_b = OprtPtr_b->next;
				//������Ҫ���ݲ���
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
			else//����ģʽ
			{
				if (a.data == false)
				{
					OprtPtr_a = OprtPtr_b = nullptr;
					a = b + (CopyThis);
					~CopyThis;
					return;
				}
				OprtPtr_a = OprtPtr_a->next;
				OprtPtr_b = OprtPtr_b->next;
				do
				{
					//����ǰλ
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
					//������һλ
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
						~CopyThis;
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
			~CopyThis;
			OprtPtr_a = OprtPtr_b = nullptr;
			return;
		}
		//�ձ�����
		else
		{
			bool Bit = false;//true��ʾ��λ����λ
			if (b.next == nullptr)//bΪ0
			{
				return;
			}
			else if (a.next == nullptr)//aΪ0��ֱ�ӽ�b������a
			{
				a = Class(b, true);
				return;
			}
			Class CopyThis(a, true);//a�Ŀ���
			if (OprtPtr_a->data == OprtPtr_b->data)//�ӷ�ģʽ
			{//�������λ
				if (OprtPtr_a->data)//a��b��Ϊ����
				{
					a.data = true;
				}
				else//a��b��Ϊ����
				{
					a.data = false;;
				}
				//����
				OprtPtr_a = OprtPtr_a->next;
				OprtPtr_b = OprtPtr_b->next;
				//������ֵ����
				do
				{//����ָ��ָ��ĵ�ǰλ
					if (
						(static_cast<unsigned long long>(OprtPtr_a->data)
							+ static_cast<unsigned long long>(OprtPtr_b->data)
							)
						>= Radix - (Bit ? 1ULL : 0ULL)
						)//��Ҫ��λ
					{
						OprtPtr_a->data =
							Data(
							(
								(unsigned long long)OprtPtr_a->data
								+ (unsigned long long)OprtPtr_b->data
								+ (Bit ? 1ULL : 0ULL)) - Radix);
						Bit = true;
					}
					else//����Ҫ��λ
					{
						OprtPtr_a->data =
							(OprtPtr_a->data
								+ OprtPtr_b->data
								+ static_cast<Data>(Bit));
						Bit = false;
					}
					//������һλ�Ĵ��������������ѡ����
					if (OprtPtr_a->next == nullptr
						&&
						NoNext(OprtPtr_b)
						&&
						Bit == false)//�����λ��a��b���ѽ���
					{
						break;
					}
					else if (
						OprtPtr_a->next == nullptr
						&&
						NoNext(OprtPtr_b)
						)//a��b�Ѿ��������������λ
					{
						OprtPtr_a->insert(0);
						OprtPtr_b = &NullObject;
					}
					else if (OprtPtr_a->next == nullptr)//a�Ѿ���������bδ����
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
			else//����ģʽ
			{
				if (a.data == false)
				{
					//OprtPtr_a = OprtPtr_b = nullptr;
					a = b + CopyThis;
					~CopyThis;
					return;
				}
				OprtPtr_a = OprtPtr_a->next;
				OprtPtr_b = OprtPtr_b->next;
				do
				{
					//����ǰλ
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
					//������һλ
					if (OprtPtr_a->next == nullptr && NoNext(OprtPtr_b) && !Bit)
					{
						a.data = 1;
						break;
					}
					else if (OprtPtr_a->next == nullptr)
					{
						CopyThis.data = !CopyThis.data;//ȡ��
						Class _b = -b;
						a = CopyThis + _b;
						a.data = !a.data;
						~_b;
						~CopyThis;
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
			~CopyThis;
			OprtPtr_a = nullptr;
			return;
		}
	}
#ifdef NoNext
#undef NoNext
#endif // NoNext

	template<class Class, typename Data, unsigned long Radix>
	void __stdcall multiply(Class& a, const Class& b) noexcept {
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
	void __stdcall multiply(Class& a, int times) noexcept {
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

	template<typename Type, unsigned long Radix = 0>
	//�����
	/*inline*/std::ostream & __stdcall out(
		std::ostream & out, const Type & b
	) noexcept {
		if (b.next != nullptr)
		{
			out << *(b.next) << "," << b.data;
		}
		else
		{
			out << b.data;
		}
		return out;
	}
	template<typename Type>
	//�����������̨����
	//��Ҫ�û�������
	inline void __stdcall SinglePrint(
		const Type& that,
		std::ostream& out = std::cout,
		bool ShowComma = true,
		unsigned MinLength = 0,
		unsigned base = 10
	) noexcept {
		if (that.next != nullptr)
		{
			SinglePrint(*that.next, out, ShowComma, MinLength, base);
			out << ((ShowComma) ? "," : "");
			char* c = new char[MinLength + 1ULL]();
			std::to_chars_result rs = std::to_chars(c, &(c[MinLength]), static_cast<int>(that.data), base);

			std::string str = c;
			delete[] c;
			if (str.length() < MinLength)
			{
				std::string nStr;
				for (size_t i = MinLength - str.length(); i > 0; i--)
				{
					nStr.push_back('0');
				}
				nStr += str;
				out << nStr;
			}
			else out << str;
		}
		else
		{
			out << that.data;
		}
		return;
	}
	template<typename Type, unsigned long Radix = 0>
	//���������������̨����
	//�����Զ�����
	/*inline*/std::ostream & __stdcall Print(
		const Type & that,
		std::ostream & out = std::cout
	) noexcept {
		if (that.next == nullptr)
		{
			out << "0";
			return out;
		}
		if (!that.data)
		{
			out << "-";
		}
		bool temp = false;
		unsigned int MinLength = 0;
		MinLength = GetPowerTimes(Radix, 10);
		unsigned OutBase = 0;
		if (MinLength == 0)
		{
			MinLength = GetPowerTimes(Radix, 16);
			if (MinLength == 0)
			{
				MinLength = GetPowerTimes(Radix, 8);
				if (MinLength == 0)
				{
					OutBase = Radix;
					out << "(Base:"
						<< Radix
						<< ")";
					temp = true;
				}
				else
				{
					OutBase = 8;
					out << "0" << std::setbase(8);
				}
			}
			else {
				OutBase = 16;
				out << "0x"
					<< std::setbase(16);
			}
		}
		else
		{
			OutBase = 10;
		}

		SinglePrint<Type>(*(that.next), out, temp, MinLength, OutBase);
		out << std::setbase(10);
		return out;
	}
	//�ص��ӿ�
	//DataΪԴ��������������Դ������
	//SubDataΪȡ�������ݵ�����
	//TypeΪԴ��������
	//SubTypeΪ������
	template<
		typename Data,
		typename SubData,
		typename Type,
		typename SubType
	>
		SubType GetSubList(
			const SubData& (__stdcall* GetFunction)(const Data&),
			const Type& that
		) noexcept {
		const Type* OprtPtr = &that;
		SubType Result(GetFunction(that.data));
		SubType* SubOprtPtr = &Result;
		while (true)
		{
			if (OprtPtr->next != nullptr)
			{
				SubOprtPtr->next = new SubType(GetFunction(OprtPtr->next->data));
			}
			else
			{
				break;
			}
			OprtPtr = OprtPtr->next;
		}
		return Result;
	}
	template<class node, typename Data, Data _Max = std::numeric_limits<Data>::max()>
	class LinkedListComputeTraits
	{
	public:
		__stdcall LinkedListComputeTraits() = delete;
		__stdcall ~LinkedListComputeTraits() = delete;
		static inline node NullNode = node(0, nullptr);
		constexpr static inline node* NullIterator = &NullNode;
		static const inline size_t length = Array::GetMinLength(_Max);
		static const inline Array::Bytes<length> Max = Array::Bytes<length>(_Max);
		static const inline Array::Bytes<length> Radix = Array::Bytes<length>(Max + Array::Bytes<length>(1));

		static Data& GetData(node* ptr) { return ptr->data; }
		static const Data& GetData(const node* ptr) { return ptr->data; }

		static node* GetNext(node* ptr) { return ptr->next; }
		static const node* GetNext(const node* ptr) { return ptr->next; }

		static void Add(Data& Res, bool& Carry, const Data a, const Data b) {
			Array::Bytes<length> Sum = Array::Bytes<length>(a);
			Sum += Array::Bytes<length>(b);
			if (Carry)
			{
				Sum += Array::Bytes<length>(1);
			}
			Res = Data(Sum % Radix);
			Carry = ((Sum / Radix > Array::Bytes<length>(0)) ? true : false);
		}
		static void Subtract(Data& Res, bool& Carry, const Data a, const Data b) {
			if (a >= b)
			{
				Res = (a - b);
			}
			else
			{

			}
		}

		static void InsertAfter(node** ptr) { (*ptr)->insert(); }
	private:

	};

#ifdef OLL
#undef OLL
#endif // OLL
#ifdef temp
#undef temp
#endif // temp
}
#ifdef LL_LENGTH
#undef LL_LENGTH
#endif // LL_LENGTH
#ifdef LL_SIMPLIFY
#undef LL_SIMPLIFY
#endif // LL_SIMPLIFY


#pragma pack(pop)