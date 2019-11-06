#pragma once

inline namespace Math{
	template<typename val>
	class NormalMatrix {
	private:
		static_assert(!std::is_integral_v<val>, "Useless type.");
		val **mat;
	public:
		const size_t m, n;
		NormalMatrix(size_t m, size_t n) :m(m), n(n), mat(new val *[m]) {
			for (size_t i = 0; i < m; ++i) {
				mat[i] = new val[n];
			}
		}
		NormalMatrix(const NormalMatrix &that) = delete;
		NormalMatrix(NormalMatrix &&that) = delete;
		~NormalMatrix() {
			for (size_t i = 0; i < m; i++) {
				delete[] mat[i];
			}
			delete[] mat;
		}
		void fill()noexcept {
			for (size_t i = 0; i < m; i++) {
				for (size_t j = 0; j < n; j++) {
					mat[i][j] = rand();
				}
			}
		}
		//reduction
		template<typename red>void fill(red r)noexcept {
			static_assert(std::is_invocable_v<red, size_t, size_t>, "Failed in Reduction");
			for (size_t i = 0; i < m; i++) {
				for (size_t j = 0; j < n; j++) {
					mat[i][j] = r(i,j);
				}
			}
		}
		friend std::ostream &operator<<(std::ostream &o, const NormalMatrix &matrix)noexcept {
			for (size_t i = 0; i < matrix.m; i++) {
				for (size_t j = 0; j < matrix.n; j++) {
					o << matrix.mat[i][j] << '\t';
				}
				o << '\b' << std::endl;
			}
			return o;
		}
		//j is the begin index
		void line_sub_from_line(size_t i1, size_t i2, size_t j = 0)noexcept {
			for (; j < n; ++j) {
				mat[i2][j] -= mat[i1][j];
			}
		}
		//j is the begin index
		void line_sub_from_mul_line(size_t i1, size_t i2, const val &times, size_t j = 0)noexcept {
			for (; j < n; ++j) {
				mat[i2][j] -= mat[i1][j] * times;
			}
		}
		void line_mul(size_t i, const val &times, size_t j = 0)noexcept {
			for (; j < n; ++j) {
				mat[i][j] *= times;
			}
		}
		//return the rank
		template<bool standard = true>
		size_t to_upper_tri()noexcept {
			size_t current_x = 0;
			//0 ~ y - 1列已经上三角化
			for (size_t y = 0; y < n; ++y) {
				//0 ~ current_x行已经有非零元
				for (size_t x = current_x; x < m; ++x) {
					if (mat[x][y] == 0) continue;
					else {
						std::swap(mat[current_x], mat[x]);
						std::cout << *this << std::endl;
						if constexpr (standard) {
							for (size_t j = y + 1; j < n; j++) {
								mat[current_x][j] /= mat[current_x][y];
							}
							mat[current_x][y] = 1;
						}
						//逐行消除同列其他非零元
						for (size_t i = 0; i < m; ++i) {
							if (i == current_x)continue;
							if (mat[i][y] != 0) {
								const val ratio = mat[i][y] / mat[current_x][y];
								mat[i][y] = 0;
								line_sub_from_mul_line(current_x, i, ratio, y + 1);
								std::cout << *this << std::endl;
							}
						}
						++current_x;
					}
				}
			}
			return current_x;
		}
	};
}