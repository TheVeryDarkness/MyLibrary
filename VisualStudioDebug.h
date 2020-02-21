#pragma once

#ifndef NOMINMAX
#define NOMINMAX 1
#endif // !NOMINMAX

#ifdef _DEBUG

#ifndef NOMINMAX
#define NOMINMAX 1
#endif // NOMINMAX
#ifndef DBG_NEW
#if 1
#define DBG_NEW ::new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif //1
#endif  //DBG_NEW
#else
#ifndef DBG_NEW
#define DBG_NEW new
#endif // !DBG_NEW
#endif  // _DEBUG