#include <cstring>
#include <iostream>
#include "kwrlib.h"
#include "kwrrnd.h"
#include "TestCase.h"

using namespace std;

namespace kwr {

kwr_TestCase(kwr_Stringize)
{
  kwr_TestStr(kwr_Stringize(XYZZY), "XYZZY");
}

kwr_TestCase(kwr_Concat)
{
  int kwr_Concat(xyzzy,123) = 123;
  auto valname = kwr_ValName(xyzzy123);
  kwr_TestStr(valname.name, "xyzzy123");
  kwr_Test(valname.value == 123);
}

kwr_TestCase(ValName)
{
  int x = 10;

  auto xvar = kwr_ValName(x);
  kwr_Test(xvar.value == 10);
  kwr_TestStr(xvar.name, "x");

  auto bvar = kwr_ValName(x + 1 < 100);
  kwr_Test(bvar.value == true);
  kwr_TestStr(bvar.name, "x + 1 < 100");

  auto svar = kwr_ValName("Test string ValName");
  kwr_TestStr(svar.value, "Test string ValName");
  kwr_TestStr(svar.name, "\"Test string ValName\"");
}

kwr_TestCase(NumRange)
{
  Range<int> numrange(1, 5);
  kwr_Test(numrange.size() == 5);
  kwr_Test(numrange.get() == 1);

  numrange.pop();
  kwr_Test(numrange.size() == 4);
  kwr_Test(numrange.get() == 2);

  numrange.pop();
  numrange.pop();
  numrange.pop();
  kwr_Test(numrange.size() == 1);
  kwr_Test(numrange.get() == 5);

  numrange.pop();
  kwr_Test(numrange.size() == 0);
}

kwr_TestCase(MemRange)
{
  int ints[5] { 10, 20, 30, 40, 50 };
  Range<int*> memrange(ints, 5);

  kwr_Test(memrange.size() == 5);
  kwr_Test(memrange.get() == 10);
  kwr_Test(memrange.more());

  memrange.pop();
  kwr_Test(memrange.size() == 4);
  kwr_Test(memrange.get() == 20);

  memrange.pop();
  memrange.pop();
  memrange.pop();
  kwr_Test(memrange.size() == 1);
  kwr_Test(memrange.get() == 50);

  memrange.pop();
  kwr_Test(memrange.size() == 0);


  int ints2[5] { 1, 2, 3, 4, 5 };

  Range<int*> range2(ints2, 5);
  range2.pop(2);
  kwr_Test(range2.size() == 3);
  kwr_Test(range2.get() == 3);
  range2.pop(4);
  kwr_Test(range2.size() == 0);

  Range<int*> range3(ints2, 5);
  range3.pop(-3);
  kwr_Test(range3.size() == 2);
  kwr_Test(range3.get() == 1);
  range3.pop(-4);
  kwr_Test(range3.size() == 0);
}

kwr_TestCase(SourceLine)
{
    SourceLine source { kwr_SourceLine };
    kwr_TestStr( source.filename, "testkwrlib.cpp" );
    kwr_Test( source.line == __LINE__ - 2);
}
 
kwr_TestCase(lerp)
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

kwr_TestCase(step)
{
   double a = 5;
   kwr_TEST( step(a, 0.0) == 0.0 );
   kwr_TEST( step(a, 4.0) == 0.0 );
   kwr_TEST( step(a, 5.0) == 1.0 );
   kwr_TEST( step(a, 6.0) == 1.0 );
}

kwr_TestCase(pulse)
{
   double a = 3.0, b = 6.0;
   kwr_TEST( pulse(a, b, 1.0) == 0.0 );
   kwr_TEST( pulse(a, b, 3.0) == 1.0 );
   kwr_TEST( pulse(a, b, 5.0) == 1.0 );
   kwr_TEST( pulse(a, b, 6.0) == 0.0 );
   kwr_TEST( pulse(a, b, 8.0) == 0.0 );
}

kwr_TestCase(clamp)
{
   double a = 4.0, b = 8.0;
   kwr_TEST( clamp(a, b, 2.0) == a );
   kwr_TEST( clamp(a, b, 4.0) == a );
   kwr_TEST( clamp(a, b, 6.0) == 6.0 );
   kwr_TEST( clamp(a, b, 8.0) == b );
   kwr_TEST( clamp(a, b, 9.0) == b );
}

kwr_TestCase(smoothstep)
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

kwr_TestCase(XorShift)
{
  // Generate the same sequence from the same seed
  XorShift<1,3,10> xorshift(1);
  kwr_TEST(xorshift() == 3075);
	kwr_TEST(xorshift() == 5898885);
	kwr_TEST(xorshift() == 3488497534);
	kwr_TEST(xorshift() == 2316485042);
	kwr_TEST(xorshift() == 1264294540);
	kwr_TEST(xorshift() == 2193138278);
	kwr_TEST(xorshift() == 1415128703);
	kwr_TEST(xorshift() == 996754385);

  // Different seeds generate different numbers;
  XorShift<2,5,15> a(1), b(2);
  kwr_TEST(a() != b());

  // Different params with same seed generate different numbers;
  XorShift<3,1,4> c(1);
  XorShift<2,7,7> d(1);
  kwr_TEST(c() != d());
}

kwr_TestCase(FisherYatesInsideOut)
{
  auto size = 100;
  int dest[size];

  XorShift0 rand { 123456789 };
  FisherYatesShuffler<XorShift0> shuffle(rand);
  shuffle(from(1), Range<int*>(dest, dest+99));

  int expected[size] { 
    3, 95, 29, 75, 46, 85, 19, 16, 20, 59, 21, 71, 34, 61, 32, 43, 82, 22, 83, 2, 39, 14, 56, 87, 96,
    94, 42, 65, 6, 69, 27, 86, 93, 37, 81, 30, 41, 78, 49, 92, 10, 84, 28, 8, 44, 79, 91, 88, 89, 98,
    18, 60, 51, 68, 97, 11, 4, 55, 50, 15, 17, 70, 90, 66, 62, 57, 23, 77, 48, 5, 80, 53, 38, 76, 31,
    73, 72, 64, 63, 52, 100, 99, 40, 36, 45, 35, 25, 33, 7, 9, 1, 13, 26, 58, 54, 12, 67, 47, 24, 74
  };

  for (int i = 0; i < size; ++i)
    kwr_TEST( expected[i] == dest[i] );
}

kwr_TestCase(Array)
{
   Array<int> hundred( Range<int>(1,100) );
   kwr_TEST( hundred.size() == 100 );
   for(int i=0; i<100; i++)
      kwr_TEST(hundred[i] == i+1);
}

//array of shuffled integers from 0 to 255
//
//	Array<int> lattice { shuffle, from(0).to(255) };
//		- array = new int[256]
//		- for(i=0; i<256; i++) array[i] = i;
//		- for(i=0; i<256; i++) swap(array[i], array[random(0, 255)]);
//
//array of 256 random floats from 0.0 to 1.0
//
//	Array<float> table { 256, randomize( from(0.0).to(1.0) ) };
//		from(0.0).to(1.0) -> Range<float> { min = 0.0; max = 1.0 }
//		- array = new float[256]
//		randomize( Range ) -> random_generator<Range::type> { min; max; float next() }
//		- for(int i=0; i<256; i++) array[i] = random(0.0, 1.0);

kwr_TestCase(UniformDistribution)
{
  using std::cout;
  using std::endl;

  XorShift0 rand { 123456789 };
  RandomUniform from0to100(0, 100);
  RandomUniform from100to200(100, 200);

  for (int i = 1000; i--; ) {
    uint32_t x;
    kwr_TEST(  from0to100(rand) < 100 );
    x =  from100to200(rand);
    kwr_TEST(x >= 100 && x < 200);
  }

}

kwr_TestCase(ArrayInitFromRange)
{
  Array<int> numbers { from(0).to(255) };

  int size = numbers.size();
  kwr_TEST(size == 256);
  for (int i = 0; i < size; ++i)
    kwr_TEST(i == numbers[i]);
}

kwr_TestCase(ArrayIndexOutOfRange)
{
  Array<int> a (10);

  a[0] = 1;
  a[9] = 2;

  try {
    //a[10];
    //kwr_Fail("Exception not thrown.");
  }
  catch (Failure& f) {
  }
}

kwr_TestCase(RequireFailureThrows)
{
    kwr_Checkpoint("RequireFailureThrows");
    int reqline = __LINE__ + 1;
    kwr_Assert( true );

    try 
    {
        reqline = __LINE__ + 1;
        kwr_Assert( false );
        kwr_Fail("Exception not thrown.");
    }
    catch (Failure& f) {
        kwr_TestStr(f.what(), "false");
        kwr_TestStr(f.source().filename, "testkwrlib.cpp");
        kwr_Test(f.source().line == reqline);
    }
    catch (...) {
        kwr_FAIL("Wrong exception type.");
    }
}

kwr_TestCase(kwrAssertFalse)
{
  try
  {
    kwr_Assert(false);
  }
  catch(Failure &f)
  {
    kwr_TestStr(f.what(), "false");
  }
}

kwr_TestCase(kwrAssertEquals)
{
  try
  {
    int x = 1;
    kwr_Assert(x == 0);
    kwr_Fail("Exception not thrown");
  }
  catch (Failure &f)
  {
    kwr_TestStr( f.what(), "x == 0");
  }
}

kwr_TestCase(KwrString)
{
  String sizedStr(100);
  kwr_Test(sizedStr.size() == 100);

  String copyStr("Copy this string.");
  kwr_Test(copyStr.size() == 17);
  kwr_TestStr(copyStr.getStr(), "Copy this string.");
  
  String a ("AAAAA");
  String b ("BBB");
  a.swap(b);
  kwr_TestStr(a.getStr(), "BBB");
  kwr_Test(a.size() == 3);
  kwr_TestStr(b.getStr(), "AAAAA");
  kwr_Test(b.size() == 5);

}


} // kwr

