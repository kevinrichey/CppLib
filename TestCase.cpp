#include <cstdlib>
#include <cstdio>
#include "TestCase.h"

namespace kwr {

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

void TestCase::Fail(const char* message, const char* filename, int lineNum) const
{
  printf("%s:%d: %s Failure: %s\n", filename, lineNum, name, message);
}

void TestCase::Assert(bool result, const char* expression, const char* filename, int lineNum) const
{
  if(!result) 
  {
    Fail(expression, filename, lineNum);
  }
}

int TestCase::RunAll()
{
  int count = 0;

  for(TestCase* test=Tests(); test; test=test->next)
  {
    count++;
    test->Run();
  }

  return count;
}

} // kwr

