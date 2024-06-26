#pragma once
#include <mutex>
#include <atomic>
#include <condition_variable>

inline namespace Darkness {
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
		~Signal() {
			std::unique_lock ul(locked_if_used);
		}
		[[deprecated]]operator T()noexcept {
			std::unique_lock ul(locked_if_used);
			T copy = data;
			return copy;
		}
		void operator++()noexcept {
			{
				std::unique_lock ul(locked_if_used);
				++data;
			}
			changed_signal.notify_all();
			return ;
		}
		void operator--()noexcept {
			{
				std::unique_lock ul(locked_if_used);
				--data;
			}
			changed_signal.notify_all();
			return ;
		}
		void operator=(T d)noexcept {
			std::unique_lock ul(locked_if_used);
			this->data = d;
		}
		template<typename Func>[[deprecated]] static auto both(Signal& a,Signal& b)noexcept {
			using namespace std::literals::chrono_literals;
			while (true) {
				bool _a = a.locked_if_used.try_lock();
				bool _b = b.locked_if_used.try_lock();
				if (_a && _b) break;
				if (_a) a.locked_if_used.unlock();
				if (_b) b.locked_if_used.unlock();
				if (!_a && !_b) { std::this_thread::sleep_for(1ns); }
				std::this_thread::sleep_for(1ns);
			}
			Func func;
			auto&& res= func(a.data, b.data);
			a.locked_if_used.unlock();
			b.locked_if_used.unlock();
			return res;
		}
		void wait_for_value(T v)noexcept {
			using namespace std::literals::chrono_literals;
			std::unique_lock ul(locked_if_used);
			changed_signal.wait(ul, [&] {
				return (this->data == v);
				});
		}
		void wait_for_more_than(T v)noexcept {
			using namespace std::literals::chrono_literals;
			std::unique_lock ul(locked_if_used);
			changed_signal.wait(ul, [&] {
				return (this->data > v);
				});
		}
	private:
		std::atomic<T> data;
		std::mutex locked_if_used;
		std::condition_variable changed_signal;
	};
}