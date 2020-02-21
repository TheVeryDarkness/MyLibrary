#pragma once

#include "Shared.h"
#include <cassert>

template<typename Data, auto val = 0>
class ConstantBuffer {
public:
	ConstantBuffer() = delete;
	~ConstantBuffer() = delete;
	constexpr static Data &get()noexcept {
		assert(NullData == val);
		return (NullData = val);
	}
protected:
	static inline Data NullData = val;
};
