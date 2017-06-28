#pragma once
#include <cstdio>

namespace kwr {

class TestCase
{
   const char* name;
   TestCase* next;

   static TestCase*& Tests();

   protected: 
   TestCase(const char* testName);
   virtual void Run() const =0;

   public: 
   static void RunAll();
   void Assert(bool result, const char* expression, const char* filename, int lineNum) const;
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
