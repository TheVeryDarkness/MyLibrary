#pragma once

namespace Template{
	template<size_t, typename T, T, T...> constexpr static T get()noexcept;
	template<typename func, typename T, T head, T...>constexpr static void for_each()noexcept;
	template<size_t...> class product;


	template<size_t index, typename T, T head, T... pack>
	constexpr static T get()noexcept {
		if constexpr (index == 0) {
			return head;
		}
		else return get<index - 1, T, pack...>();
	}

	template<size_t index, typename T, T head>
	constexpr static T get()noexcept {
		static_assert(index == 0, "Out of range");
		return head;
	}


	template<typename Func, typename T, T head, T... pack>constexpr static void for_each()noexcept {
		Func f;
		f(head);
		if constexpr (0 != sizeof...(pack)) {
			for_each<Func, T, pack...>();
		}
		return;
	}

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