#pragma once

#include <Shared.h>
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
		auto i = std::find_if_not(Cache.begin(), Cache.end(), [](void* that)->bool {return(that == nullptr); });
		if (i == Cache.end())
		{
			return malloc(Block);
		}
		else {
			void* temp = *i;
			*i = nullptr;
			return temp;
		}
	}

	void MY_LIBRARY push(void* block)noexcept {
		auto i = std::find_if(Cache.begin(), Cache.end(), [](void* that)->bool {return(that == nullptr); });
		if (i == Cache.end())
		{
			return free(block);
		}
		else {
			*i = block;
			return;
		}
	}

private:
	std::array<void*, CacheSize> Cache;
};
