#include <cstdio>
#include <cstring>
#include <cassert>
#include <iostream>
#include "kwr/SourceLine.h"
#include "kwr/UnitTest.h"
#include "kwr/Range.hpp"
#include "kwr/assertion.hpp"
#include "kwr/kwrstring.h"

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

kwr_Test(String_copies_const_chars)
{
	const char literal[] = "Yarr!";
	String s(literal);
	kwr_Assert( Expect(s.Length()) == sizeof(literal)-1 );
	kwr_Assert( Expect(s.Cstr()) != (const char*)literal );
	kwr_Assert( !strcmp(s.Cstr(), literal) );
}

kwr_Test(String_copies_String)
{
	const char literal[] = "There can be, only one!";
	String first(literal);
	String second(first);
	kwr_Assert( Expect(second.Cstr()).NotNull() );
	kwr_Assert( !strcmp(second.Cstr(), literal) );
	kwr_Assert( Expect(second.Cstr()) != first.Cstr() );
}

kwr_Test(String_assignment)
{
	String first("To be, or not to be");
	String second("That is the question.");
	second = first;
	kwr_Assert( Expect(first) == second );
	kwr_Assert( Expect(first.Cstr()) != second.Cstr() );
}

kwr_Test(String_swap)
{
	String first("this");
	String second("that");
	first.Swap(second);
	kwr_Assert( Expect(first) == String("that") );
	kwr_Assert( Expect(second) == String("this") );
}

kwr_Test(String_equality_operator)
{
	String first("Ka-Chow!");
	String second("Kachooga!");
	kwr_Assert( Expect(first == second).IsFalse() );
	kwr_Assert( Expect(first) == String("Ka-Chow!") );
}

kwr_Test(String_inequality_operator)
{
	String first("Ka-Chow!");
	String second("Kachooga!");
	kwr_Assert( Expect(first != second).IsTrue() );
	kwr_Assert( Expect(first) != second );
}

int main()
{
	UnitTest::RunAll();
	return 0;
}

