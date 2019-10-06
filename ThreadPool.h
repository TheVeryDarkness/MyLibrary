#pragma once

#include <thread>
#include <condition_variable>

class threadPool {
public:
	static inline constexpr size_t poolSize = 8;
	threadPool() = delete;
	~threadPool() = delete;
	static std::thread &pop()noexcept {
		locked_if_being_used.lock();
		wait_for_thread.wait(locked_if_empty);
		if (flag >0) {
			--flag;
			if (flag == 0) {
				locked_if_empty.lock();
			}
			return pool[flag];
		}
		locked_if_being_used.unlock();
	}
	static std::thread &push()noexcept {
		locked_if_being_used.lock();
		wait_for_thread.notify_one();
		locked_if_being_used.unlock();
	}
private:
	static inline size_t flag;
	static inline std::thread pool[poolSize];
	static inline std::mutex locked_if_being_used;
	static inline std::mutex locked_if_empty;
	static inline std::condition_variable_any wait_for_thread;
};