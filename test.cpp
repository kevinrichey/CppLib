#include <cstdio>
#include <cstring>
#include <cassert>
#include <iostream>
#include "kwr/SourceLine.h"
#include "kwr/UnitTest.h"
#include "kwr/Range.hpp"
#include "kwr/assertion.hpp"

using namespace std;
using namespace kwr;

kwr_Test(SourceLine)
{
	auto source = kwr_SourceLine;
	kwr_Assert(source.linenumber == __LINE__-1);
	kwr_Assert(!strcmp(source.filename, "test.cpp"));
}

kwr_Test(Range_Length_between_min_and_max)
{
	kwr_Assert(Range<int>(10,20).Length() == 11);
}

kwr_Test(Expect_equals_false)
{
	auto assertion = Expect(10) == 1;
	kwr_Assert( !assertion.result );
	kwr_Assert( assertion.actual == 10 );
	kwr_Assert( assertion.expected == 1 );
	kwr_Assert( !strcmp(assertion.type, "Equals") );
}

kwr_Test(Expect_equals_true)
{
	auto assertion = Expect(4) == 4;
	kwr_Assert( assertion.result );
	kwr_Assert( assertion.actual == 4 );
	kwr_Assert( assertion.expected == 4 );
	kwr_Assert( !strcmp(assertion.type, "Equals") );
}

kwr_Test(Expect_not_equals_false)
{
	auto assertion = Expect(5) != 5;
	kwr_Assert( !assertion.result );
	kwr_Assert( assertion.actual == 5 );
	kwr_Assert( assertion.expected == 5 );
	kwr_Assert( !strcmp(assertion.type, "Not equals") );
}

kwr_Test(Expect_not_equals_true)
{
	auto assertion = Expect(5) != 6;
	kwr_Assert( assertion.result );
	kwr_Assert( assertion.actual == 5 );
	kwr_Assert( assertion.expected == 6 );
	kwr_Assert( !strcmp(assertion.type, "Not equals") );
}

kwr_Test(Expect_IsNull_true)
{
	char* n = NULL;
	auto assertion = Expect(n).IsNull();
	kwr_Assert( assertion.result );
	kwr_Assert( !strcmp(assertion.actual, "Null") );
	kwr_Assert( !strcmp(assertion.expected, "Null") );
	kwr_Assert( !strcmp(assertion.type, "IsNull") );
}

kwr_Test(Expect_NotNull_false)
{
	char* n = NULL;
	auto assertion = Expect(n).NotNull();
	kwr_Assert( !assertion.result );
	kwr_Assert( !strcmp(assertion.actual, "Null") );
	kwr_Assert( !strcmp(assertion.expected, "not Null") );
	kwr_Assert( !strcmp(assertion.type, "NotNull") );
}

class Puke {};
void Barf() { throw Puke(); }
void Nope() { }

kwr_Test(Expect_Throws_true)
{
	auto assertion = Expect(Barf).Throws<Puke>();
	kwr_Assert( assertion.result );
	kwr_Assert( !strcmp(assertion.actual, "thrown exception") );
	kwr_Assert( !strcmp(assertion.expected, "thrown exception") );
	kwr_Assert( !strcmp(assertion.type, "Throws") );
}

kwr_Test(Expect_Throws_false)
{
	auto assertion = Expect(Nope).Throws<Puke>();
	kwr_Assert( !assertion.result );
	kwr_Assert( !strcmp(assertion.actual, "exception not thrown") );
	kwr_Assert( !strcmp(assertion.expected, "thrown exception") );
	kwr_Assert( !strcmp(assertion.type, "Throws") );
}

kwr_Test(Assertion_failures)
{
	kwr_Assert( Expect(100) == 1 );
	kwr_Assert( Expect(2) != 2 );
	kwr_Assert( Expect(this).IsNull() );
	kwr_Assert( Expect(Nope).Throws<Puke>() );
}

int main()
{
	UnitTest::RunAll();
	return 0;
}

