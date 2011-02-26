#include <cstdio>
#include <cstring>
#include <cassert>
#include "kwr/SourceLine.h"
#include "kwr/UnitTest.h"
#include "kwr/Range.hpp"

using namespace kwr;

kwr_Test(FailingTest)
{
	kwr_Assert(false);
}

kwr_Test(SourceLine)
{
	auto source = kwr_SourceLine;
	kwr_Assert(source.linenumber == __LINE__-1);
	kwr_Assert(!strcmp(source.filename, "test.cpp"));
}

kwr_Test(Range_Length_between_min_and_max)
{
	kwr_Assert(Range<int>(10,20).Length() == 11);
}

int main()
{
	UnitTest::RunAll();
	return 0;
}

