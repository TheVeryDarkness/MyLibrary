#pragma once

#include <stdexcept>
#include <exception>
#pragma warning(disable:4996)
namespace Darkness {
	template<size_t sz>
	class String :std::exception {
	public:
		String(const char *s) noexcept {
			strcpy(str, s);
		}
		~String() = default;
		operator char *()noexcept { return str; }
		const char *what()const noexcept override {
			return str;
		}
	private:
		char str[sz];
	};
}