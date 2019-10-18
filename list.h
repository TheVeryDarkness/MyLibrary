#pragma once

#include "ConstantBuffer.h"
#include "LinkedList.h"

namespace LL {
	template<typename Data, size_t num, size_t CacheSize = 50>
	class OAL {
		MEMORY_CACHE(CacheSize);
		class const_iterator {
		public:
			MY_LIB const_iterator(const OAL *_ptr):pA(_ptr),pD(_ptr->data) { }
			MY_LIB ~const_iterator() { }

			const Data& MY_LIB operator*()noexcept {
				return *pD;
			}
			const_iterator &MY_LIB operator++()noexcept {
				++pD;
				if (pD - pA->data == num) {
					pA = pA->next;
					pD = pA->data;
				}
				return *this;
			}
			bool operator==(const const_iterator &that)const noexcept { 
				return this->pA == that->pA && this->pD == that->pD;
			}
			bool operator!=(const const_iterator &that)const noexcept {
				return !(*this == that);
			}
			bool operator==(nullptr_t)const noexcept { 
				return reinterpret_cast<const Data *>(this->pA->next) == pD;
			}
			bool operator!=(nullptr_t)const noexcept {
				return !(*this == nullptr);
			}

		private:
			const OAL *pA;
			const Data *pD;
		};
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
	public:
		explicit OAL(Data head, OAL *next) :next(next) {
			static_assert(sizeof(OAL) == sizeof(data) + sizeof(next), "Dangerous!");
			data[0] = head; 
		}
		explicit OAL(Data head = Data()) :next(reinterpret_cast<OAL *>(&this->data[1])) {
			static_assert(sizeof(OAL) == sizeof(data) + sizeof(next), "Dangerous!");
			data[0] = head; 
		}
		~OAL() { }
		constexpr auto begin()const { return const_iterator(this); }
		constexpr auto end()const { return nullptr; }
		constexpr auto cbegin()const { return const_iterator(this); }
		constexpr auto cend()const { return nullptr; }
		constexpr bool noMoreNode()const noexcept {
			return
				reinterpret_cast<size_t>(this) < reinterpret_cast<size_t>(next)
				&&
				reinterpret_cast<size_t>(next) <= reinterpret_cast<size_t>(&next);
		}
		constexpr bool hasVacancy()const noexcept {
			return
				reinterpret_cast<size_t>(this) < reinterpret_cast<size_t>(next)
				&&
				reinterpret_cast<size_t>(next) < reinterpret_cast<size_t>(&next);
		}
		constexpr bool justFull()const noexcept {
			return reinterpret_cast<size_t>(next) == reinterpret_cast<size_t>(&next);
		}
		void insert(Data n)noexcept {
			assert(this != next);
			if (hasVacancy()) {
				*reinterpret_cast<Data *>(next) = n;
				++*reinterpret_cast<Data **>(&next);
			}
			else if (justFull()) {
				this->next = new OAL(n);
			}
			else {
				auto tmp = this->next;
				this->next = new OAL(n, tmp);
			}
		}
		void release()noexcept {
			while (!noMoreNode()) {
				this->cut_node();
			}
		}
	};
}