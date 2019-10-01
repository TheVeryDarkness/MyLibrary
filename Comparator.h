#pragma once

namespace Math {
	enum class CmprT :unsigned char;
	template<typename T> class Pack;
	template<typename T, CmprT comp> class Comparator;


	enum class CmprT :unsigned char {
		smaller, larger, notSmaller, notLarger
	};
	template<typename T>
	class Pack {
	private:
		const T &data;
	public:
		explicit Pack(const T &data)noexcept :data(data) { }
		~Pack() = default;
		decltype(data) operator()()const noexcept {
			return this->data;
		}
		friend auto operator<(const Pack &a, const Pack &b)noexcept {
			return Comparator<T, CmprT::smaller>(a, b);
		}
		friend auto operator>(const Pack &a, const Pack &b)noexcept {
			return Comparator<T, CmprT::larger>(a, b);
		}
		friend auto operator>=(const Pack &a, const Pack &b)noexcept {
			return Comparator<T, CmprT::notSmaller>(a, b);
		}
		friend auto operator<=(const Pack &a, const Pack &b)noexcept {
			return Comparator<T, CmprT::notLarger>(a, b);
		}
	};

	template<typename T, CmprT comp>
	class Comparator {
	private:
		const Pack<T> &a, &b;
		bool Val;
	protected:
		constexpr static bool cmpr(const Pack<T> &a, const Pack<T> &b)noexcept {
			return cmpr(a(), b());
		}
		constexpr static bool cmpr(const T &a, const T &b)noexcept {
			if constexpr (comp == CmprT::smaller) {
				return a < b;
			}if constexpr (comp == CmprT::larger) {
				return a > b;
			}if constexpr (comp == CmprT::notSmaller) {
				return a >= b;
			}if constexpr (comp == CmprT::notLarger) {
				return a <= b;
			}
		}
	public:
		Comparator(const Pack<T> &a, const Pack<T> &b, bool Val)noexcept
			:a(a), b(b), Val(Val) { }
		Comparator(const Pack<T> &a, const Pack<T> &b)noexcept
			:a(a), b(b), Val(cmpr(a, b)) { }
		~Comparator() = default;
		operator bool() const noexcept {
			return Val;
		}
		friend Comparator operator<(const Comparator &This, const Pack<T> &That) noexcept {
			if (!This.Val)
				return Comparator(This.a, That, false);
			return Comparator(This.a, That, (This.Val && This.b() < That()));
		}
		friend Comparator operator>(const Comparator &This, const Pack<T> &That) noexcept {
			if (!This.Val)
				return Comparator(This.a, That, false);
			return Comparator(This.a, That, (This.Val && This.b() > That()));
		}
		friend Comparator operator>=(const Comparator &This, const Pack<T> &That) noexcept {
			if (!This.Val)
				return Comparator(This.a, That, false);
			return Comparator(This.a, That, (This.Val && This.b() >= That()));
		}
		friend Comparator operator<=(const Comparator &This, const Pack<T> &That) noexcept {
			if (!This.Val)
				return Comparator(This.a, That, false);
			return Comparator(This.a, That, (This.Val && This.b() <= That()));
		}
	};
}