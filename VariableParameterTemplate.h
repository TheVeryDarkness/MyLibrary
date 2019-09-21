#pragma once

namespace Template{
	template<typename T, T...>class Index;
	template<size_t...> class product;



	template<typename T, T head, T... pack>
	class Index<T, head, pack...> {
	public:
		Index() = delete;
		~Index() = delete;
		constexpr static T get(T index)noexcept {
			if constexpr (index == 0) {
				return head;
			}
			else return Index<T, pack...>::get(index - 1);
		}
		constexpr static T getEnd()noexcept {
			return Index<T, pack...>::getEnd();
		}
	private:
	};

	template<typename T, T head>
	class Index<T, head> {
	public:
		Index() = delete;
		~Index() = delete;
		constexpr static T get(size_t index)noexcept {
			static_assert(index == 0, "The index is out of range.");
			return head;
		}
		constexpr static T getEnd()noexcept {
			return head;
		}
	private:
	};

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