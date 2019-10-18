#pragma once

#include "OLL.h"
#include "DLL.h"

namespace LL {
	template<typename Data, size_t align = 64, size_t CacheSize = 50>
	class alignas(align) OAL :public OLL<Data[(align - sizeof(void *)) / sizeof(Data)], CacheSize> {
		static_assert(align >= sizeof(void *) + sizeof(Data), "Alignment is too small.");
		using super = OLL<Data[(align - sizeof(void *)) / sizeof(Data)], CacheSize>;
	public:
		OAL() { }

		~OAL() { }

	private:

	};
	template<typename Data, size_t align = 64, size_t CacheSize = 50>
	class DAL :public DLL<Data[(align - 2 * sizeof(void *)) / sizeof(Data)], CacheSize> {
		static_assert(align >= 2 * sizeof(void *) + sizeof(Data), "Alignment is too small.");
		using super = DLL<Data[(align - 2 * sizeof(void *)) / sizeof(Data)], CacheSize>;
	public:
		DAL() { }

		~DAL() { }

	private:

	};

}