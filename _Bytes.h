#pragma once
#include "Shared.h"


#pragma pack(push)
#pragma pack(1)

constexpr std::pair<size_t, size_t> MY_LIBRARY IntelligentLength(size_t sz)noexcept {
	if (sz >= 8)
	{
		return std::pair<size_t, size_t>(sz - 8, 8);
	}
	else if (sz >= 4)
	{
		return std::pair<size_t, size_t>(sz - 4, 4);
	}
	else if (sz >= 2)
	{
		return std::pair<size_t, size_t>(sz - 2, 2);
	}
	else if (sz >= 1)
	{
		return std::pair<size_t, size_t>(sz - 1, 1);
	}
}

template<size_t Length>
class _Bytes;

template<size_t Length>
class _Int;

template<>class _Int<1> {
public:
	using type=unsigned __int8;
};
template<>class _Int<2> {
public:
	using type=unsigned __int16;
};
template<>class _Int<4> {
public:
	using type=unsigned __int32;
};
template<>class _Int<8> {
public:
	using type=unsigned __int64;
};
template<>
class _Bytes<0> {
public:
	MY_LIBRARY _Bytes() = default;
	MY_LIBRARY ~_Bytes() = default;
	static constexpr size_t getAccount()noexcept { return 0; }
};
template<size_t Length>
class _Bytes :public _Bytes<IntelligentLength(Length).first>
{
public:
	template<typename Val, typename... Vals>
	MY_LIBRARY _Bytes(Val val, Vals... vals)noexcept
		: Byte(val), _Bytes<IntelligentLength(Length).first>(vals...) {} //= default;
	MY_LIBRARY ~_Bytes() = default;
	static constexpr size_t getAccount()noexcept { return (_Bytes<IntelligentLength(Length).first>::getAccount() + 1); }

private:
	typename _Int<IntelligentLength(Length).second>::type Byte;
};
#pragma pack(pop)