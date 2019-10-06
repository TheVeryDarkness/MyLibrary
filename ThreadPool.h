#pragma once

#include <thread>
#include <condition_variable>
#include <mutex>

template<size_t poolSize>
class threadPool {
private:
	bool occupied[poolSize] = {};
	std::thread pool[poolSize];
	std::mutex locked_if_being_used;
	std::condition_variable wait_for_thread;
	bool available() const noexcept {
		for (const auto &b : occupied) if (!b)	return true;
		return false;
	}
	bool empty()const noexcept {
		for (const auto &b : occupied) if (b)	return false;
		return true;
	}
	size_t find() const noexcept  {
		size_t i = 0;
		for (; i < poolSize; ++i) {
			if (!occupied[i])	return i;
		}
		assert(i < poolSize);
		return size_t(-1);
	}
	size_t match(std::thread& that)const noexcept  {
		size_t i = 0;
		for (; i < poolSize; ++i) {
			if (pool[i].get_id() == that.get_id())	return i;
		}
		assert(i < poolSize);
		return size_t(-1);
	}
public:
	threadPool() = default;
	~threadPool() = default;
	std::thread &pop()noexcept {
		std::unique_lock ul(locked_if_being_used);
		while (!available()) {
			wait_for_thread.wait(ul);
		}
		auto &&index = find();
		occupied[index] = true;
		return pool[index];
	}
	void push(std::thread& that)noexcept {
		std::unique_lock ul(locked_if_being_used);
		occupied[match(that)] = false;
		wait_for_thread.notify_one();
	}
	const auto &getCV()noexcept {
		return wait_for_thread;
	}
};