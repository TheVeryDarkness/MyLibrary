#pragma once

template<typename Ptr> inline void PtrDelete(Ptr ptr) { if (ptr != nullptr)delete ptr; }