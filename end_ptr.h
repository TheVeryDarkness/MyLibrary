#pragma once


struct end_ptr_t {
	template<typename T>constexpr bool __stdcall operator==(T *ptr)noexcept {
		return !(ptr + 1);
	}
};

static inline end_ptr_t end_ptr;