#pragma once

#include <thread>
#include <condition_variable>
#include <mutex>

namespace Darkness {


	template<typename Data, size_t poolSize>
	class threadPool {
	public:
		threadPool() = default;
		~threadPool() = default;
		template<typename T>
		void pop(Task<Data, poolSize> task)noexcept {
			std::unique_lock ul(locked_if_being_used);
			while (!available()) wait_for_thread.wait(ul);
			auto &&index = find();
			occupied[index] = true;

			pool[index] = std::thread(task);
		}
		void push(size_t that)noexcept {
			std::unique_lock ul(locked_if_being_used);
			occupied[that] = false;
			wait_for_thread.notify_one();
		}
		void wait()noexcept {
			std::unique_lock ul(locked_if_being_used);
			wait_for_thread.wait(ul);
		}
	private:
		bool occupied[poolSize] = {};
		std::thread pool[poolSize];
		Data data[poolSize];
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
		size_t find() const noexcept {
			size_t i = 0;
			for (; i < poolSize; ++i) {
				if (!occupied[i])	return i;
			}
			assert(i < poolSize);
			return size_t(-1);
		}
		size_t match(std::thread &that)const noexcept {
			size_t i = 0;
			for (; i < poolSize; ++i) {
				if (pool[i].get_id() == that.get_id())	return i;
			}
			assert(i < poolSize);
			return size_t(-1);
		}
	};
	template<typename Data, size_t poolSize>
	class Task {
	public:
		template<typename... para>Task(size_t index, para...pa) :index_in_pool(index), t(pa) { }
		~Task() { 
			pool.push(index_in_pool);
		}
		auto operator()()noexcept {
			return t();
		}

	private:
		threadPool<Data, poolSize> &pool;
		size_t index_in_pool;
		const Data &data;
	};
}