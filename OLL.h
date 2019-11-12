#pragma once

#include "LinkedList.h"
#include "ConstantBuffer.h"

namespace Darkness {
	inline namespace LL {
		//DataΪ�������ͣ�������Ϊָ��
		template <typename Data, size_t CacheSize = 50>
		//����oneway������linked list�������ࣩ
		//Notice:
		//The head of the linked list should be in the stack or heap,
		//however, the other sections should be in the heap.
		class OLL {
			template<typename Type>
			INLINED friend std::ostream &MY_LIB out(
				std::ostream &out, const Type &b
			)noexcept;
			template<
				typename __Data,
				typename SubData,
				typename Type,
				typename SubType>
				friend SubType GetSubList(
					const SubData &(MY_LIB *GetFunction)(const __Data &),
					const Type &that
				)noexcept;
			template<class inNode, class outNode, auto inRadix, auto outRadix, bool Destroy>
			friend outNode MY_LIB Transform(inNode &in)noexcept;

			MEMORY_CACHE(CacheSize);


			struct iterator {
				using in = OLL;
			public:
				static constexpr auto MY_LIB getRadix()noexcept { return decltype(ptr->data)::getRadix(); }
				static constexpr in *MY_LIB NEXT(in &i)noexcept { if (i.next == nullptr)i.insert(); return i.next; }

				constexpr MY_LIB iterator(in *_ptr)noexcept :ptr(_ptr) { }

				constexpr bool MY_LIB operator==(const in *_ptr)const noexcept { return this->ptr == _ptr; }
				constexpr bool MY_LIB operator!=(const in *_ptr)const noexcept { return this->ptr != _ptr; }
				constexpr bool MY_LIB operator==(const iterator _ptr)const noexcept { return this->ptr == _ptr.ptr; }
				constexpr bool MY_LIB operator!=(const iterator _ptr)const noexcept { return this->ptr != _ptr.ptr; }
				constexpr bool MY_LIB operator==(nullptr_t)const noexcept { return this->ptr == nullptr; }
				constexpr bool MY_LIB operator!=(nullptr_t)const noexcept { return this->ptr != nullptr; }
				constexpr bool MY_LIB operator==(end_ptr_t)const noexcept {
					assert(this->ptr != nullptr);
					return this->ptr->next == nullptr;
				}
				constexpr bool MY_LIB operator!=(end_ptr_t)const noexcept {
					assert(this->ptr != nullptr);
					return this->ptr->next != nullptr;
				}

				constexpr in *operator->() const noexcept {
					return this->ptr;
				}

				MY_LIB ~iterator()noexcept = default;

				constexpr iterator &MY_LIB operator++()noexcept {
					if (this->ptr != nullptr) {
						if (this->ptr->next == nullptr) {
							this->ptr->insert();
						}
						this->ptr = ptr->next;
					}
					return *this;
				}

				constexpr iterator &MY_LIB operator+=(size_t sz)noexcept {
					for (size_t i = 0; i < sz && this->ptr != nullptr; i++) {
						++(*this);
					}
					return *this;
				}

				constexpr iterator MY_LIB operator+(size_t sz)const noexcept {
					iterator it(*this);
					for (size_t i = 0; i < sz; i++) {
						if (it.ptr != nullptr) {
							it.ptr = it.ptr->next;
						}
						else break;
					}
					return it;
				}

				constexpr auto &MY_LIB operator*()const noexcept {
					if (this->ptr != nullptr) {
						return this->ptr->data;
					}
					else {
						return ConstantBuffer<Data, 0>::get();
					}
				}
				constexpr void insert(iterator &it, Data d) {
					it.ptr->insert(d);
				}

			private:
				in *ptr;
				friend struct const_iterator;
			};


			struct const_iterator {
				using in = const OLL;
				friend class OLL;
			public:
				static constexpr auto MY_LIB getRadix()noexcept { return decltype(ptr->data)::getRadix(); }
				static constexpr in *MY_LIB NEXT(in &i)noexcept { if (i.next == nullptr)i.insert(); return i.next; }
				constexpr MY_LIB const_iterator(const in *_ptr)noexcept :ptr(_ptr) { }
				MY_LIB ~const_iterator()noexcept = default;

				constexpr bool MY_LIB operator==(const const_iterator &_ptr)const noexcept { return this->ptr == _ptr.ptr; }
				constexpr bool MY_LIB operator!=(const const_iterator &_ptr)const noexcept { return this->ptr != _ptr.ptr; }
				constexpr bool MY_LIB operator==(const iterator &_ptr)const noexcept { return this->ptr == _ptr.ptr; }
				constexpr bool MY_LIB operator!=(const iterator &_ptr)const noexcept { return this->ptr != _ptr.ptr; }
				constexpr bool MY_LIB operator==(nullptr_t)const noexcept { return this->ptr == nullptr; }
				constexpr bool MY_LIB operator!=(nullptr_t)const noexcept { return this->ptr != nullptr; }
				constexpr bool MY_LIB operator==(end_ptr_t)const noexcept {
					assert(this->ptr != nullptr);
					return this->ptr->next == nullptr;
				}
				constexpr bool MY_LIB operator!=(end_ptr_t)const noexcept {
					assert(this->ptr != nullptr);
					return this->ptr->next != nullptr;
				}

				constexpr const_iterator &MY_LIB operator++()noexcept {
					if (this->ptr != nullptr) {
						this->ptr = ptr->next;
					}
					return *this;
				}

				constexpr const_iterator &MY_LIB operator+=(size_t sz)noexcept {
					for (size_t i = 0; i < sz && this->ptr != nullptr; i++) {
						++(*this);
					}
					return *this;
				}

				constexpr const_iterator MY_LIB operator+(size_t sz)const noexcept {
					const_iterator it(*this);
					it += sz;
					return it;
				}

				constexpr const Data &MY_LIB operator*()const noexcept {
					if (this->ptr != nullptr) {
						return this->ptr->data;
					}
					else {
						return ConstantBuffer<Data, 0>::get();
					}
				}

			private:
				in *ptr;
			};
		public:
			using value_type = Data;
			constexpr INLINED iterator begin() noexcept {
				return iterator(this);
			}
			constexpr INLINED const_iterator begin()const noexcept {
				return const_iterator(this);
			}
			constexpr INLINED const_iterator cbegin()const noexcept {
				return const_iterator(this);
			}
			constexpr INLINED iterator end() noexcept {
				return iterator(nullptr);
			}
			constexpr INLINED const_iterator end() const noexcept {
				return const_iterator(nullptr);
			}
			constexpr INLINED const_iterator cend() const noexcept {
				return const_iterator(nullptr);
			}
			constexpr bool isNull()const noexcept {
				for (auto i : *this) {
					if (i != 0) {
						return false;
					}
				}
				return true;
			}
		protected:

			//ָ����һ�ڵ�ָ��
			OLL *next;
			//����
			Data data;
			//���ƹ��캯��
			//Ĭ��Ϊ���
			INLINED MY_LIB OLL(
				const OLL &that,
				bool DeepCopy
			) noexcept :data(that.data) {
				this->release();
				if (DeepCopy) {
					OLL *OprtPtr = this;
					const OLL *PreOprtPtr = &that;
					while (true) {
						if (PreOprtPtr->next != nullptr) {
							OprtPtr->insert(PreOprtPtr->next->data);
							OprtPtr = OprtPtr->next;
							PreOprtPtr = PreOprtPtr->next;
						}
						else {
							OprtPtr->next = nullptr;
							break;
						}
					}
				}
				else {
					this->next = that.next;
				}
				return;
			}
		public:
			INLINED size_t MY_LIB RawLength()const noexcept {
				LL_LENGTH(OLL);
			}
			INLINED MY_LIB OLL(const OLL &that) = delete;
			//�ƶ����캯��
			//Ĭ��Ϊǳ����
			INLINED MY_LIB OLL(
				OLL &&that
			) noexcept :data(that.data), next(that.next) {
				that.next = nullptr;
			}
			//����ʼ������ͷ�Ĺ��캯��
			constexpr explicit INLINED MY_LIB OLL(
				Data HeadData,
				OLL *NextPtr = nullptr
			) noexcept :data(HeadData), next(NextPtr) { }
			//�ͷ�����ͷ���Ӧ���ڵ�ָ��
			INLINED void MY_LIB destruct() noexcept {
				this->release();
				this->data = Data(0);
				return;
			}
			//�ͷ�����ͷ���Ӧ���ڵ�ָ��
			INLINED void MY_LIB release() noexcept {
				while (this->next != nullptr) {
					this->erase_after();
				}
				return;
			}
			//����
			INLINED void MY_LIB swap(
				OLL &that
			) noexcept {
				std::swap(this->data, that.data);
				std::swap(this->next, that.next);
				return;
			}
			//���Ǹ�ֵ
			/*INLINED*/void MY_LIB SetValue(
				size_t num, const Data *data
			) noexcept {
				auto OprtPtr = this->begin();//������ǰ����
				OprtPtr->data = data;
				size_t count = 1;
				while (true) {
					if (count >= num) {
						break;
					}
					if (OprtPtr->next == nullptr) {
						OprtPtr->insert();
					}
					else {
						break;
					}
					++OprtPtr;
					OprtPtr->data = &data[count - 1];
					count++;
				}
				return;
			}
			//���Ǹ�ֵ
			//��Ϊ��֪����ԭ�򣬶�SBLinkedList����
			/*INLINED*/void __cdecl SetValue(
				size_t num, Data data, ...
			) noexcept {
				auto OprtPtr = this->begin();//������ǰ����
				OprtPtr->data = data;
				size_t count = 1;
				while (true) {
					if (count >= num) {
						break;
					}
					if (OprtPtr->next == nullptr) {
						OprtPtr->insert();
					}
					else {
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
			INLINED OLL &MY_LIB operator=(OLL &&that) noexcept {
				if (&that == this) {
					return *this;
				}
				this->destruct();
				this->data = that.data;
				this->next = that.next;
				that.next = nullptr;
				return *this;
			}
			/*INLINED*/bool MY_LIB operator==(
				const OLL &that
				)const noexcept {
				const OLL *OprtPtr = this, *PreOprtPtr = &that;
				while (true) {
					if (OprtPtr == nullptr
						&&
						PreOprtPtr == nullptr) {
						return true;
					}
					else if (OprtPtr == nullptr
						||
						PreOprtPtr == nullptr) {
						return false;
					}
					if (OprtPtr->data != PreOprtPtr->data) {
						return false;
					}
					OprtPtr = OprtPtr->next;
					PreOprtPtr = PreOprtPtr->next;
				}
			}
			bool MY_LIB operator!=(const OLL &that)const noexcept {
				return !(*this == that);
			}
			//������ͷ������������ͷ����ʼ������֮�������
			//ʹ��new����������
			/*INLINED*/OLL MY_LIB invert(const OLL &b) const noexcept {
				OLL Result(b.data);
				const OLL *OprtPtr = &b;
				while (OprtPtr->next != nullptr) {
					Result.insert(OprtPtr->next->data);
					OprtPtr = (OprtPtr->next);
				}
				return Result;
			}
			//�ڵ�ǰλ�ú�����µ�һ��
			INLINED void insert(Data New = Data(false)) noexcept {
				OLL *temp = this->next;
				this->next = DBG_NEW OLL(New, temp);
				return;
			}
			//ɾ����ǰλ�ú��һλ
			INLINED void MY_LIB erase_after() noexcept(DEBUG_FLAG) {
				assert(this->next != nullptr);
				//if (this->next)
				{
					OLL *temp = this->next->next;
					delete this->next;
					this->next = temp;
				}
				return;
			}
			//ɾ����ǰλ�ú��һλ
			static INLINED void MY_LIB erase_after(const_iterator begin) noexcept {
				const_cast<OLL *>(begin.ptr)->erase_after();
				return;
			}
			//ɾ����ǰλ�ú��һλ
			static INLINED void MY_LIB erase_after(const_iterator begin, nullptr_t) noexcept {
				while (begin + 1 != nullptr) erase_after(begin);
				return;
			}
			//������ǰλ�ú��һλ
			INLINED Data MY_LIB pop() noexcept(DEBUG_FLAG) {
				Data temp = this->next->data;
				this->erase_after();
				return temp;
			}
			INLINED OLL *MY_LIB Simplify() noexcept {
				LL_SIMPLIFY(OLL);
			}
			//�ж�that�����Ƿ�Ϊ�����ĩβ
			//��ʹ��ȱʡ��������ʾ��ȡ��ǰ�����Ƿ�Ϊ�����ĩβ
			INLINED bool MY_LIB IsEnding(
				const OLL *that = nullptr
			)const noexcept {
				return ((that == nullptr)
					?
					(this->next == nullptr)
					:
					(that->next == nullptr));
			}
			INLINED friend std::ostream &MY_LIB operator<<(
				std::ostream &out, const OLL &that) noexcept {
				return LL::out<OLL>(out, that);
			}

			void MY_LIB SinglePrint(
				std::ostream &out = std::cout,
				bool Comma = true
			)const noexcept {
				return LL::SinglePrint<OLL>(*this, out);
			}

			OLL &operator<<=(size_t sz)noexcept {
				for (unsigned int index = 0; index < sz; index++) {
					this->insert(this->data);
					this->data = Data(0);
				}
				return *this;
			}

			OLL &operator>>=(size_t sz)noexcept {
				for (unsigned int index = 0; index < sz; index++) {
					this->data = this->pop();
				}
				return *this;
			}
		protected:
			//��ȡthat���ڴ洢������
			//��ʹ��ȱʡ��������ʾ��ȡ��ǰ���ڴ洢������
			INLINED const Data &MY_LIB GetThisData(
				const OLL *that = nullptr
			)const noexcept {
				return ((that == nullptr) ?
					this->data
					:
					that->data);
			}
			//��ȡthat���ڵ���һ����
			//��ʹ��ȱʡ��������ʾ��ȡ��ǰ���ڵ���һ����
			INLINED const OLL *MY_LIB GetNext(
				const OLL *that = nullptr
			)const noexcept {
				return ((that == nullptr) ?
					this->next
					:
					that->next);
			}
		};
	}
}