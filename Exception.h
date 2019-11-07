#pragma once

#include <stdexcept>
#include <exception>


namespace Darkness {
	class RadixError :public std::exception {
	public:
		__stdcall RadixError(const char *);
		__stdcall ~RadixError();
		const char *what()const;
	private:
		const char *Message;
	};
	inline namespace Function {
		class UnknownFunction :public std::exception {
		public:
			UnknownFunction(const char *msg, const std::string &name)
				:std::exception(msg), functionName(name) { }
			std::string what() {
				return exception::what() + functionName;
			}
		private:
			std::string functionName;
		};
	}
}