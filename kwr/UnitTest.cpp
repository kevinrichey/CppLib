#include <cstdlib>
#include <cstdio>
#include "UnitTest.h"

namespace kwr
{
	UnitTest*& UnitTest::Tests()
	{
		static UnitTest* tests = NULL;
		return tests;
	}

	UnitTest::UnitTest(const char* testName) 
		: next(Tests()), name(testName) 
	{
		Tests() = this; 
	}

	void UnitTest::Assert(const char* filename, int lineNum, bool condition) const
	{
		if(!condition) 
		{
			printf("%s:%d: %s: Assert failed\n", filename, lineNum, name);
			printf("   Expected: true\n");
			printf("   But was:  false\n");
		}
	}

	void UnitTest::RunAll()
	{
		for(UnitTest* test=Tests(); test; test=test->next)
		{
			test->Run();
		}
	}

}
