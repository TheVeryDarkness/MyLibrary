#pragma once

#include "Shared.h"
#include <array>

template<size_t CacheSize>
class MemoryCache
{
public:
	MY_LIB MemoryCache()noexcept{}

	MY_LIB ~MemoryCache()noexcept{
		for (auto& var : Cache)
		{
			if (var!=nullptr)
			{
				free(var);
				var = nullptr;
			}
		}
	}


	void* pop(size_t Block) {
	#ifndef _DEBUG
		for (auto &val : Cache) {
			if (val != nullptr) {
				void *temp = val;
				val = nullptr;
				return temp;
			}
		}
	#endif // !_DEBUG
		return malloc(Block);
	}

	void MY_LIB push(void* block)noexcept {
	#ifndef _DEBUG
		for (auto &val : Cache) {
			if (val == nullptr) {
				val = block;
				return;
			}
		}
	#endif // !_DEBUG
		return free(block);
	}

private:
	void* Cache[CacheSize] = {};
};
