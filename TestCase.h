#pragma once
#include <cstdio>

namespace kwr {

class TestCase
{
  public: 

    static int RunAll();
    void Fail(const char* message, const char* filename, int lineNum) const;
    void Assert(bool result, const char* expression, const char* filename, int lineNum) const;

  protected: 

    TestCase(const char* testName);
    virtual void Run() =0;

  private:

    const char* name;
    TestCase* next;

    static TestCase*& Tests();
};

#define kwr_TestCase(testName) \
class Test_##testName : public kwr::TestCase { \
	public: Test_##testName() : TestCase(#testName) {} \
	void Run(); \
} test_##testName; \
void Test_##testName::Run()


#define kwr_FAIL(message)    Fail((message), __FILE__, __LINE__)
#define kwr_Fail(message)    Fail((message), __FILE__, __LINE__)
#define kwr_TEST(testValue)  Assert((testValue), #testValue, __FILE__, __LINE__)
#define kwr_Test(testValue)  Assert((testValue), #testValue, __FILE__, __LINE__)
#define kwr_TestStr(str,expect)  kwr_Test( str && !strcmp(str, expect) )

}

// vim: foldmethod=marker
