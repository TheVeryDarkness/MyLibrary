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
				return this->pA == nullptr;
			}
			bool operator!=(nullptr_t)const noexcept {
				return this->pA != nullptr;
			}

		private:
			const OAL *pA;
			const Data *pD;
		};
	protected:
		Data data[num];
		OAL *next;
	public:
		explicit OAL(Data head, OAL *next) :next(next) { data[0] = head; }
		explicit OAL(Data head = Data()) :next(reinterpret_cast<OAL *>(this->data[1])) { data[0] = head; }
		~OAL() { }
		constexpr auto begin()const { return const_iterator(this); }
		constexpr auto end()const { return nullptr; }
		constexpr auto cbegin()const { return const_iterator(this); }
		constexpr auto cend()const { return nullptr; }
		void insert(Data n)noexcept {
			assert(this != next);
			if (
				reinterpret_cast<size_t>(this) < reinterpret_cast<size_t>(next)
				&&
				reinterpret_cast<size_t>(next) < reinterpret_cast<size_t>(&next)
				) {
				*reinterpret_cast<Data *>(next) = n;
				*reinterpret_cast<Data **>(&next) += 1;
			}
			else if (reinterpret_cast<size_t>(next) == reinterpret_cast<size_t>(&next)) {
				this->next = new OAL(n);
			}
			else {
				auto tmp = this->next;
				this->next = new OAL(n, tmp);
			}
		}
		void release()noexcept {
			while (this->next!=nullptr) {

			}
		}
	};
}