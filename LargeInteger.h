#pragma once

#include "Shared.h"
#include "Statistics.h"
#include <cassert>

namespace LargeInteger {
	template<typename LL, auto radix>
	class LargeUnsigned
	{
		using radix_t=decltype(radix);
	public:
		template<typename val>
		LargeUnsigned(val Val)noexcept	{
			for (auto& i = LinkedList.begin();; ++i)
			{
				i = Val % radix;
				if ((Val /= radix) == static_cast<val>(0)) {
					break;
				}
			}
			assert(Val == static_cast<val>(0));
			return;
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
		template<typename val>
		LargeSigned(val Val)noexcept
			:PosSign(Val > 0), LargeUnsigned<LL, radix>(ABS(val)) {}

		~LargeSigned()
		{
		}

	private:
		bool PosSign;
	};

}