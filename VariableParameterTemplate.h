#pragma once

namespace Template{
	template<size_t, typename T, T, T...> constexpr static T get()noexcept;
	template<typename func, typename T, T...>constexpr static T for_each();
	template<size_t...> class product;


	template<size_t index, typename T, T head, T... pack>
	constexpr static T get()noexcept {
		if constexpr (index == 0) {
			return head;
		}
		else return get<index - 1, T, pack...>();
	}


	template<typename func, typename T, T...>constexpr static T for_each();

	template<size_t head, size_t ...pack>
	class product<head, pack...> {
	public:
		static constexpr size_t value()noexcept {
			return head * product<pack...>::value();
		}
	};

	template<size_t head>
	class product<head> {
	public:
		static constexpr size_t value()noexcept {
			return head;
		}
	};
}