#pragma once

#include <ostream>
//Coordination
//��άֱ������
class CODNT
{
public:
	static long xOrigin, yOrigin;//ԭ��
	long x;
	long y;
	__stdcall CODNT(long, long);
	static const long& __stdcall GetX(const CODNT&);
	static const long& __stdcall GetY(const CODNT&);
	std::ostream& __stdcall operator<<(std::ostream&);
	//����ת��
	CODNT __stdcall Translate(CODNT);
	//����ת��
	CODNT __stdcall Translate(long, long);
};