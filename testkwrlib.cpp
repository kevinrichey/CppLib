#include <cstring>
#include "kwrlib.h"
#include "TestCase.h"

using namespace std;
using namespace kwr;

kwr_TESTCASE(SourceLine)
{
	auto source = kwr_SourceLine;
	kwr_TEST(source.linenumber == __LINE__-1);
	kwr_TEST(!strcmp(source.filename, "testkwrlib.cpp"));
   kwr_TEST(!strcmp(KWR_SOURCE_LINE, "testkwrlib.cpp:13"));
}

kwr_TESTCASE(lerp)
{
   int a = 10, b = 20;
   kwr_TEST( lerp(a, b, 0.0) == 10 );
   kwr_TEST( lerp(a, b, 0.3) == 13 );
   kwr_TEST( lerp(a, b, 0.5) == 15 );
   kwr_TEST( lerp(a, b, 0.8) == 18 );
   kwr_TEST( lerp(a, b, 1.0) == 20 );

   double x = 0.5, y = 5.5;
   kwr_TEST( lerp(x, y, 0.0) == 0.5 );
   kwr_TEST( lerp(x, y, 0.3) == 2 );
   kwr_TEST( lerp(x, y, 0.5) == 3 );
   kwr_TEST( lerp(x, y, 0.8) == 4.5 );
   kwr_TEST( lerp(x, y, 1.0) == 5.5 );
}

kwr_TESTCASE(step)
{
   double a = 5;
   kwr_TEST( step(a, 0.0) == 0.0 );
   kwr_TEST( step(a, 4.0) == 0.0 );
   kwr_TEST( step(a, 5.0) == 1.0 );
   kwr_TEST( step(a, 6.0) == 1.0 );
}

kwr_TESTCASE(pulse)
{
   double a = 3.0, b = 6.0;
   kwr_TEST( pulse(a, b, 1.0) == 0.0 );
   kwr_TEST( pulse(a, b, 3.0) == 1.0 );
   kwr_TEST( pulse(a, b, 5.0) == 1.0 );
   kwr_TEST( pulse(a, b, 6.0) == 0.0 );
   kwr_TEST( pulse(a, b, 8.0) == 0.0 );
}

kwr_TESTCASE(clamp)
{
   double a = 4.0, b = 8.0;
   kwr_TEST( clamp(a, b, 2.0) == a );
   kwr_TEST( clamp(a, b, 4.0) == a );
   kwr_TEST( clamp(a, b, 6.0) == 6.0 );
   kwr_TEST( clamp(a, b, 8.0) == b );
   kwr_TEST( clamp(a, b, 9.0) == b );
}

kwr_TESTCASE(smoothstep)
{
   double a = 5.0, b = 7.0;
   kwr_TEST( smoothstep(a, b, 4.0) == 0.0 );
   kwr_TEST( smoothstep(a, b, 5.0) == 0.0 );
   kwr_TEST( smoothstep(a, b, 5.3) - 0.06075 < 0.0000001 );
   kwr_TEST( smoothstep(a, b, 6.0) == 0.5 );
   kwr_TEST( smoothstep(a, b, 6.9) - 0.99275 < 0.0000001 );
   kwr_TEST( smoothstep(a, b, 7.0) == 1.0 );
   kwr_TEST( smoothstep(a, b, 8.0) == 1.0 );
}

// vim: foldmethod=marker
