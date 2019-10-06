#pragma once
#include <mutex>

namespace LargeInteger {
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
		template<typename _Predicate>void wait(_Predicate&& p)noexcept {
			std::unique_lock ul(locked_if_used);
			changed_signal.wait(ul, p);
		}
	private:
		T data;
		std::mutex locked_if_used;
		std::condition_variable changed_signal;
	};
}