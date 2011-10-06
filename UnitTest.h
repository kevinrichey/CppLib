#ifndef kwr_UNITTEST_H
#define kwr_UNITTEST_H 

#include "assertion.hpp"
#include "kwrstring.h"
#include <cstdio>

namespace kwr
{
	class UnitTest
	{
		UnitTest* next;
		const char* name;

		static UnitTest*& Tests();

		protected: 
		UnitTest(const char* testName);
		virtual void Run() const =0;

		public: 
		static void RunAll();
		void Assert(const char* filename, int lineNum, bool condition) const;

		template<class Actual, class Expected> 
		void Assert(const char* filename, int lineNum, const Assertion<Actual,Expected>& assertion) const
		{
			if(!assertion.result) 
			{
				printf("%s:%d: %s: %s Assert failed\n", filename, lineNum, name, assertion.type);
				printf("   Expected: %s\n", String(assertion.expected).Cstr());
				printf("    but was: %s\n", String(assertion.actual).Cstr());
			}
		}
	};

#define kwr_Assert(testValue)  Assert(__FILE__, __LINE__, (testValue))

#define kwr_Test(testName) \
class Test##testName : public kwr::UnitTest { \
	public: Test##testName() : UnitTest(#testName) {} \
	void Run() const; \
} test##testName; \
void Test##testName::Run() const

}
#endif
