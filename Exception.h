#pragma once

#include <stdexcept>
#include <exception>


class RadixError:public std::exception
{
public:
	__stdcall RadixError(const char*);
	__stdcall ~RadixError();
	const char* what()const;
private:
	const char* Message;
};
