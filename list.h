#pragma once

#include "ConstantBuffer.h"
#include "LinkedList.h"

namespace LL {
	//As for class OAL,
	//	if its pointer "next" points to a place inside itself,
	//	it means that it has no next node,
	//	pointer "next" points to the last element it's using;
	//	otherwise, ths pointer points to the next node.
	template<typename Data, size_t num, size_t CacheSize = 50>
	class OAL {
		static_assert(!std::is_array_v<Data>, "Array type is not available.");
		MEMORY_CACHE(CacheSize);
	protected:
		Data data[num];
		OAL *flag_next;
		void cut_node()noexcept {
			assert(!noMoreNode());
			if (this->flag_next->noMoreNode()) {
				delete this->flag_next;
				this->flag_next = reinterpret_cast<OAL *>(this->data + (num - 1));
			}
			else {
				auto tmp = this->flag_next->flag_next;
				delete this->flag_next;
				this->flag_next = tmp;
			}
		}
		void insert_node_after(Data n)noexcept {
			assert(justFull());		
			this->flag_next = new OAL(n);
		}
		void next_move_forward()noexcept {
			//assert(noMoreNode());
			++(*reinterpret_cast<Data **>(&flag_next));
		}
		void push_back_this(Data n)noexcept {
			assert(hasVacancy());
			next_move_forward();
			*reinterpret_cast<Data *>(flag_next) = n;
		}
	#ifdef _DEBUG
	public:
	#endif // _DEBUG
		constexpr auto begin() { return iterator(this); }
		constexpr auto end() { return nullptr; }
	public:
		using value_type = Data;
		explicit OAL(Data head, OAL *next) :flag_next(next) {
			data[0] = head; 
		}
		explicit OAL(Data head = Data()) :flag_next(reinterpret_cast<OAL *>(this->data)) {
			data[0] = head; 
		}
		explicit OAL(OAL &&that) noexcept {
		#pragma omp parallel for
			for (size_t i = 0; i < num; ++i) {
				this->data[i] = that.data[i];
			}
			if (is_in(&that, that.flag_next->data)) {
				this->flag_next = reinterpret_cast<OAL *>(this->data + (that.flag_next->data - that.data));
			}
			else { 
				this->flag_next = that.flag_next;
				that.flag_next = reinterpret_cast<OAL *>(that.data + (num - 1));
			}
		}
		OAL &operator=(const OAL &that)noexcept {
			if (&that == this) {
				return *this;
			}
			this->destruct();
			memcpy(this->data, that.data, sizeof(data));
			if (is_in(&that, that.flag_next->data)) {
				this->flag_next = reinterpret_cast<OAL *>(this->data + (that.flag_next->data - that.data));
			}
			else {
				this->flag_next = that.flag_next;
			}
			return *this;
		}
		explicit OAL(const OAL &that) {
			if (is_in(&that, that.flag_next->data)) {
				this->flag_next = reinterpret_cast<OAL *>(this->data + (that.flag_next->data - that.data));
			}
			else {
				this->flag_next = that.flag_next;
			}
			memcpy(this->data, that.data, sizeof(data));
		}
		~OAL() { }
		constexpr auto begin()const { return const_iterator(this); }
		constexpr auto cbegin()const { return const_iterator(this); }
		constexpr auto end()const { return nullptr; }
		constexpr auto cend()const { return nullptr; }
		constexpr bool operator==(const OAL &that)const noexcept {
			for (auto i = this->cbegin(), j = that.cbegin(); i != this->cend() || j != cend(); ++i, ++j) {
				if (*i != *j) {
					return false;
				}
			}
			return true;
		}
		constexpr bool operator!=(const OAL &that)const noexcept {
			return !(*this == that);
		}
		constexpr void swap(OAL &that)noexcept {
			if (is_in(this, this->flag_next->data)) {
				size_t tmp = this->flag_next->data - this->data;
				this->flag_next = reinterpret_cast<OAL *>(this->data + (that.flag_next->data - that.data));
				that.flag_next = reinterpret_cast<OAL *>(that.data + tmp);
			}
			else {
				std::swap(this->flag_next, that.flag_next);
			}
			Data *i = this->data, *j = that.data;
			for (size_t n = 0; n < num; ++n, ++i, ++j) {
				std::swap(*i, *j);
			}
		}
		constexpr bool isNull()const noexcept {
			for (auto i : *this) {
				if (i != 0) {
					return false;
				}
			}
			return true;
		}
		constexpr bool noMoreNode()const noexcept {
			return
				this <= flag_next
				&&
				flag_next->data < data + num;
		}
		constexpr bool hasVacancy()const noexcept {
			return
				this <= flag_next
				&&
				flag_next->data < data + (num - 1);
		}
		constexpr bool justFull()const noexcept {
			return flag_next->data == data + (num - 1);
		}
		constexpr void Simplify()noexcept {
			OAL *flagPtr = this, *oprtPtr = this;
			while (!oprtPtr->noMoreNode()) {
				for (const auto &i : oprtPtr->data) {
					if (i != Data(0)) {
						flagPtr = oprtPtr;
						break;
					}
				}
			}
			flagPtr->release();
			for (auto &i : flagPtr->data) {
				if (i != Data(0)) {
					flagPtr->flag_next = reinterpret_cast<OAL *>(&i);
				}
			}
		}
		void push_back(Data n)noexcept {
			if (hasVacancy()) {
				push_back_this(n);
			}
			else if (justFull()) {
				insert_node_after(n);
			}
			else this->flag_next->push_back(n);
		}
		void release()noexcept {
			while (!noMoreNode()) {
				this->cut_node();
			}
		}
		void destruct()noexcept {
			this->release();
			memset(this->data, 0, sizeof(data));
		}
		void SHL(Data last)noexcept {
			for (OAL *OprtPtr = this; ; ) {
				if (OprtPtr->hasVacancy()) {
					memcpy(OprtPtr->data + 1, OprtPtr->data, sizeof(Data[num - 1]));
					OprtPtr->data[0] = last;
					OprtPtr->next_move_forward();
				}
				else if (OprtPtr->justFull()) {
					OprtPtr->insert_node_after(OprtPtr->data[num - 1]);
					memcpy(OprtPtr->data + 1, OprtPtr->data, sizeof(Data[num - 1]));
					OprtPtr->data[0] = last;
				}
				else {
					assert(!OprtPtr->noMoreNode());
					Data tmp = last;
					last = OprtPtr->data[num - 1];
					memcpy(OprtPtr->data + 1, OprtPtr->data, sizeof(Data[num - 1]));
					OprtPtr->data[0] = tmp;
					OprtPtr = OprtPtr->flag_next;
					continue;
				}
				break;
			}
		}
		OAL &operator<<=(size_t sz)noexcept {
			for (size_t i = 0; i < sz; ++i) {
				this->SHL(0);
			}
			return *this;
		}
		OAL &operator>>=(size_t sz)noexcept {

		}
	private:
		constexpr static bool is_in(const OAL *pA, const Data *pD)noexcept {
			return (pA->data <= pD) && (pD <= pA->data + num);
		}
		constexpr static bool legel_iterator_ptr(const OAL *pA, const Data *pD)noexcept {
			return
				(pA == nullptr && pD == nullptr)
				||
				(is_in(pA, pD)
				&&
				(!is_in(pA, pA->flag_next->data) || pD <= pA->flag_next->data));
		}
		constexpr static bool ending(const OAL *pA, const Data *pD)noexcept {
			return pA->flag_next->data == pD;
		}
		constexpr static bool at_last_place_of_this_node(const OAL *pA, const Data *pD)noexcept {
			return pA->data + (num - 1) == pD;
		}
		//It will points to nullptr after moving out of range.
		class const_iterator {
			friend class iterator;
			const_iterator &operator=(nullptr_t)noexcept {
				this->pA = nullptr;
				this->pD = nullptr;
				return *this;
			}
		public:
			MY_LIB const_iterator(const OAL *_ptr) :pA(_ptr), pD(_ptr->data) { }
			MY_LIB const_iterator(const OAL *_pA, const Data *_pD) : pA(_pA), pD(_pD) { }
			MY_LIB ~const_iterator() { }

			const Data &MY_LIB operator*()const noexcept {
				assert(legel_iterator_ptr(pA, pD));
				return (*this == nullptr) ? ConstantBuffer<Data, 0>::get() : *pD;
			}
			const_iterator &MY_LIB operator++()noexcept {
				assert(legel_iterator_ptr(pA, pD));
				if (at_last_place_of_this_node(pA, pD)) {
					if (pA->noMoreNode()) {
						*this = nullptr;
					}
					else {
						pA = pA->flag_next;
						pD = pA->data;
						assert(legel_iterator_ptr(pA, pD));
					}
				}
				else if (*this != nullptr) {
					if (ending(pA, pD)) {
						*this = nullptr;
					}
					else ++pD;
					assert(legel_iterator_ptr(pA, pD));
				}
				return *this;
			}
			const_iterator MY_LIB operator+(size_t sz)const noexcept {
				const_iterator it(*this);
				for (size_t i = 0; i < sz; ++i) {
					++it;
				}
				return it;
			}
			const OAL *operator->()const noexcept {
				assert(pA != nullptr);
				return this->pA;
			}
			bool operator==(const const_iterator &that)const noexcept {
				return this->pA == that->pA && this->pD == that->pD;
			}
			bool operator!=(const const_iterator &that)const noexcept {
				return !(*this == that);
			}
			bool operator==(nullptr_t)const noexcept {
				return pA == nullptr && pD == nullptr;
			}
			bool operator!=(nullptr_t)const noexcept {
				return !(*this == nullptr);
			}

		protected:
			const OAL *pA;
			const Data *pD;
		};
		//It will points to nullptr after moving out of range.
		class iterator :protected const_iterator {
			friend class const_iterator;
			using super = const_iterator;
		public:
			explicit MY_LIB iterator(OAL *_ptr) :super(_ptr) { }
			MY_LIB iterator(OAL *_pA, Data *_pD) : super(_pA, _pD) { }
			MY_LIB ~iterator() { }

			Data &MY_LIB operator*()noexcept {
				assert(legel_iterator_ptr(super::pA, super::pD));
				return (*this == nullptr) ? ConstantBuffer<Data, 0>::get() : *const_cast<Data *>(super::pD);
			}
			const Data &MY_LIB operator*()const noexcept {
				assert(legel_iterator_ptr(super::pA, super::pD));
				return (*this == nullptr) ? ConstantBuffer<Data, 0>::get() : *super::pD;
			}
			iterator &MY_LIB operator++()noexcept {
				assert(legel_iterator_ptr(super::pA, super::pD));
				if (ending(super::pA, super::pD)) {
					const_cast<OAL *>(super::pA)->push_back(0);
				}
				if (at_last_place_of_this_node(super::pA, super::pD)) {
					super::pA = super::pA->flag_next;
					super::pD = super::pA->data;
				}
				else ++super::pD;
				assert(legel_iterator_ptr(super::pA, super::pD));
				return *this;
			}
			constexpr iterator MY_LIB operator+(size_t sz)const noexcept {
				iterator it(const_cast<OAL *>(this->super::pA), const_cast<Data *>(this->super::pD));
				for (size_t i = 0; i < sz; ++i) {
					static_cast<const_iterator *>(&it)->operator++();
				}
				return it;
			}
			OAL *operator->() noexcept {
				return const_cast<OAL *>(this->super::pA);
			}
			const OAL *operator->()const noexcept {
				return this->super::pA;
			}
			constexpr bool operator==(const iterator &that)const noexcept {
				return this->super::pA == that->super::pA && this->super::pD == that->super::pD;
			}
			constexpr bool operator!=(const iterator &that)const noexcept {
				return !(*this == that);
			}
			bool operator==(nullptr_t)const noexcept {
				assert(legel_iterator_ptr(super::pA, super::pD));
				return this->super::operator==(nullptr);
				//return reinterpret_cast<const Data *>(this->pA->next) == pD;
			}
			bool operator!=(nullptr_t)const noexcept {
				return !(*this == nullptr);
			}
		};
	};
}