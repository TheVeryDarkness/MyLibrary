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

		template<auto radix = m_radix>using z = LargeSigned<std::forward_list<decltype(radix)>, radix>;
		template<auto radix = m_radix>using n = LargeUnsigned<std::forward_list<decltype(radix)>, radix>;
		template<auto radix = m_radix>using q = RationalSigned<n<radix>>;
		template<auto radix = m_radix>using uq = RationalUnsigned<n<radix>>;
		using Z = z<m_radix>;
		using N = n<m_radix>;
		using Q = q<m_radix>;
		using uQ = uq<m_radix>;
	}
}