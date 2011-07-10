#include <cstdio>
#include <cassert>
#include <cstring>
#include "UnitTest.h"
#include "assertion.hpp"

using namespace std;
using namespace kwr;

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

kwr_Test(Expect_IsFalse_true)
{
	auto assertion = Expect(false).IsFalse();
	kwr_Assert( assertion.result );
	kwr_Assert( !strcmp(assertion.actual, "false") );
	kwr_Assert( !strcmp(assertion.expected, "false") );
	kwr_Assert( !strcmp(assertion.type, "IsFalse") );
}

kwr_Test(Expect_IsFalse_false)
{
	auto assertion = Expect(true).IsFalse();
	kwr_Assert( !assertion.result );
	kwr_Assert( !strcmp(assertion.actual, "true") );
	kwr_Assert( !strcmp(assertion.expected, "false") );
	kwr_Assert( !strcmp(assertion.type, "IsFalse") );
}

kwr_Test(Expect_IsTrue_true)
{
	auto assertion = Expect(true).IsTrue();
	kwr_Assert( assertion.result );
	kwr_Assert( !strcmp(assertion.actual, "true") );
	kwr_Assert( !strcmp(assertion.expected, "true") );
	kwr_Assert( !strcmp(assertion.type, "IsTrue") );
}

kwr_Test(Expect_IsTrue_false)
{
	auto assertion = Expect(false).IsTrue();
	kwr_Assert( !assertion.result );
	kwr_Assert( !strcmp(assertion.actual, "false") );
	kwr_Assert( !strcmp(assertion.expected, "true") );
	kwr_Assert( !strcmp(assertion.type, "IsTrue") );
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


