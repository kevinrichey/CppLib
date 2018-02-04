#include <cstring>
#include <iostream>
#include "kwrlib.h"
#include "TestCase.h"

using namespace std;
using namespace kwr;

kwr_TESTCASE(SourceLine)
{
	auto source = kwr_SourceLine;
	kwr_TEST(source.linenumber == __LINE__-1);
	kwr_TEST(!strcmp(source.filename, "testkwrlib.cpp"));
   kwr_TEST(!strcmp(KWR_SOURCE_LINE, "testkwrlib.cpp:14"));
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

kwr_TESTCASE(XorShift)
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

kwr_TESTCASE(Array)
{
   Array<int> hundred( Range<>(1,100) );
   kwr_TEST( hundred.size() == 100 );
   for(int i=0; i<100; i++)
      kwr_TEST(hundred[i] == i+1);
}

struct TimesCounter
{
   typedef int result_type;
   int *count = 0;
   int operator()()
   {
      return ++(*count);
   }
};

kwr_TESTCASE(Times)
{
   int count = 0;
   TimesCounter c { &count };
   auto timer = Times(5, c);

   kwr_TEST(timer.length() == 5);
   for(; timer.length(); ++timer) *timer;
   kwr_TEST(count == 5);
}

kwr_TESTCASE(TimesRandom)
{
  RandomDouble randomizer(1509797);

  auto t = Times(10, randomizer); 
	kwr_TEST(t.length() == 10 && abs(*t - 0.430444));  ++t;
	kwr_TEST(t.length() ==  9 && abs(*t - 0.755587));  ++t;
	kwr_TEST(t.length() ==  8 && abs(*t - 0.998875));  ++t;
	kwr_TEST(t.length() ==  7 && abs(*t - 0.396148));  ++t;
	kwr_TEST(t.length() ==  6 && abs(*t - 0.697454));  ++t;
	kwr_TEST(t.length() ==  5 && abs(*t - 0.436028));  ++t;
	kwr_TEST(t.length() ==  4 && abs(*t - 0.346031));  ++t;
	kwr_TEST(t.length() ==  3 && abs(*t - 0.224874));  ++t;
	kwr_TEST(t.length() ==  2 && abs(*t - 0.771649));  ++t;
	kwr_TEST(t.length() ==  1 && abs(*t - 0.713165));  ++t;
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

kwr_TESTCASE(UniformDistribution)
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

kwr_TESTCASE(ArrayInitFromRange)
{
  Array<int> numbers { from(0).to(255) };

  int size = numbers.size();
  kwr_TEST(size == 256);
  for (int i = 0; i < size; ++i)
    kwr_TEST(i == numbers[i]);

}

kwr_TESTCASE(FisherYatesInsideOut)
{
  auto size = 100;
  int dest[size];

  XorShift0 rand { 123456789 };
  FisherYatesShuffler<XorShift0> shuffle(rand);
  shuffle(from(1), over(dest, 100));

  int expected[size] { 
    3, 95, 29, 75, 46, 85, 19, 16, 20, 59, 21, 71, 34, 61, 32, 43, 82, 22, 83, 2, 39, 14, 56, 87, 96,
    94, 42, 65, 6, 69, 27, 86, 93, 37, 81, 30, 41, 78, 49, 92, 10, 84, 28, 8, 44, 79, 91, 88, 89, 98,
    18, 60, 51, 68, 97, 11, 4, 55, 50, 15, 17, 70, 90, 66, 62, 57, 23, 77, 48, 5, 80, 53, 38, 76, 31,
    73, 72, 64, 63, 52, 100, 99, 40, 36, 45, 35, 25, 33, 7, 9, 1, 13, 26, 58, 54, 12, 67, 47, 24, 74
  };

  for (int i = 0; i < size; ++i)
    kwr_TEST( expected[i] == dest[i] );

}

class Doubler
{
  public:
    template <typename S, typename D>
    void operator()(S source, D dest)
    {
      while (dest.size()) {
        dest.put( 2 * source.get() );
        source.pop();
        dest.pop();
      }
    }
};

kwr_TESTCASE(ArrayInitFromFunctor)
{
  Array<int> a { Doubler(), from(1).to(10) };

  int expected[10] { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20 };
  for (int i = 0; i < 10; ++i)
    kwr_TEST( expected[i] == a[i] );
}

kwr_TESTCASE(RandomRange)
{
  XorShift0 rand { 98347259 };
  RandomUniform uniform(500, 1000);
  RandomRange<XorShift0, RandomUniform> randrange(rand, uniform, 100);

  kwr_TEST(randrange.size() == 100);

  uint32_t expected[100] 
  {
    514, 912, 890, 532, 753, 666, 906, 922, 542, 855, 
    621, 778, 998, 742, 647, 924, 645, 945, 732, 647,
    552, 670, 646, 927, 973, 902, 947, 532, 767, 968,
    950, 841, 797, 674, 717, 989, 978, 541, 968, 567,
    862, 892, 634, 601, 515, 667, 820, 766, 946, 837,
    523, 900, 536, 620, 869, 793, 609, 522, 576, 821,
    513, 748, 616, 563, 790, 976, 653, 704, 532, 650,
    720, 506, 974, 926, 799, 550, 621, 651, 532, 844,
    982, 787, 874, 912, 589, 719, 795, 966, 635, 601,
    825, 882, 710, 966, 550, 775, 559, 799, 504, 910 
  };
  
  uint32_t *i = expected;

  while (randrange.size()) {
    auto g = randrange.get();
    kwr_TEST(500 <= g && g < 1000);
    kwr_TEST(g == *i++);
    kwr_TEST(g == randrange.get());
    randrange.pop();
  }
  std::cout << endl;

}

// vim: foldmethod=syntax
