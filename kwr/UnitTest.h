#ifndef kwr_UNITTEST_H
#define kwr_UNITTEST_H 

#include "SourceLine.h"

namespace kwr
{
	class UnitTest
	{
		UnitTest* next;
		const char* name;

		static UnitTest*& Tests();

		protected: 
		UnitTest(const char* testName);

		protected:
		virtual void Run() const =0;
		void Assert(const char* filename, int lineNum, bool condition) const;

		public: 
		static void RunAll();
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
