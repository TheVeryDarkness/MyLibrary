#pragma once

template<typename Ptr> inline void PtrDelete(Ptr ptr) { if (ptr != nullptr)delete ptr; }


#define DEBUG_OUT OutputDebugStringA((std::string(__FILE__) + " " + std::to_string(__LINE__) + "\n").c_str())