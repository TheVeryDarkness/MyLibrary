#pragma once

#include "Shared.h"

namespace LargeInteger {
	template<typename LL, auto radix>
	class LargeUnsigned
	{
		using radix_t=decltype(radix);
	public:
		LargeUnsigned(val Val)noexcept	{
			for (auto i:LinkedList)
			{
				i = Val % radix;
				i /= radix;
			}
		}

		~LargeUnsigned(){
		}

	private:
		LL LinkedList;
	};
	template<typename LL, auto radix>
	class LargeSigned:private LargeUnsigned<LL, radix>
	{
	public:
		LargeSigned()
		{
		}

		~LargeSigned()
		{
		}

	private:
		bool Sign;
	};

}