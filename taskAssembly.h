#pragma once

#include "mylog.h"
#include <thread>
#include <condition_variable>
#include <mutex>
#include <tuple>

namespace Darkness {
	template<size_t poolSize, typename... Para>
	class taskAssembly {
	private:
		bool no_more_data = false;
		bool busy[poolSize] = {};
		std::thread pool[poolSize];
		std::tuple<Para...> *data[poolSize] = {};
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
		taskAssembly() = default;
		~taskAssembly()noexcept {
			for (auto &t : pool) {
				if (t.joinable())t.join();
			}
		}
		template<typename T>size_t pop(Para... para)noexcept {
			std::unique_lock ul(locked_if_being_used);
			while (!available()) wait_for_thread.wait(ul);
			auto index = find();
			busy[index] = true;
			this->data = std::tuple(para...);
			if (pool[index].joinable()) {
				pool[index].join();
				pool[index] = std::thread([&]() {
					pool.locked_if_busy[index].lock();

					std::unique_lock ul(locked_if_being_used);
					while (!no_more_data) {
						T t(data[index]);
						t();
						wait_for_data[index].wait(ul);
					}

					pool.locked_if_busy[index].unlock();
					pool.push(index);
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
	};
}