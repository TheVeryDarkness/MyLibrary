#pragma once



#include <forward_list>
#include <PreciseMath.h>

//caculation must be given
template<class func>
//series with limited items
class Series
{
private:
	std::forward_list<func> series;
public:
	Series(const std::initializer_list<func>& list) :series(list) {}
	~Series() {
		for (auto& i : series)
		{
			i.destruct();
		}
	}
	Series& __stdcall diff() {
		for (auto& i : series) {
			i.diff();
		}
		return *this;
	}
	Series& __stdcall integral() {
		for (auto& i : series) {
			i.integral();
		}
		return *this;
	}
	//不再自动换行
	template<class func>
	friend std::ostream& __stdcall operator<<(std::ostream&, const Series<func>&);
};
template<class func>
std::ostream& __stdcall operator<<(std::ostream& o, const Series<func>& s) {
	size_t sz = 0;
	for (auto const& i : s.series)
	{
		sz++;
	}
	for (auto const& i : s.series)
	{
		o << i;
		if ((--sz) > 0)
		{
			o << " + ";
		}
	}
	return o;
}
