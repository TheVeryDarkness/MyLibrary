#pragma once


struct end_ptr_t {
	template<typename iter>constexpr bool __stdcall operator==(iter ptr)noexcept {
		return !(ptr + 1);
	}
};

static inline end_ptr_t end_ptr;