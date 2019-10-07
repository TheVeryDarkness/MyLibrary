#pragma once

#include "mylog.h"
#include <thread>
#include <condition_variable>
#include <mutex>

namespace Darkness {

	template<typename T, size_t poolSize, typename... Para>
	class taskAssembly {
	private:
		bool no_more_data = false;
		bool busy[poolSize] = {};
		std::thread pool[poolSize];
		std::tuple<Para...> data[poolSize];
		std::mutex locked_if_busy[poolSize];
		std::condition_variable wait_for_data[poolSize];
		std::mutex locked_if_being_used;
		std::condition_variable wait_for_thread;

		bool available() const noexcept {
			for (const auto &b : busy) if (!b)	return true;
			return false;
		}
		size_t find() const noexcept {
			size_t i = 0;
			for (; i < poolSize; ++i) {
				if (!busy[i]) {
					return i;
				}
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
			assert(that < poolSize);
			busy[that] = false;
			wait_for_thread.notify_one();
		}
	public:
		static_assert(
			std::is_base_of_v<Task, T>,
			"To use the pool safely, you must let your class deprived from my class."
			);

		taskAssembly() = default;
		~taskAssembly()noexcept{ 
			for (auto &t : pool) {
				if (t.joinable())t.join();
			}
		}
		size_t pop(Para... para)noexcept {
			std::unique_lock ul(locked_if_being_used);
			while (!available()) wait_for_thread.wait(ul);
			auto index = find();
			busy[index] = true;
			if (pool[index].joinable()) {
				pool[index].join();
				pool[index] = std::thread([=]() {
					std::unique_lock ul(locked_if_being_used);
					while (!no_more_data) {
						T t(*this, index, para...);
						t();
					}
					});
			}
			else {

			}
			return index;
		}
		void ending()noexcept {
			std::unique_lock ul(locked_if_being_used);
			this->no_more_data = true;
		}
		void wait()noexcept {
			std::unique_lock ul(locked_if_being_used);
			wait_for_thread.wait(ul);
		}
		//Return true if no thread is working.
		bool empty()const noexcept {
			for (const auto &b : busy) if (b)	return false;
			return true;
		}
		const std::thread &operator[](size_t index)noexcept {
			return pool[index];
		}
		class Task {
		public:
			Task(taskAssembly &p, size_t index)
				:pool(p), index_in_pool(index) {
				pool.locked_if_busy[index_in_pool].lock();
			}
			~Task() {
				pool.locked_if_busy[index_in_pool].unlock();
				pool.push(index_in_pool);
			}
			Task(const Task &that) = delete;
			Task(Task &&that) = delete;
		private:
			taskAssembly &pool;
			size_t index_in_pool;
		};
	};
}