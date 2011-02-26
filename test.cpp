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

kwr_Test(FailingTest)
{
	kwr_Assert(false);
}

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

template<class AssertionType>
void PrintAssert(const AssertionType& assertion)
{
	cout << "Condition: " << assertion.type << endl;
	cout << "   Result: " << (assertion.result ? "true" : "false") << endl;
	cout << "   Expected: " << assertion.expected << endl;
	cout << "   But was:  " << assertion.actual << endl;
}

class Puke {};
void Barf()
{
	throw Puke();
}

void Nope()
{
}

int main()
{
	UnitTest::RunAll();

	PrintAssert( Expect(10) == 1 );
	PrintAssert( Expect(4) == 4 );
	PrintAssert( Expect(5) != 5 );
	PrintAssert( Expect(5) != 6 );

	char* n = NULL;
	PrintAssert( Expect(n).IsNull() );
	PrintAssert( Expect(n).NotNull() );

	PrintAssert( Expect(Barf).Throws<Puke>() );
	PrintAssert( Expect(Nope).Throws<Puke>() );

	return 0;
}

