#pragma once
#include <iostream>

namespace Darkness {
	inline namespace Physics {
		//长度L、质量M、时间T、电流强度I、温度t、物质的量n和光强度J
		template<typename Data = int>
		class Dimension {
		private:
			Data L, M, T, I, t, n, J;
		public:
			constexpr Dimension(Data L, Data M, Data T, Data I, Data t, Data n, Data J)noexcept
				:L(L), M(M), T(T), I(I), t(t), n(n), J(J) { }
			~Dimension() { }
			Dimension operator*(const Dimension &that)const noexcept {
				return Dimension(
					this->L + that.L,
					this->M + that.M, 
					this->T + that.T, 
					this->I + that.I, 
					this->t + that.t,
					this->n + that.n,
					this->J + that.J
				);
			}
			Dimension operator/(const Dimension &that)const noexcept {
				return Dimension(
					this->L - that.L,
					this->M - that.M, 
					this->T - that.T, 
					this->I - that.I, 
					this->t - that.t,
					this->n - that.n,
					this->J - that.J
				);
			}
			friend std::ostream &operator<<(std::ostream &o, const Dimension &dim)noexcept {
				bool has = false;
				constexpr auto mul = " * ";
				if (dim.L != 0) { 
					o << "m";
					if (dim.L != 1)o << "^(" << dim.L << ')'; 
					has = true;
				}
				if (dim.M != 0) { 
					o << (has ? mul : "") << "kg"; 
					if (dim.M != 1)o << "^(" << dim.M << ')'; 
					has = true;
				}
				if (dim.T != 0) { 
					o << (has ? mul : "") << "s";
					if (dim.T != 1)o << "^(" << dim.T << ')'; 
					has = true;
				}
				if (dim.I != 0) { 
					o << (has ? mul : "") << "A";
					if (dim.I != 1)o << "^(" << dim.I << ')';
					has = true;
				}
				if (dim.t != 0) { 
					o << (has ? mul : "") << "K";
					if (dim.t != 1)o << "^(" << dim.t << ')';
					has = true;
				}
				if (dim.n != 0) { 
					o << (has ? mul : "") << "mol";
					if (dim.n != 1)o << "^(" << dim.n << ')';
					has = true;
				}
				if (dim.J != 0) {
					o << (has ? mul : "") << "cd";
					if (dim.J != 1)o << "^(" << dim.J << ')';
					has = true;
				}
				return o << '\b';
			}
		};

		//长度L、质量M、时间T、电流强度I、温度t、物质的量n和光强度J
		template<typename DatT, typename DimT = int>
		class Quantity :protected Dimension<DimT> {
			using super = Dimension<DimT>;
			DatT data;
		public:
			constexpr Quantity(DatT dt, DimT L = 0, DimT M = 0, DimT T = 0, DimT I = 0, DimT t = 0, DimT n = 0, DimT J = 0)
				:data(dt), super(L, M, T, I, t, n, J) { }
			constexpr Quantity(DatT dt, const super &that)
				: data(dt), super(that) { }
			Quantity operator*(const Quantity &that)const noexcept {
				return Quantity(this->data * that.data, static_cast<const super &>(*this) *static_cast<const super &>(that));
			}
			Quantity operator/(const Quantity &that)const noexcept {
				return Quantity(this->data / that.data, static_cast<const super &>(*this) / static_cast<const super &>(that));
			}
			~Quantity() { }
			friend std::ostream &operator<<(std::ostream &o, const Quantity &dim)noexcept {
				return o << dim.data << static_cast<const super &>(dim);
			}
		};

		[[nodiscard]] Quantity<unsigned long long>(operator "" _m)(unsigned long long length)noexcept {
			return Quantity(length, 1);
		}
		[[nodiscard]] Quantity<unsigned long long>(operator "" _kg)(unsigned long long mass)noexcept {
			return Quantity(mass, 0, 1);
		}
		[[nodiscard]] Quantity<unsigned long long>(operator "" _s)(unsigned long long time)noexcept {
			return Quantity(time, 0, 0, 1);
		}
		[[nodiscard]] Quantity<unsigned long long>(operator "" _A)(unsigned long long current)noexcept {
			return Quantity(current, 0, 0, 0, 1);
		}
		[[nodiscard]] Quantity<unsigned long long>(operator "" _K)(unsigned long long temperature)noexcept {
			return Quantity(temperature, 0, 0, 0, 0, 1);
		}
		[[nodiscard]] Quantity<unsigned long long>(operator "" _mol)(unsigned long long Mol)noexcept {
			return Quantity(Mol, 0, 0, 0, 0, 0, 1);
		}
		[[nodiscard]] Quantity<unsigned long long>(operator "" _N)(unsigned long long force)noexcept {
			return Quantity(force, 1, 1, -2);
		}
		[[nodiscard]] Quantity<unsigned long long>(operator "" _J)(unsigned long long work)noexcept {
			return Quantity(work, 2, 1, -2);
		}
	}
}