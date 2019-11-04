#pragma once

namespace Darkness {
	//长度L、质量M、时间T、电流强度I、温度t、物质的量n和光强度J
	template<typename Data=int>
	class Dimension {
	public:
		Dimension(Data L, Data M, Data T, Data I, Data t, Data n, Data J)noexcept
			:L(L), M(M), T(T), I(I), t(t), n(n), J(J) { }
		~Dimension() { }
	private:
		Data L, M, T, I, t, n, J;
	};

	//长度L、质量M、时间T、电流强度I、温度t、物质的量n和光强度J
	template<typename DatT,typename DimT=int>
	class Quantity:protected Dimension<DimT> {
		using super = Dimension<DimT>;
	public:
		Quantity(DatT dt, DimT L = 0, DimT M = 0, DimT T = 0, DimT I = 0, DimT t = 0, DimT n = 0, DimT J = 0)
			:data(dt), super(L, M, T, I, t, n, J) { }
		~Quantity() { }
	private:
		DatT data;
	};

	Quantity<unsigned long long> operator""m(unsigned long long length)noexcept {
		return Quantity(length, 1);
	}
	Quantity<unsigned long long> operator""kg(unsigned long long mass)noexcept {
		return Quantity(mass, 0, 1);
	}
	Quantity<unsigned long long> operator""s(unsigned long long time)noexcept {
		return Quantity(time, 0, 0, 1);
	}
	Quantity<unsigned long long> operator""A(unsigned long long current)noexcept {
		return Quantity(current, 0, 0, 0, 1);
	}
	Quantity<unsigned long long> operator""K(unsigned long long temperature)noexcept {
		return Quantity(temperature, 0, 0, 0, 0, 1);
	}
	Quantity<unsigned long long> operator""mol(unsigned long long Mol)noexcept {
		return Quantity(Mol, 0, 0, 0, 0, 0, 1);
	}
}