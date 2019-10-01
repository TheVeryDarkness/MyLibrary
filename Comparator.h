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
		~Pack() { }
		decltype(data) operator()()const noexcept {
			return this->data;
		}
		constexpr friend auto operator<(const Pack &a, const Pack &b)noexcept {
			return Comparator<T, CmprT::smaller>(a, b);
		}
		constexpr friend auto operator>(const Pack &a, const Pack &b)noexcept {
			return Comparator<T, CmprT::larger>(a, b);
		}
		constexpr friend auto operator>=(const Pack &a, const Pack &b)noexcept {
			return Comparator<T, CmprT::notSmaller>(a, b);
		}
		constexpr friend auto operator<=(const Pack &a, const Pack &b)noexcept {
			return Comparator<T, CmprT::notLarger>(a, b);
		}
	};

	template<typename T, CmprT comp>
	class Comparator {
	private:
		const Pack<T> &a, &b;
		bool Val;
	public:
		Comparator(const Pack<T> &a, const Pack<T> &b, bool Val = true)noexcept :a(a), b(b), Val(Val) { }
		~Comparator() { }
		operator bool() const noexcept {
			return Val;
		}
		friend Comparator operator <(const Comparator &This,const Pack<T> &That) noexcept {
			if (!This.Val) {
				return Comparator(This.a, That, false);
			}
			return Comparator(This.a, That, (This.a() < This.b() && This.b() < That()));
		}
	};
}