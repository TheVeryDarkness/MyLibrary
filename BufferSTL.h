#pragma once

namespace Darkness {
	template<typename container, auto val>
	class Buffer {
	public:
		static container &MY_LIB get() noexcept { return data; }
	private:
		static inline container data;
	};
}