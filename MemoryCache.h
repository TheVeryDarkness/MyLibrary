#pragma once

#include "Shared.h"
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
		return (first_null == 0) ? malloc(Block) : Cache[--first_null];
	}

	void MY_LIB push(void *block)noexcept {
		if (first_null == CacheSize) {
			return free(block);
		}
		else {
			Cache[first_null++] = block;
		}
	}

private:
	void* Cache[CacheSize] = {};
	size_t first_null = 0;
};
