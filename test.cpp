#include <cstring>
#include <cassert>
#include "kwr/SourceLine.h"

int main()
{
	auto source = kwr_SourceLine;
	assert(source.linenumber == __LINE__-1);
	assert(!strcmp(source.filename, "test.cpp"));

	return 0;
}

