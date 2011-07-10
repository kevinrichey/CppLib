#include <cstring>
#include "SourceLine.h"
#include "UnitTest.h"

using namespace std;
using namespace kwr;

kwr_Test(SourceLine)
{
	auto source = kwr_SourceLine;
	kwr_Assert(source.linenumber == __LINE__-1);
	kwr_Assert(!strcmp(source.filename, "testSourceLine.cpp"));
}


