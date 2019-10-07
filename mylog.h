#pragma once

#include <fstream>
#include <mutex>

class safe_ofstream {
public:
	safe_ofstream(const char *path) :f(path,std::ios::out) { assert(f); }

	~safe_ofstream() { f << std::endl; f.close(); }
	operator std::ostream &()noexcept {
		return f;
	}
	template<typename All>safe_ofstream &operator<<(All a)noexcept {
		f << a;
		return *this;
	}
	std::ofstream &operator<<(const char* a)noexcept {
		f << a;
		return f;
	}
	std::ofstream &operator<<(void* a)noexcept {
		f << a;
		return f;
	}
private:
	std::ofstream f;
};


static inline safe_ofstream mlog("D:\\out.out");
std::mutex om;
#define mlog std::cout

#ifdef _DEBUG
#define _LOG
#endif // _DEBUG
