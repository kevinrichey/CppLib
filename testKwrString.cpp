#include <cstdio>
#include <cstring>
#include "UnitTest.h"
#include "kwrstring.h"

using namespace std;
using namespace kwr;

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

kwr_Test(StringConvert_int)
{
	kwr_Assert( Expect(String(10)) == String("10") );
	kwr_Assert( Expect(String(-1)) == String("-1") );
}

kwr_Test(StringConvert_String)
{
	String text("text");
	kwr_Assert( Expect(String(text)) == String("text") );
}

kwr_Test(StringConvert_chars)
{
	const char* text = "text";
	kwr_Assert( Expect(String(text)) == String("text") );
}

kwr_Test(StringConvert_bool)
{
	kwr_Assert( Expect(String(true)) == String("true") );
	kwr_Assert( Expect(String(false)) == String("false") );
}

kwr_Test(StringConvert_double)
{
	kwr_Assert( Expect(String(0.1)) == String("0.100000") );
}

kwr_Test(String_with_printf)
{
	char output[100];
	snprintf(output, sizeof(output), "%s", String(100).Cstr());
	kwr_Assert( Expect(strcmp(output,"100")) == 0 );
}

