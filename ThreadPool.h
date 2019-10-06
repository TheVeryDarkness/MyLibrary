#pragma once

#include <thread>
#include <condition_variable>
#include <mutex>

namespace Darkness {
	template<size_t poolSize>
	class threadPool {
	public:
		class Task {
		public:
			Task(size_t index) :index_in_pool(index) { }
			~Task() {
				pool.push(index_in_pool);
			}

		private:
			threadPool<poolSize> &pool;
			size_t index_in_pool;
		};

		threadPool() = default;
		~threadPool() = default;
		template<typename T, typename... Para>
		size_t pop(Para... para)noexcept {
			static_assert(
				std::is_base_of_v<Task, T>, 
				"To use the pool safely, you must let your class deprived from my class."
				);
			std::unique_lock ul(locked_if_being_used);
			while (!available()) wait_for_thread.wait(ul);
			auto &&index = find();
			occupied[index] = true;
			T task(index, para...);

			pool[index] = std::thread(task);
			return index;
		}
		void wait()noexcept {
			std::unique_lock ul(locked_if_being_used);
			wait_for_thread.wait(ul);
		}
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
		void push(size_t that)noexcept {
			std::unique_lock ul(locked_if_being_used);
			pool[that].join();
			occupied[that] = false;
			wait_for_thread.notify_one();
		}
	};
}