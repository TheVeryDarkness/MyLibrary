#pragma once

namespace Math {
	template<typename T,size_t sz>
	class arr {
	public:
		template<typename induce>
		constexpr arr(induce& ind) noexcept {
			for (auto& i : data) {
				if constexpr (std::is_same_v<T, decltype(ind())>) {
					i = ind();
				}
				else {
					i = T(ind);
				}
			}
		}
		~arr() { }
		

		constexpr T& operator[](size_t index)noexcept {
			return data[index];
		}
		constexpr const T& operator[](size_t index)const noexcept {
			return data[index];
		}


		template<typename os>
		friend os& operator<<(os& o, const arr& a)noexcept {
			for (auto const& i : a.data) {
				o << i << ' ';
			}
			return o << '\b';
		}

	private:
		T data[sz];
	};

}