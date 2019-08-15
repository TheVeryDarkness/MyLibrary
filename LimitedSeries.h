#pragma once


#include "DefaultFunction.h"
#include "PreciseMath.h"
#include <forward_list>

//caculation must be given
//series with limited items
class Series
{
private:
	std::forward_list<LL::Function*> series;
public:
	Series(const std::initializer_list<LL::Function*>& list) :series(list) {}
	~Series() {
		for (auto& i : series)
		{
			delete i;
		}
	}
	Series& __stdcall diff() {
		for (auto& i : series) {
			i->diff(i);
		}
		return *this;
	}
	Series& __stdcall integral() {
		for (auto& i : series) {
			i->integral(i);
		}
		return *this;
	}
	//不再自动换行
	friend std::ostream& __stdcall operator<<(std::ostream&, const Series&);
};

std::ostream& __stdcall operator<<(std::ostream& o, const Series& s) {
	size_t sz = 0;
	for (auto const& i : s.series)
	{
		sz++;
	}
	for (auto const& i : s.series)
	{
		i->Print(o);
		if ((--sz) > 0)
		{
			o << " + ";
		}
	}
	return o;
}
