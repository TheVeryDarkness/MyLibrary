#pragma once

#include "Shared.h"
#include <array>

template<size_t CacheSize>
class MemorryCache
{
public:
	MY_LIB MemorryCache()noexcept{}

	MY_LIB ~MemorryCache()noexcept{
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
		for (auto& val : Cache)
		{
			if (val != nullptr)
			{
				void* temp = val;
				val = nullptr;
				return temp;
			}
		}
		return malloc(Block);
	}

	void MY_LIB push(void* block)noexcept {
		for (auto& val : Cache)
		{
			if (val == nullptr)
			{
				val = block;
				return;
			}
		}
		return free(block);
	}

private:
	void* Cache[CacheSize] = {};
};
