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
		class iterator;
		class const_iterator;
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
		~OAL() { }
		constexpr auto begin()const { return const_iterator(this); }
		constexpr auto cbegin()const { return const_iterator(this); }
		constexpr auto end()const { return nullptr; }
		constexpr auto cend()const { return nullptr; }
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
			return is_in(pA, pD) && (!is_in(pA, pA->flag_next->data) || pD <= pA->flag_next->data + 1);
		}
		constexpr static bool has_ended(const OAL *pA, const Data *pD)noexcept {
			return pA->flag_next->data + 1 == pD;
		}
		constexpr static bool ending(const OAL *pA, const Data *pD)noexcept {
			return pA->flag_next->data == pD;
		}
		constexpr static bool is_at_out_place(const OAL *pA, const Data *pD)noexcept {
			return pA->data + num == pD;
		}
		class const_iterator final{
			friend class iterator;
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
				if (!is_at_out_place(pA, pD) && !has_ended(pA, pD)) {
					++pD;
					assert(is_in(pA, pD));
				}
				if (is_at_out_place(pA, pD) && !pA->noMoreNode()) {
					//assert(has_ended(pA, pD));
					pA = pA->flag_next;
					pD = pA->data;
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
				return this->pA;
			}
			bool operator==(const const_iterator &that)const noexcept {
				return this->pA == that->pA && this->pD == that->pD;
			}
			bool operator!=(const const_iterator &that)const noexcept {
				return !(*this == that);
			}
			bool operator==(nullptr_t)const noexcept {
				return has_ended(pA, pD) || is_at_out_place(pA, pD);
			}
			bool operator!=(nullptr_t)const noexcept {
				return !(*this == nullptr);
			}

		private:
			const OAL *pA;
			const Data *pD;
		};
		class iterator final {
			friend class const_iterator;
		public:
			explicit MY_LIB iterator(OAL *_ptr) :pA(_ptr), pD(_ptr->data) { }
			MY_LIB iterator(OAL *_pA, Data *_pD) : pA(_pA), pD(_pD) { }
			MY_LIB ~iterator() { }

			Data &MY_LIB operator*()noexcept {
				assert(legel_iterator_ptr(pA, pD));
				return (*this == nullptr) ? ConstantBuffer<Data, 0>::get() : *const_cast<Data *>(pD);
			}
			const Data &MY_LIB operator*()const noexcept {
				assert(legel_iterator_ptr(pA, pD));
				return (*this == nullptr) ? ConstantBuffer<Data, 0>::get() : *pD;
			}
			iterator &MY_LIB operator++()noexcept {
				++pD;
				if (has_ended(pA, pD)) {
					const_cast<OAL *>(pA)->push_back(0);
				}
				if (is_at_out_place(pA, pD)) {
					pA = pA->flag_next;
					pD = pA->data;
				}
				assert(is_in(pA, pD));
				return *this;
			}
			constexpr iterator MY_LIB operator+(size_t sz)const noexcept {
				const_iterator it(this->pA, this->pD);
				for (size_t i = 0; i < sz; i++) {
					++it;
				}
				iterator i(const_cast<OAL *>(it.pA), const_cast<Data *>(it.pD));
				return i;
			}
			OAL *operator->() noexcept {
				return const_cast<OAL *>(this->pA);
			}
			const OAL *operator->()const noexcept {
				return this->pA;
			}
			constexpr bool operator==(const iterator &that)const noexcept {
				return this->pA == that->pA && this->pD == that->pD;
			}
			constexpr bool operator!=(const iterator &that)const noexcept {
				return !(*this == that);
			}
			bool operator==(nullptr_t)const noexcept {
				return has_ended(pA, pD) || is_at_out_place(pA, pD);
				//return reinterpret_cast<const Data *>(this->pA->next) == pD;
			}
			bool operator!=(nullptr_t)const noexcept {
				return !(*this == nullptr);
			}

		private:
			const OAL *pA;
			const Data *pD;
		};
	};
}