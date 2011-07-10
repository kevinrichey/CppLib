#ifndef KWR_RANGE_H
#define KWR_RANGE_H

#include <cstdlib>

namespace kwr
{
	template<typename T>
	class Range
	{
		T first, last;

		public:
		Range(T min, T max) : first(min), last(max) {}

		//public: int Choose() const
		//{
			//return Choose(rand);
		//}

		//template<int RandomInt()>
		//template<typename Selector>
		//int Choose(Selector S) const
		//{
			//return S() % Length() + first;
		//}

		int Length() const
		{
			return last - first + 1;
		}

		//int operator[](unsigned index) const
		//{
			//return first + index;
		//}
	};
}

#endif
