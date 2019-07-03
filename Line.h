#pragma once

#include "CODNT.h"

//直线参数（一般式）
typedef struct Line {
	long A, B, C;
	struct Line operator=(const struct Line& that);
} Line;
inline Line __stdcall GetLine(
	CODNT, CODNT
);