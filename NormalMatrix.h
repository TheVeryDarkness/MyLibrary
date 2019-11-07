#pragma once

inline namespace Math{
	template<typename val>
	class NormalMatrix {
	public:
		static_assert(!std::is_integral_v<val>, "Useless type.");
		const size_t m, n;
	private:
		val **mat;
	public:
		NormalMatrix(size_t m, size_t n) :m(m), n(n), mat(new val *[m]) {
			for (size_t i = 0; i < m; ++i) {
				mat[i] = new val[n];
			}
		}
		NormalMatrix(const NormalMatrix &that):m(that.m),n(that.n),mat(new val*[m]) {
			for (size_t i = 0; i < m; ++i) {
				mat[i] = new val[n];
				for (size_t j = 0; j < n; ++j) {
					mat[i][j] = that.mat[i][j];
				}
			}
		}
		NormalMatrix(NormalMatrix &&that)noexcept
			:mat(that.mat), m(that.m), n(that.n) {
			that.mat = nullptr;
		}
		~NormalMatrix() {
			for (size_t i = 0; i < m; i++) {
				delete[] mat[i];
			}
			delete[] mat;
		}
		void fill()noexcept {
			for (size_t i = 0; i < m; ++i) {
				for (size_t j = 0; j < n; ++j) {
					mat[i][j] = rand();
				}
			}
		}
		//reduction
		template<typename red>void fill(red r)noexcept(std::is_nothrow_invocable_r_v<val, red, size_t, size_t>) {
			static_assert(std::is_invocable_r_v<val, red, size_t, size_t>, "Failed in Reduction");
			for (size_t i = 0; i < m; ++i) {
				for (size_t j = 0; j < n; ++j) {
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
				auto tmp = std::move(mat[i1][j] * times);
				mat[i2][j] -= tmp;
			}
		}
		void line_mul(size_t i, const val &times, size_t j = 0)noexcept {
			for (; j < n; ++j) {
				mat[i][j] *= times;
			}
		}
		//return the rank
		template<bool standard = true>
		size_t to_diagon()noexcept {
			size_t current_x = 0;
			//0 ~ y - 1列已经上三角化
			for (size_t y = 0; y < n; ++y) {
				//0 ~ current_x行已经有非零元
				for (size_t x = current_x; x < m; ++x) {
					if (mat[x][y] == 0) continue;
					else {
						std::swap(mat[current_x], mat[x]);
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