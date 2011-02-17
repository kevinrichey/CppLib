#include <cstdio>
#include <cstring>
#include <cassert>
#include "kwr/SourceLine.h"

template<class AssertionType>
void PrintAssert(const AssertionType& assertion)
{
	printf("Condition: %s\n", assertion.Condition());
	printf("   Result: %s\n", assertion.Compute() ? "true" : "false");
	printf("   Expect: %d\n", assertion.expected);
	printf("   Actual: %d\n", assertion.actual);
}

#define kwr_AssertOperator(NAME,OP) \
template<class ExpectedType, class ActualType> struct Assert##NAME { \
	const ExpectedType& expected; \
	const ActualType& actual; \
	bool Compute() const { return expected OP actual; } \
	const char* Condition() const { return #NAME; } \
}

kwr_AssertOperator(Equals,==);
kwr_AssertOperator(Differs,!=);

template<class ActualType>
struct Expectation
{
	const ActualType& actualValue;

	template<class ExpectedType>
	AssertEquals<ActualType,ExpectedType> Equals(const ExpectedType& expectedValue) const
	{
		return AssertEquals<ActualType,ExpectedType> { actualValue, expectedValue };
	}

	template<class ExpectedType>
	AssertDiffers<ActualType,ExpectedType> Differs(const ExpectedType& expectedValue) const
	{
		return AssertDiffers<ActualType,ExpectedType> { actualValue, expectedValue };
	}

	AssertEquals<ActualType,ActualType> IsNull() const
	{
		return AssertEquals<ActualType,ActualType> { actualValue, NULL };
	}
};

template<class Type>
Expectation<Type> Expect(const Type& value)
{
	return Expectation<Type> { value };
}

int main()
{
	auto source = kwr_SourceLine;
	assert(source.linenumber == __LINE__-1);
	assert(!strcmp(source.filename, "test.cpp"));

	PrintAssert( Expect(10).Equals(1) );
	PrintAssert( Expect(4).Equals(4) );
	PrintAssert( Expect(5).Differs(5) );
	PrintAssert( Expect(5).Differs(6) );

	char* n = NULL;
	auto isNullAssert = Expect(n).IsNull();
	printf("Condition: %s\n", isNullAssert.Condition());
	printf("   Result: %s\n", isNullAssert.Compute() ? "true" : "false");
	printf("   Expect: %s\n", isNullAssert.expected ? isNullAssert.expected : "NULL");
	printf("   Actual: %s\n", isNullAssert.actual ? isNullAssert.actual : "NULL");

	return 0;
}

