// CODNT.cpp : ���徲̬��ĺ�����
//

#include "CODNT.h"

inline __stdcall CODNT::CODNT(long x=0, long y=0):x(x),y(y) {}

inline const long& __stdcall CODNT::GetX(const CODNT& that) {
	return that.x;
}
inline const long& __stdcall CODNT::GetY(const CODNT& that) {
	return that.y;
}

inline std::ostream& __stdcall CODNT::operator<<(
	std::ostream& out
	) {
	out << this->x << " " << this->y;
	return out;
}

inline CODNT __stdcall CODNT::Translate(
	CODNT Client//���û��� ��Ļ�ϵ�����
) {
	CODNT target(Client.x, Client.y);
	return target;
}

inline CODNT __stdcall CODNT::Translate(long, long) {
	CODNT result(x, y);
	return Translate(result);
}