#include <cstdio>
#include <cstring>
#include <cassert>
#include "kwr/SourceLine.h"
#include "kwr/UnitTest.h"

using namespace kwr;

kwr_Test(FailingTest)
{
	kwr_Assert(false);
}

kwr_Test(SourceLine)
{
	auto source = kwr_SourceLine;
	Assert(__FILE__, __LINE__, source.linenumber == __LINE__-1);
	Assert(__FILE__, __LINE__, !strcmp(source.filename, "test.cpp"));
}

int main()
{
	UnitTest::RunAll();
	return 0;
}

