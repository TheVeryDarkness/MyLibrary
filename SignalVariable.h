#pragma once
#include <mutex>

namespace Darkness {
	class Empty {
	public:
		Empty(){ }
		~Empty(){ }
		constexpr bool operator()()const noexcept { return true; }
	private:

	};
	template<typename T>
	class Signal {
	public:
		Signal(const T &data) :data(data) { }
		~Signal() { changed_signal.notify_all(); }
		operator T()noexcept {
			std::unique_lock ul(locked_if_used);
			T copy = data;
			return copy;
		}
		void operator++()noexcept {
			{
				std::unique_lock ul(locked_if_used);
				++data;
			}
			changed_signal.notify_one();
			return ;
		}
		void operator--()noexcept {
			{
				std::unique_lock ul(locked_if_used);
				--data;
			}
			changed_signal.notify_one();
			return ;
		}
		void operator=(T d)noexcept {
			std::unique_lock ul(locked_if_used);
			this->data = d;
		}
		template<typename Func> static auto both(Signal& a,Signal& b)noexcept {
			std::unique_lock ula(a.locked_if_used);
			std::unique_lock ulb(b.locked_if_used);
			Func func;
			auto&& res= func(a.data, b.data);
			return res;
		}
		template<typename _Predicate = Empty>void wait(_Predicate && p = Empty())noexcept {
			std::unique_lock ul(locked_if_used);
			changed_signal.wait(ul, p);
		}
	private:
		T data;
		std::mutex locked_if_used;
		std::condition_variable changed_signal;
	};
}