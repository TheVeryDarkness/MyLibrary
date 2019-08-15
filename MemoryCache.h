#pragma once

#include "Shared.h"
#include <array>

template<size_t CacheSize>
class MemorryCache
{
public:
	MY_LIBRARY MemorryCache()noexcept{}

	MY_LIBRARY ~MemorryCache()noexcept{
		for (auto var : Cache)
		{
			if (var!=nullptr)
			{
				free(var);
				var = nullptr;
			}
		}
	}


	void* pop(size_t Block) {
		for (size_t i = 0; i < CacheSize; i++)
		{
			if (Cache[i] != nullptr)
			{
				void* temp = Cache[i];
				Cache[i] = nullptr;
				return temp;
			}
		}
		return malloc(Block);
	}

	void MY_LIBRARY push(void* block)noexcept {
		for (size_t i = 0; i < CacheSize; i++)
		{
			if (Cache[i] == nullptr)
			{
				Cache[i] = block;
				return;
			}
		}
		return free(block);
	}

private:
	void* Cache[CacheSize] = {};
};
