#pragma once

#ifdef _DEBUG
#include "VisualStudioDebug.h"
#endif // _DEBUG
#include "Abandoned.h"

#include <iostream>
#include <array>
#include <iomanip>
#include <string>
#include <charconv>
#include "Exception.h"
#include "Statistics.h"
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
	template <typename Data, Data Max>
	class OLL;
	template<typename Data, Data Max>
	class DLL;
	template<
		typename Data,
		typename SubData,
		typename Type,
		typename SubType>
		SubType GetSubList(
			const SubData& (MY_LIBRARY* GetFunction)(const Data&),
			const Type& that
		)noexcept;


	//����


	//DataΪ�������ͣ�������Ϊָ��
	template <
		typename Data, Data Max
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
		inline friend void MY_LIBRARY multiply(
			Class& a,
			const Class& b
		)noexcept;
		template<class Class, typename Data, unsigned long Radix>
		inline friend void MY_LIBRARY multiply(
			Class& a,
			int times)noexcept;
		template<typename Type, unsigned long Radix>
		/*inline*/friend std::ostream& MY_LIBRARY out(
			std::ostream& out, const Type& b
		)noexcept;
		template<typename Type>
		inline friend void MY_LIBRARY SinglePrint(
			const Type& that,
			std::ostream& out,
			bool ShowComma,
			unsigned MinLength,
			unsigned base
		)noexcept;
		template<typename Type, auto Radix>
		/*inline*/friend std::ostream& MY_LIBRARY _Print(
			const Type& that,
			std::ostream& out
		)noexcept;
		template<
			typename Data,
			typename SubData,
			typename Type,
			typename SubType>
			friend SubType GetSubList(
				const SubData& (MY_LIBRARY* GetFunction)(const Data&),
				const Type& that
			)noexcept;

		//�ڴ滺��
		static inline std::array<void*, 20> Buffer = {};
		static void* MY_LIBRARY operator new(size_t size) {
			auto i = std::find_if_not(Buffer.begin(), Buffer.end(), [](void* that)->bool {return(that == nullptr); });
			if (i==Buffer.end())
			{
				return malloc(size);
			}
			else {
				void* temp = *i;
				*i = nullptr;
				return temp;
			}
		}
		static void MY_LIBRARY operator delete(void* _ptr, size_t size) {
			auto i = std::find_if(Buffer.begin(), Buffer.end(),  [](void* that)->bool {return(that == nullptr); });
			if (i == Buffer.end())
			{
				return free(_ptr);
			}
			else {
				*i = _ptr;
			}
		}
	public:
		//����
		/*inline*/void MY_LIBRARY operator*=(int times) noexcept {
			LL::multiply<OLL, Data, Max + Data(1)>(*this, times);
		}
		//����
		/*inline*/OLL MY_LIBRARY operator*(int times)const noexcept {
			OLL Res(*this, true);
			Res *= times;
			return Res;
		}
		//����
		/*inline*/void MY_LIBRARY operator*=(const OLL& b) noexcept {
			LL::multiply<OLL, Data, Max + Data(1)>(*this, b);
		}
		//����
		/*inline*/OLL MY_LIBRARY operator*(const OLL& b)const noexcept {
			OLL Res(*this, true);
			Res *= b;
			return Res;
		}
		//����
		inline void MY_LIBRARY operator-=(const OLL& b) noexcept {
			*this += (-b);
		}
		//����OLL������
		inline OLL MY_LIBRARY operator-(
			)const noexcept {
			OLL res(*this);
			res.data = !res.data;
			return res;
		}
		//����OLL����Ӻ�
		inline OLL MY_LIBRARY operator+(
			const OLL& b//������
			)  const noexcept {
			OLL Result(*this, true);//�洢���
			Result += b;
			return Result;
		}
		//����OLL����+=
		inline void MY_LIBRARY operator+=(const OLL& that) noexcept {
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
				LongCompute::AddTo<OLL*, Data, LinkedListComputeTraits<OLL, Data, Max>>(that.next, this->next);
			}
			else {
				short Cmpr = LongCompute::Compare<OLL*, Data, LinkedListComputeTraits<OLL, Data, Max>>(this->next, that.next);
				if (Cmpr == LongCompute::Equal)
				{
					this->destruct();
				}
				if (Cmpr == LongCompute::Larger)
				{
					LongCompute::SubtractFrom<OLL*, Data, LinkedListComputeTraits<OLL, Data, Max>>(that.next, this->next);
				}
				else
				{
					OLL temp(that, true);
					LongCompute::SubtractFrom<OLL*, Data, LinkedListComputeTraits<OLL, Data, Max>>(this->next, temp.next);
					*this = temp;
				}
			}
			this->Simplify();
		}
		//����OLL�������
		/*inline*/OLL MY_LIBRARY operator-(
			const OLL& b//������
			)const noexcept {
			return (*this + (-b));
		}
		void MY_LIBRARY operator++() {
			OLL temp(true, 1);
			*this += temp;
			temp.destruct();
		}
		inline MY_LIBRARY ~OLL() {
			this->next = nullptr;
		}
#ifdef _DEBUG
	public:
#else
	protected:
#endif // _DEBUG

		//ָ����һ�ڵ�ָ��
		OLL* next = nullptr;
		//����
		Data data;
		//���ƹ��캯��
		//Ĭ��Ϊ���
		inline MY_LIBRARY OLL(
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
		inline unsigned long MY_LIBRARY GetMax()const noexcept {
			return Max;
		}
		inline size_t MY_LIBRARY RawLength()const noexcept {
			LL_LENGTH(OLL);
		}
		//���ƹ��캯��
		//Ĭ��Ϊǳ����
		inline MY_LIBRARY OLL(
			const OLL& that
		) noexcept {
			this->data = that.data;
			this->destruct();
			this->next = that.next;
			return;
		}
		inline MY_LIBRARY OLL(
			bool positive,
			unsigned short value
		) {
			this->data = positive;
			*this = value;
		}
		//����ʼ������ͷ�Ĺ��캯��
		constexpr explicit inline MY_LIBRARY OLL(
			Data HeadData,
			OLL* NextPtr = nullptr
		) noexcept {
			if constexpr (Max == 0)
			{
				this->data = HeadData;
				this->next = NextPtr;
			}
			else
			{
				this->data = Data(HeadData);
				this->next = NextPtr;
			}
			return;
		}
		//�ͷ�����ͷ���Ӧ���ڵ�ָ��
		inline void MY_LIBRARY destruct() noexcept {
			while (this->next != nullptr)
			{
				this->cut();
			}
			return;
		}
		//�ͷ�����ͷ���Ӧ���ڵ�ָ��
		inline void MY_LIBRARY operator~() noexcept {
			this->destruct();
			return;
		}
		//����������ֵ
		/*inline*/Data MY_LIBRARY MaxInAllNodes() const noexcept {
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
		/*inline*/Data MY_LIBRARY MinInAllNodes()const noexcept {
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
		/*inline*/Data MY_LIBRARY Average()const noexcept
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
		inline void MY_LIBRARY swap(
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
		/*inline*/void MY_LIBRARY SetValue(
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
		inline OLL& MY_LIBRARY operator=(
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
		/*inline*/bool MY_LIBRARY operator==(
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
				if (LongCompute::Compare<OLL*, Data, LinkedListComputeTraits<OLL, Data>>(this, &that) == LongCompute::Smaller)
					return true;
				else return false;
			}
			else
			{
				if (LongCompute::Compare<OLL*, Data, LinkedListComputeTraits<OLL, Data>>(&that, this) == LongCompute::Smaller)
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
				if (LongCompute::Compare<OLL*, Data, LinkedListComputeTraits<OLL, Data>>(this, &that) == LongCompute::Larger)
					return true;
				else return false;
			}
			else
			{
				if (LongCompute::Compare<OLL*, Data, LinkedListComputeTraits<OLL, Data>>(&that, this) == LongCompute::Larger)
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
				LongCompute::DivideInto<OLL*, Data, LinkedListComputeTraits<OLL, Data, Max>>(that.next, this->next);
			}
			else return;
			this->Simplify();
		}
		void MY_LIBRARY operator/=(const OLL& that)noexcept {
			if (this->next != nullptr && that.next != nullptr)
			{
				OLL Res(this->data);
				LongCompute::DivideInto<OLL, OLL*, Data, LinkedListComputeTraits<OLL, Data, Max>>(Res, that.next, this->next);
				*this = Res;
			}
			else return;
			this->Simplify();
		}
		//������ͷ������������ͷ����ʼ������֮�������
		//ʹ��new����������
		/*inline*/OLL MY_LIBRARY invert(const OLL& b) const noexcept {
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
		inline void MY_LIBRARY cut() noexcept {
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
		inline OLL* MY_LIBRARY Simplify() noexcept {
			LL_SIMPLIFY(OLL);
		}
		//���Ǹ�ֵ
		/*inline*/OLL& MY_LIBRARY operator=(
			long value
			) noexcept {
			if constexpr (Max == Data(0))
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
						if constexpr (Max == std::numeric_limits<Data>::max())
						{
							OprtPtr->insert(static_cast<Data>(value));
						}
						else OprtPtr->insert((Data)(value % (Max + 1)));
					}
					OprtPtr = OprtPtr->next;
					if constexpr (Max == std::numeric_limits<Data>::max())
					{
						value >>= sizeof(Data) * Array::BitsPerByte;
					}
					else value = value / (Max + 1);
				}
			}
			return *this;
		}
		//�ж�that�����Ƿ�Ϊ�����ĩβ
		//��ʹ��ȱʡ��������ʾ��ȡ��ǰ�����Ƿ�Ϊ�����ĩβ
		inline bool MY_LIBRARY IsEnding(
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
		/*inline*/long long  MY_LIBRARY GetValue()const noexcept {
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
					value += ((unsigned long long)(OprtPtr->data)) * (unsigned long long)pow(static_cast<long long>(Max) + 1, n);
					n++;
				}
				else
				{
					break;
				}
			}
			return (sign) ? value : (-value);
		}
		inline friend std::ostream& MY_LIBRARY operator<<(
			std::ostream& out, const OLL& that) noexcept {
			return LL::out<OLL>(out, that);
		}
		void MY_LIBRARY SinglePrint(
			std::ostream& out = std::cout,
			bool Comma = true
		)const noexcept {
			return LL::SinglePrint<OLL>(*this, out);
		}
		//���������������̨����
		/*inline*/std::ostream& MY_LIBRARY Print(
			std::ostream& out = std::cout
		) const noexcept {
			return LL::_Print<OLL, Max + Data(1)>(*this, out);
		}
	protected:
		//��ȡthat���ڴ洢������
		//��ʹ��ȱʡ��������ʾ��ȡ��ǰ���ڴ洢������
		inline const Data& MY_LIBRARY GetThisData(
			const OLL* that = nullptr
		)const noexcept {
			return ((that == nullptr) ?
				this->data
				:
				that->data);
		}
		//��ȡthat���ڵ���һ����
		//��ʹ��ȱʡ��������ʾ��ȡ��ǰ���ڵ���һ����
		inline const OLL* MY_LIBRARY GetNext(
			const OLL* that = nullptr
		)const noexcept {
			return ((that == nullptr) ?
				this->next
				:
				that->next);
		}
	};
	template<typename Data, Data Max>
	class DLL
	{
		//static const inline std::forward_list<Data> Factor = PrimeFactorList(static_cast<Data>(Radix));
		//static const inline Data MaxFactor = MinConti(Factor);

		template<class node, typename Data, Data _Max>
		friend class LinkedListComputeTraits;
		//��Ԫ��������

		template<class Class, typename Data, unsigned long Radix>
		//����
		inline friend void MY_LIBRARY multiply(
			Class& a,
			const Class& b
		)noexcept;
		template<class Class, typename Data, unsigned long Radix>
		inline friend void MY_LIBRARY multiply(
			Class& a,
			int times)noexcept;
		template<typename Type, unsigned long Radix>
		/*inline*/friend std::ostream& MY_LIBRARY out(
			std::ostream& out, const Type& b
		)noexcept;
		template<typename Type>
		inline friend void MY_LIBRARY SinglePrint(
			const Type& that,
			std::ostream& out,
			bool ShowComma,
			unsigned MinLength,
			unsigned base
		)noexcept;
		template<typename Type, auto Radix>
		/*inline*/friend std::ostream& MY_LIBRARY _Print(
			const Type& that,
			std::ostream& out
		)noexcept;
		template<
			typename Data,
			typename SubData,
			typename Type,
			typename SubType>
			friend SubType GetSubList(
				const SubData& (MY_LIBRARY* GetFunction)(const Data&),
				const Type& that
			)noexcept;
		friend class Q;
	public:
		//����
		inline void MY_LIBRARY operator*=(int times) {
			LL::multiply<DLL, Data, Max + Data(1)>(*this, times);
		}
		//����
		inline DLL MY_LIBRARY operator*(int times)const {
			DLL Res(*this, true);
			Res *= times;
			return Res;
		}
		//����
		/*inline*/void MY_LIBRARY operator*=(const DLL& b) {
			LL::multiply<DLL, Data, Max + Data(1)>(*this, b);
		}
		//����
		inline DLL MY_LIBRARY operator*(const DLL& b)const {
			DLL Res(*this, true);
			Res *= b;
			return Res;
		}
		//���ؾ���ֵ
		inline const DLL MY_LIBRARY abs()const noexcept {
			return ((this->data > 0) ? *this : (-*this));
		}
		//������
		inline bool MY_LIBRARY IsPositive()const noexcept {
			return (this->data > 0);
		}
		//���ظ�
		inline void MY_LIBRARY SetToContradict()noexcept {
			this->data = ((this->data == 0) ? 1 : 0);
		}
		//����DLL����Ӻ�
		inline DLL MY_LIBRARY operator+(
			const DLL& b//������
			)  const noexcept {
			DLL Result(*this, true);//�洢���
			Result += b;
			return Result;
		}
		//����DLL����+=
		inline void MY_LIBRARY operator+=(const DLL& that)noexcept {
			if (that.next == nullptr)
			{
				return;
			}
			if (this->next == nullptr)
			{
				*this = DLL(that, true);
				return;
			}
			if ((this->data > 0 && that.data > 0) || (this->data == 0 && that.data == 0))
			{
				LongCompute::AddTo<DLL*, Data, LinkedListComputeTraits<DLL, Data, Max>>(that.next, this->next);
			}
			else {
				short Cmpr = LongCompute::Compare<DLL*, Data, LinkedListComputeTraits<DLL, Data, Max>>(this->next, that.next);
				if (Cmpr == LongCompute::Equal)
				{
					this->destruct();
				}
				if (Cmpr == LongCompute::Larger)
				{
					LongCompute::SubtractFrom<DLL*, Data, LinkedListComputeTraits<DLL, Data, Max>>(that.next, this->next);
				}
				else
				{
					DLL temp(that, true);
					LongCompute::SubtractFrom<DLL*, Data, LinkedListComputeTraits<DLL, Data, Max>>(this->next, temp.next);
					*this = temp;
				}
			}
			this->Simplify();
		}
		//����DLL�������
		inline DLL MY_LIBRARY operator-(
			const DLL& b//������
			)const noexcept {
			return (*this + (-b));
		}
		//����
		inline void MY_LIBRARY operator-=(const DLL& b)noexcept {
			DLL _b = -b;
			*this += _b;
			_b.destruct();
		}
		//����
		inline void MY_LIBRARY operator-=(DLL& b)noexcept {
			Data Orig = b.data;
			b.data = !b.data;
			*this += b;
			b.data = Orig;
		}
		//����DLL������
		inline DLL MY_LIBRARY operator-(
			)const noexcept {
			DLL res(*this, true);
			res.data = !res.data;
			return res;
		}
		void MY_LIBRARY operator++() {
			DLL temp(true, 1);
			*this += temp;
			temp.destruct();
		}
		inline _stdcall ~DLL() {
			this->next = nullptr;
			this->last = nullptr;
		}
#ifdef _DEBUG
	public:
#else
	protected:
#endif // _DEBUG
		Data data;
		DLL* next = nullptr;
		DLL* last = nullptr;
		//�����ǳ�����ɲ���DeepCopyָ��
		explicit inline MY_LIBRARY DLL(
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
		constexpr explicit inline MY_LIBRARY DLL(
			Data HeadData = Data(false),
			DLL* NextPtr = nullptr,
			DLL* LastPtr = nullptr
		)noexcept :
			data(HeadData),
			next(NextPtr),
			last(LastPtr) {}
		//ǳ����
		inline MY_LIBRARY DLL(
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
		inline void MY_LIBRARY operator=(
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
		inline bool MY_LIBRARY operator==(
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
				if (OprtPtr->data != (that % (Max + 1)))
					return false;
				else
				{
					that /= (Max + 1);
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
		inline bool MY_LIBRARY operator==(
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
		inline bool MY_LIBRARY operator<(const DLL& that)const noexcept {
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
		inline bool MY_LIBRARY operator<=(const DLL& that)const noexcept {
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
		inline bool MY_LIBRARY operator>(const DLL& that)const noexcept {
			return (that < *this);
		}
		inline bool MY_LIBRARY operator>=(const DLL& that)const noexcept {
			return (that <= *this);
		}
		inline void MY_LIBRARY destruct() noexcept {
			while (this->next != nullptr)
			{
				this->cut();
			}
			return;
		}
		inline void MY_LIBRARY operator~() noexcept {
			this->destruct();
			return;
		}
		//�ڵ�ǰλ�ú�����µ�һ��
		inline void MY_LIBRARY insert(
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
		inline void MY_LIBRARY cut() noexcept {
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
		inline DLL* MY_LIBRARY Simplify() noexcept {
			LL_SIMPLIFY(DLL);
		}
	protected:
		//ˢ��lastָ��
		//��ָ֤����ȷ
		//����ĩβָ��
		inline DLL* MY_LIBRARY Fresh() noexcept {
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
		inline DLL* MY_LIBRARY GetEnd()const noexcept {
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
		inline size_t MY_LIBRARY RawLength()const noexcept {
			LL_LENGTH(DLL);
		}
		explicit inline MY_LIBRARY DLL(
			bool positive,
			unsigned long value
		) noexcept {
			if constexpr (Max == 0)
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
							(Data)(value % (Max + 1))
						);
						OprtPtr = OprtPtr->next;
						if constexpr (Max == std::numeric_limits<Data>::max())
						{
							value >>= sizeof(Data) * Array::BitsPerByte;
						}
						else value /= (Max + 1);
					}
				}
			}
		}
		//ǳ����
		//���Ǹ�ֵ
		inline DLL& MY_LIBRARY operator=(
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
		inline void MY_LIBRARY operator/=(
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
			if constexpr (Max == 0)
			{
				return *this;
			}
			if (that % (Max + 1) == 0) {
				return ((*this >>= 1) /= (that / (Max + 1)));
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
		inline void MY_LIBRARY operator%=(unsigned long that) noexcept {
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
			if constexpr (Max == 0)
			{
				return *this;
			}
			if ((Max + 1) % that == 0) {
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
				OprtPtr->data = (OprtPtr->data + Upper * (Max + 1)) % that;
				Upper = OprtPtr->data;
			}
			OprtPtr->destruct();
			return *this;
		}
		inline void MY_LIBRARY operator%=(const DLL& that) noexcept {
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
		inline void MY_LIBRARY operator/=(const DLL& that) noexcept {
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
		inline friend std::ostream& MY_LIBRARY operator<<=(
			std::ostream& out, const DLL& that
			) noexcept {
			return LL::out<DLL>(out, that);
		}
		inline void MY_LIBRARY SinglePrint(
			std::ostream& out = std::cout,
			bool Comma = true
		)const noexcept {
			return LL::SinglePrint<DLL>(*this, out);
		}
		//���������������̨����
		inline std::ostream& MY_LIBRARY Print(
			std::ostream& out = std::cout
		)const noexcept {
			return LL::_Print<DLL, Max + Data(1)>(*this, out);
		}
	};

	template<typename Type>
	//�����
	/*inline*/std::ostream& MY_LIBRARY out(
		std::ostream& out, const Type& b
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
	inline void MY_LIBRARY SinglePrint(
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
	template<typename Type, auto Radix = 0>
	//���������������̨����
	//�����Զ�����
	/*inline*/std::ostream & MY_LIBRARY _Print(
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
			const SubData& (MY_LIBRARY* GetFunction)(const Data&),
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
	class LinkedListComputeTraits :public Array::SampleTraits<Data, _Max>
	{
	public:
		MY_LIBRARY LinkedListComputeTraits() = delete;
		MY_LIBRARY ~LinkedListComputeTraits() = delete;
		static inline Data NullData = 0;
		constexpr static inline node* NullIterator = nullptr;

		static Data& MY_LIBRARY GetData(node* ptr) { return ((ptr == nullptr) ? (NullData = 0) : (ptr->data)); }

		static node* MY_LIBRARY GetNext(node* ptr) { return ((ptr == nullptr) ? nullptr : (ptr->next)); }

		static void MY_LIBRARY assign(node* ptr, size_t sz) { *ptr <<= sz; }
		static void MY_LIBRARY InsertAfter(node** ptr, Data data) { (*ptr)->insert(data); }

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