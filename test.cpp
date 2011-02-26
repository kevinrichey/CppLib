#include <cstdio>
#include <cstring>
#include <cassert>
#include <iostream>
#include "kwr/SourceLine.h"
#include "kwr/assertion.hpp"

using namespace std;
using namespace kwr;

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

