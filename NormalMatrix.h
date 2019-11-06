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
			static_assert(std::is_invocable_v<r, size_t, size_t>, "Failed in Reduction");
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
		//return the rank
		template<bool standard = true>
		size_t to_upper_tri()noexcept {
			size_t current_x = 0;
			//前y列已经上三角化
			for (size_t y = 0; y < n; ++y) {
				//前current_x已经有非零元
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
						//逐行运算
						for (size_t i = 0; i < m; ++i) {
							if (i == x)continue;
							if (mat[i][y] != 0) {
								const val ratio = mat[x][y] / mat[i][y];
								mat[i][y] = 0;
								for (size_t j = y + 1; j < n; ++j) {
									mat[i][j] *= ratio;
									mat[i][j] -= mat[x][j];
								}
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