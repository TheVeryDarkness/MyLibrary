#pragma once

#include <Windows.h>

#ifndef DEBUG_OUT
#define DEBUG_OUT OutputDebugStringA((std::string(__FILE__) + " " + std::to_string(__LINE__) + "\n").c_str())
#endif // !DEBUG_OUT