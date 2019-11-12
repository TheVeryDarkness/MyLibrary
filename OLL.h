#pragma once

#include "LinkedList.h"
#include "ConstantBuffer.h"

namespace Darkness {
	inline namespace LL {
		//Data为数据类型，勿将其置为指针
		template <typename Data, size_t CacheSize = 50>
		//单向（oneway）链表（linked list）（基类）
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

			//指向下一节的指针
			OLL *next;
			//数据
			Data data;
			//复制构造函数
			//默认为深拷贝
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
			//移动构造函数
			//默认为浅拷贝
			INLINED MY_LIB OLL(
				OLL &&that
			) noexcept :data(that.data), next(that.next) {
				that.next = nullptr;
			}
			//仅初始化链表头的构造函数
			constexpr explicit INLINED MY_LIB OLL(
				Data HeadData,
				OLL *NextPtr = nullptr
			) noexcept :data(HeadData), next(NextPtr) { }
			//释放链表头后对应链节的指针
			INLINED void MY_LIB destruct() noexcept {
				this->release();
				this->data = Data(0);
				return;
			}
			//释放链表头后对应链节的指针
			INLINED void MY_LIB release() noexcept {
				while (this->next != nullptr) {
					this->erase_after();
				}
				return;
			}
			//交换
			INLINED void MY_LIB swap(
				OLL &that
			) noexcept {
				std::swap(this->data, that.data);
				std::swap(this->next, that.next);
				return;
			}
			//覆盖赋值
			/*INLINED*/void MY_LIB SetValue(
				size_t num, const Data *data
			) noexcept {
				auto OprtPtr = this->begin();//操作当前对象
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
			//覆盖赋值
			//因为不知名的原因，对SBLinkedList禁用
			/*INLINED*/void __cdecl SetValue(
				size_t num, Data data, ...
			) noexcept {
				auto OprtPtr = this->begin();//操作当前对象
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
			//覆盖赋值
			//浅拷贝
			//将清除被赋值对象原有内容
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
			//从链表头（不包括链表头）开始，倒置之后的链节
			//使用new创建新链表
			/*INLINED*/OLL MY_LIB invert(const OLL &b) const noexcept {
				OLL Result(b.data);
				const OLL *OprtPtr = &b;
				while (OprtPtr->next != nullptr) {
					Result.insert(OprtPtr->next->data);
					OprtPtr = (OprtPtr->next);
				}
				return Result;
			}
			//在当前位置后插入新的一节
			INLINED void insert(Data New = Data(false)) noexcept {
				OLL *temp = this->next;
				this->next = DBG_NEW OLL(New, temp);
				return;
			}
			//删除当前位置后的一位
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
			//删除当前位置后的一位
			static INLINED void MY_LIB erase_after(const_iterator begin) noexcept {
				const_cast<OLL *>(begin.ptr)->erase_after();
				return;
			}
			//删除当前位置后的一位
			static INLINED void MY_LIB erase_after(const_iterator begin, nullptr_t) noexcept {
				while (begin + 1 != nullptr) erase_after(begin);
				return;
			}
			//弹出当前位置后的一位
			INLINED Data MY_LIB pop() noexcept(DEBUG_FLAG) {
				Data temp = this->next->data;
				this->erase_after();
				return temp;
			}
			INLINED OLL *MY_LIB Simplify() noexcept {
				LL_SIMPLIFY(OLL);
			}
			//判断that链节是否为链表的末尾
			//若使用缺省参数，表示获取当前链节是否为链表的末尾
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
			//获取that链节存储的数据
			//若使用缺省参数，表示获取当前链节存储的数据
			INLINED const Data &MY_LIB GetThisData(
				const OLL *that = nullptr
			)const noexcept {
				return ((that == nullptr) ?
					this->data
					:
					that->data);
			}
			//获取that链节的下一链节
			//若使用缺省参数，表示获取当前链节的下一链节
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