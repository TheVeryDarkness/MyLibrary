#pragma once

#include "mylog.h"
#include "VisualStudioDebug.h"
#include <thread>
#include <condition_variable>
#include <mutex>
#include <tuple>

inline namespace Darkness {
	//constexpr auto wake_up_interval = std::chrono::nanoseconds(1);

	template<size_t parallelSize>
	class parallel {
	public:
		template<typename T, typename... Para>
		class taskAssembly {
		private:
			static_assert(std::is_invocable_v<T, Para...>, "Invalid argument.");
			static_assert(std::is_invocable_r_v<void, T, Para...>, "We don't support return value.");
			volatile bool no_more_data = false;
			bool data_freshed[parallelSize] = { };
			std::thread pool[parallelSize];
			std::tuple<Para...> *data[parallelSize] = {};
			std::mutex locked_if_busy[parallelSize];
			std::condition_variable wait_for_data[parallelSize];
			std::mutex locked_if_being_used;
			std::condition_variable wait_for_thread;

			bool available() const noexcept {
				for (const auto &b : data_freshed) if (!b)	return true;
				return false;
			}
			size_t find() const noexcept {
				size_t i = 0;
				for (; i < parallelSize; ++i) {
					if (!data_freshed[i]) {
						return i;
					}
				}
				assert(i < parallelSize);
				return find();
			}
		public:
			taskAssembly() = default;
			~taskAssembly()noexcept {
				this->ending();
				{
					std::unique_lock ul(locked_if_being_used);
				WakeUp:
					for (auto &w : wait_for_data) {
						w.notify_all();
					}
					if (!empty()) {
						this->wait_for_thread.wait(ul);
						goto WakeUp;
					}
				}
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
					assert(index < parallelSize);
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
						bool *freshed,
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
							std::unique_lock ul(locked_if_being_used);
							wait_for_thread.notify_all();
						#ifdef _LOG
							om.lock();
							mlog << "Thread " << index << " is destroyed. It's id is " << pool[index].get_id() << std::endl;
							om.unlock();
						#endif // _LOG
						}, &(data_freshed[index]), &(locked_if_busy[index]), &(wait_for_data[index]), &data[index]);
				#ifdef _LOG
					om.lock();
					mlog << "Thread " << index << " is created. It's id is " << pool[index].get_id() << std::endl;
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
			//Return true if no thread is working.
			bool empty()const noexcept {
				for (const auto &b : data_freshed) if (b)	return false;
				return true;
			}
			const std::thread &operator[](size_t index)const noexcept {
				return pool[index % parallelSize];
			}
		};
	};
}