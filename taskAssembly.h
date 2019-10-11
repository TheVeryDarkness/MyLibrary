#pragma once

#include "mylog.h"
#include "VisualStudioDebug.h"
#include <thread>
#include <condition_variable>
#include <mutex>
#include <tuple>

namespace Darkness {
	constexpr auto wake_up_interval = std::chrono::nanoseconds(1);
	template<size_t poolSize, typename T, typename... Para>
	class taskAssembly {
	private:
		static_assert(std::is_invocable_v<T, Para...>, "Invalid argument.");
		volatile bool no_more_data = false;
		bool data_freshed[poolSize] = { };
		std::thread pool[poolSize];
		std::tuple<Para...>* data[poolSize] = {};
		std::mutex locked_if_busy[poolSize];
		std::condition_variable wait_for_data[poolSize];
		std::mutex locked_if_being_used;
		std::condition_variable wait_for_thread;

		bool available() const noexcept {
			for (const auto &b : data_freshed) if (!b)	return true;
			return false;
		}
		size_t find() const noexcept {
			size_t i = 0;
			for (; i < poolSize; ++i) {
				if (!data_freshed[i]) {
					return i;
				}
			}
			assert(i < poolSize);
			return find();
		}
	public:
		taskAssembly() = default;
		~taskAssembly()noexcept {
			this->ending();
			do {
				for (auto &w : wait_for_data) {
					w.notify_all();
					this->wait();
				}
			}while (!empty());
			for (auto &t : pool) {
				if (t.joinable())
					t.join();
			}
			for (auto &d : data) {
				if (d != nullptr) {
					delete d;
					d = nullptr;
				}
			}
		}
		size_t pop(Para... para)noexcept {
			using namespace std::literals::chrono_literals;
			size_t index;
			{
				std::unique_lock ul(locked_if_being_used);
				while (!available()) wait_for_thread.wait(ul);
				index = find();
				assert(index < poolSize);
			}
			{
				std::unique_lock ul(locked_if_busy[index]);
				if (this->data[index] != nullptr)
					delete this->data[index];
				this->data[index] = new std::tuple(para...);
				assert((data_freshed[index]) == false);
				data_freshed[index] = true;
			}
			if (!pool[index].joinable()) {
				pool[index] = std::thread([this, index](
					bool* freshed,
					std::mutex *const locked_busy,
					std::condition_variable *const wait_data,
					std::tuple<Para...> **priv_data
					) {
						T t;
						while ((*freshed) || !this->no_more_data) {
							std::unique_lock ul(*locked_busy);
							while (!(*freshed) && !this->no_more_data) {
								wait_data->wait(ul);
							}
							assert(*freshed == true || this->no_more_data);
							if (*freshed) {
								assert((*freshed) == true);
								std::apply(t, **priv_data);
								*freshed = false;
							}
							wait_for_thread.notify_one();
						}
					#ifdef _LOG
						om.lock();
						mlog << "Thread " << index << " is destroyed. It's id is " << pool[index].get_id() << std::endl;
						om.unlock();
					#endif // _LOG
					}, &(data_freshed[index]), &(locked_if_busy[index]), &(wait_for_data[index]), &data[index]);
			#ifdef _LOG
				om.lock();
				mlog << "Thread " << index << " is created. It's id is "<<pool[index].get_id() << std::endl;
				om.unlock();
			#endif // _LOG
			}
			else {
				wait_for_data[index].notify_one();
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
			for (const auto &b : data_freshed) if (b)	return false;
			return true;
		}
		const std::thread &operator[](size_t index)noexcept {
			return pool[index];
		}
	};
}