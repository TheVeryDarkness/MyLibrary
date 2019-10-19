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
		OAL *next;
		void cut_node()noexcept {
			assert(!noMoreNode());
			if (this->next->noMoreNode()) {
				delete this->next;
				this->next = reinterpret_cast<OAL *>(&this->next);
			}
			else {
				auto tmp = this->next->next;
				delete this->next;
				this->next = tmp;
			}
		}
		void insert_node_after(Data n)noexcept {
			assert(justFull());		
			this->next = new OAL(n);
		}
		void next_move_forward()noexcept {
			//assert(noMoreNode());
			++(*reinterpret_cast<Data **>(&next));
		}
		void push_back_this(Data n)noexcept {
			assert(hasVacancy());
			*reinterpret_cast<Data *>(next) = n;
			next_move_forward();
		}
	#ifdef _DEBUG
	public:
	#endif // _DEBUG
		constexpr auto begin() { return iterator(this); }
		constexpr auto end() { return nullptr; }
	public:
		using value_type = Data;
		explicit OAL(Data head, OAL *next) :next(next) {
			static_assert(sizeof(OAL) == sizeof(data) + sizeof(next), "Dangerous!");
			data[0] = head; 
		}
		explicit OAL(Data head = Data()) :next(reinterpret_cast<OAL *>(this->data + 1)) {
			static_assert(sizeof(OAL) == sizeof(data) + sizeof(next), "Dangerous!");
			data[0] = head; 
		}
		~OAL() { }
		constexpr auto begin()const { return const_iterator(this); }
		constexpr auto cbegin()const { return const_iterator(this); }
		constexpr auto end()const { return nullptr; }
		constexpr auto cend()const { return nullptr; }
		constexpr bool noMoreNode()const noexcept {
			return
				this < next
				&&
				reinterpret_cast<size_t>(next) <= reinterpret_cast<size_t>(&next);
		}
		constexpr bool hasVacancy()const noexcept {
			return
				this < next
				&&
				reinterpret_cast<size_t>(next) < reinterpret_cast<size_t>(&next);
		}
		constexpr bool justFull()const noexcept {
			return reinterpret_cast<size_t>(next) == reinterpret_cast<size_t>(&next);
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
					flagPtr->next = reinterpret_cast<OAL *>(&i);
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
			else this->next->push_back(n);
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
					OprtPtr = OprtPtr->next;
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
			return (pA->data <= pD) && (pD <= reinterpret_cast<const Data *>(&pA->next));
		}
		constexpr static bool legel_iterator_ptr(const OAL *pA, const Data *pD)noexcept {
			return is_in(pA, pD) && (!is_in(pA, pA->next->data) || pD <= pA->next->data);
		}
		constexpr static bool has_ended(const OAL *pA, const Data *pD)noexcept {
			return pA->next->data == pD;
		}
		class iterator final{
		public:
			MY_LIB iterator(OAL *_ptr) :pA(_ptr), pD(_ptr->data) { }
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
				if (this->pA->next->data == pD) {
					const_cast<OAL *>(pA)->push_back(0);
				}
				if (pD == pA->data + num) {
					pA = pA->next;
					pD = pA->data;
				}
				assert(is_in(pA, pD));
				return *this;
			}
			constexpr iterator MY_LIB operator+(size_t sz)const noexcept {
				iterator it(*this);
				for (size_t i = 0; i < sz; i++) {
					if (!has_ended(pA, pD)) {
						++(it.pD);
						if (it.pD == it.pA->data + num && !pA->justFull()) {
							assert(!it.pA->noMoreNode());
							it.pA = it.pA->next;
							it.pD = it.pA->data;
						}
						assert(is_in(it.pA, it.pD));
					}
				}
				return it;
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
				assert(reinterpret_cast<const Data *>(&this->pA->next) != pD);
				return has_ended(pA, pD);
				//return reinterpret_cast<const Data *>(this->pA->next) == pD;
			}
			bool operator!=(nullptr_t)const noexcept {
				return !(*this == nullptr);
			}

		private:
			const OAL *pA;
			const Data *pD;
		};


		class const_iterator final{
		public:
			MY_LIB const_iterator(const OAL *_ptr) :pA(_ptr), pD(_ptr->data) { }
			MY_LIB ~const_iterator() { }

			const Data &MY_LIB operator*()const noexcept {
				assert(legel_iterator_ptr(pA, pD));
				return (*this == nullptr) ? ConstantBuffer<Data, 0>::get() : *pD;
			}
			const_iterator &MY_LIB operator++()noexcept {
				assert(legel_iterator_ptr(pA, pD));
				if (pD==pA->next->data) {
					(void)pD;
				}
				if (!has_ended(pA, pD)) {
					++pD;
				}
				if (!pA->noMoreNode()) {
					//assert(has_ended(pA, pD));
					assert(!pA->noMoreNode());
					pA = pA->next;
					pD = pA->data;
				}
				assert(is_in(pA, pD));
				assert(legel_iterator_ptr(pA, pD));
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
				assert(reinterpret_cast<const Data *>(&this->pA->next) != pD);
				return has_ended(pA, pD);
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