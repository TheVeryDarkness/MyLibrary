#include "Exception.h"

__stdcall RadixError::RadixError(const char* Message)
	:Message(Message)
{
}

__stdcall RadixError::~RadixError()
{
}

inline const char* RadixError::what()const {
	return this->Message;
}