#include <cstdio>
#include "UnitTest.h"
#include "Range.hpp"

using namespace std;
using namespace kwr;

kwr_Test(Range_Length_between_min_and_max)
{
	kwr_Assert(Range<int>(10,20).Length() == 11);
}

