#include <cstdlib>
#include <cstdio>
#include "TestCase.h"

namespace kwr
{
	TestCase*& TestCase::Tests()
	{
		static TestCase* tests = NULL;
		return tests;
	}

	TestCase::TestCase(const char* testName) 
		: next(Tests()), name(testName) 
	{
		Tests() = this; 
	}

	void TestCase::Assert(bool result, const char* expression, const char* filename, int lineNum) const
	{
		if(!result) 
		{
			printf("%s:%d: %s Failure: %s\n", filename, lineNum, name, expression);
		}
	}

	void TestCase::RunAll()
	{
		for(TestCase* test=Tests(); test; test=test->next)
		{
			test->Run();
		}
	}

}
