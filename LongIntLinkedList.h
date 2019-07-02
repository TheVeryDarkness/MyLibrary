#pragma once

#include "LinkedList.h"
#include "CODNT.h"

namespace LL {
	//长整型数（long）单向（oneway）链表（linked list）
	typedef OLL<long> LOLL;
	//坐标（coordinate）单向（oneway）链表（linked list）
	typedef OLL<CODNT>COLL;
}

//定义区