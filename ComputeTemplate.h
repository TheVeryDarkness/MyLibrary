#pragma once

#include <cstdlib>
#include <iterator>

template<typename Data, Data _MaxOfOneUnit, Data _MinOfOneUnit = 0>
class compute_traits
{
public:
	compute_traits() = delete;
	~compute_traits() = delete;
	using OverFlow=size_t;
	static OverFlow GetFlow(std::initializer_list<Data> list) {
		Data i0 = 0;
		for (auto const& i:list)
		{
			if constexpr(_MaxOfOneUnit>0)
			{
				if (i > _MaxOfOneUnit - i0)
				{
					i0 -= (_MaxOfOneUnit - i + 1);
				}
			}
			if constexpr (_MinOfOneUnit<0)
			{
				if (i < _MinOfOneUnit - i0)
				{
					i0 += (i - _MinOfOneUnit - 1);
				}
			}
		}
	}
	constexpr inline Data MaxOfOneUnit() { return _MaxOfOneUnit; }
private:

};

template<typename Linear, typename Data, Data Max,class _Traits>
inline Linear AddTo(const Linear& a, Linear& b) {
	for (std::iterator<std::forward_iterator_tag, Data>& i : b)
	{
		
	}
}