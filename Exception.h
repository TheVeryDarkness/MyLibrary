#pragma once

#include <stdexcept>
#include <exception>
#include <Windows.h>


class RadixError:public std::exception
{
public:
	__stdcall RadixError(const char*);
	__stdcall ~RadixError();
	const char* what()const;
private:
	const char* Message;
};

inline std::string HrToString(HRESULT hr)
{
	char s_str[64] = {};
	sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
	return std::string(s_str);
}

class GraphicsException :public std::runtime_error
{
public:
	GraphicsException(const char* msg):std::runtime_error(msg) {}
};

class HrException : public std::runtime_error
{
public:
	HrException(HRESULT hr) : std::runtime_error(HrToString(hr)), m_hr(hr) {}
	HRESULT Error() const { return m_hr; }
private:
	const HRESULT m_hr;
};