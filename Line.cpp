#pragma once
// Line.cpp : 定义静态库的函数。
//

#include "Line.h"
#include <ctime>

Line Line::operator=(const Line& that) {
	this->A = that.A;
	this->B = that.B;
	this->C = that.C;
	return *this;
}

//获得直线参数
extern inline Line __stdcall GetLine(
	CODNT Start,
	CODNT End
) {
	Line line;//直线参数（直线的一般式）
	if (Start.x == End.x && Start.y == End.y) {//所给两点重合
#ifdef _DEBUG
		throw;
#else
		srand((unsigned int)time(0));
		line.A = rand();
		line.B = rand();
		line.C = -line.A * Start.x - line.B * Start.y;
#endif // _DEBUG
	}
	else if (Start.x == End.x && Start.y != End.y) {//平行于y轴的直线
		line.B = 0;
		line.A = 1L;
		line.C = -Start.x;
	}
	else if (Start.x != End.x && Start.y == End.y) {//平行于x轴的直线
		line.A = 0;
		line.B = 1L;
		line.C = -Start.y;
	}
	else if (Start.x != End.x && Start.y != End.y) {//一般位置曲线
		line.A = End.y - Start.y;
		line.B = -(End.x - Start.x);
		line.C = -End.x * line.A - End.y * line.B;
		//C=End.y*(End.x - Start.x)-End.x*(End.y - Start.y);
		//C = End.x*Start.y - End.y*Start.x;
	}
	return line;
}