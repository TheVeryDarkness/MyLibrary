#pragma once

#include "LargeInteger.h"
#include "OLL.h"
#include "DLL.h"
#include "LargeRational.h"
#include "list.h"

namespace Darkness {
	inline namespace LargeInteger {
		constexpr unsigned int m_radix = 1000000000U;

		typedef LL::OLL<unsigned int> forward_list;
		typedef LL::DLL<unsigned int> list;
		typedef LL::OAL<unsigned int, 64U / sizeof(void *) - 1> m_forward_list;

		typedef LargeSigned<std::forward_list<unsigned __int32>, m_radix> Z;
		typedef LargeUnsigned<std::forward_list<unsigned __int32>, m_radix> N;
		typedef RationalSigned<N> Q;
		typedef RationalUnsigned<N> uQ;



	}
}