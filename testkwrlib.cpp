#include <cstring>
#include "kwrlib.h"
#include "TestCase.h"

using namespace std;
using namespace kwr;

kwr_TESTCASE(SourceLine)
{
	auto source = kwr_SourceLine;
	kwr_TEST(source.linenumber == __LINE__-1);
	kwr_TEST(!strcmp(source.filename, "testkwrlib.cpp"));
   kwr_TEST(!strcmp(KWR_SOURCE_LINE, "testkwrlib.cpp:13"));
}


