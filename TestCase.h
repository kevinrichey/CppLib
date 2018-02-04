#pragma once
#include <cstdio>

namespace kwr {

class TestCase
{
   public: 

      static int RunAll();
      void Assert(bool result, const char* expression, const char* filename, int lineNum) const;

   protected: 

      TestCase(const char* testName);
      virtual void Run() const =0;

   private:

      const char* name;
      TestCase* next;

      static TestCase*& Tests();
};

#define kwr_TESTCASE(testName) \
class Test_##testName : public kwr::TestCase { \
	public: Test_##testName() : TestCase(#testName) {} \
	void Run() const; \
} test_##testName; \
void Test_##testName::Run() const

#define kwr_TEST(testValue)  Assert((testValue), #testValue, __FILE__, __LINE__)

}

// vim: foldmethod=marker
