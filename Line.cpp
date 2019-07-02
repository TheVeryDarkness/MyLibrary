#pragma once
// Line.cpp : ���徲̬��ĺ�����
//

#include "Line.h"
#include <ctime>

Line Line::operator=(const Line& that) {
	this->A = that.A;
	this->B = that.B;
	this->C = that.C;
	return *this;
}

//���ֱ�߲���
extern inline Line __stdcall GetLine(
	CODNT Start,
	CODNT End
) {
	Line line;//ֱ�߲�����ֱ�ߵ�һ��ʽ��
	if (Start.x == End.x && Start.y == End.y) {//���������غ�
#ifdef _DEBUG
		throw;
#else
		srand((unsigned int)time(0));
		line.A = rand();
		line.B = rand();
		line.C = -line.A * Start.x - line.B * Start.y;
#endif // _DEBUG
	}
	else if (Start.x == End.x && Start.y != End.y) {//ƽ����y���ֱ��
		line.B = 0;
		line.A = 1L;
		line.C = -Start.x;
	}
	else if (Start.x != End.x && Start.y == End.y) {//ƽ����x���ֱ��
		line.A = 0;
		line.B = 1L;
		line.C = -Start.y;
	}
	else if (Start.x != End.x && Start.y != End.y) {//һ��λ������
		line.A = End.y - Start.y;
		line.B = -(End.x - Start.x);
		line.C = -End.x * line.A - End.y * line.B;
		//C=End.y*(End.x - Start.x)-End.x*(End.y - Start.y);
		//C = End.x*Start.y - End.y*Start.x;
	}
	return line;
}