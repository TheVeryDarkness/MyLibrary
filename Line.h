#pragma once

#include "CODNT.h"

//ֱ�߲�����һ��ʽ��
typedef struct Line {
	long A, B, C;
	struct Line operator=(const struct Line& that);
} Line;
inline Line __stdcall GetLine(
	CODNT, CODNT
);