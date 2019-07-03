#pragma once

#include <ostream>
//Coordination
//二维直角坐标
class CODNT
{
public:
	static long xOrigin, yOrigin;//原点
	long x;
	long y;
	__stdcall CODNT(long, long);
	static const long& __stdcall GetX(const CODNT&);
	static const long& __stdcall GetY(const CODNT&);
	std::ostream& __stdcall operator<<(std::ostream&);
	//坐标转换
	CODNT __stdcall Translate(CODNT);
	//坐标转换
	CODNT __stdcall Translate(long, long);
};