#pragma once

#include "Shared.h"
#include <cassert>
#include <array>

template<size_t CacheSize>
class MemoryCache
{
public:
	MY_LIB MemoryCache()noexcept{}

	MY_LIB ~MemoryCache()noexcept {
		for (void **p = Cache + first_null - 1; Cache - 1 != p; --p) {
			assert(*p != nullptr);
			free(*p);
			*p = nullptr;
		}
	#ifdef _DEBUG
		for (auto i : Cache) {
			assert(i == nullptr);
		}
	#endif // _DEBUG
	}


	void* pop(size_t Block) {
	#ifdef _DEBUG
		return malloc(Block);
	#else
		return (first_null == 0) ? malloc(Block) : Cache[--first_null];
	#endif // _DEBUG
	}

	void MY_LIB push(void *block)noexcept {
	#ifdef _DEBUG
		return free(block);
	#else
		return (first_null == CacheSize) ? free(block) : (void)(Cache[first_null++] = block);
	#endif // _DEBUG
	}

private:
	void* Cache[CacheSize] = {};
	size_t first_null = 0;
};
