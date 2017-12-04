#pragma once
#include <cstdint>
#include <cmath>
#include <utility>

namespace kwr {

// Grouping Levels:
// 1. Category
// 2. Classes
// 3. Methods

//{{{1 Utility

struct NonCopyable //{{{2
{
   NonCopyable() =default;
   explicit NonCopyable (const NonCopyable&) =delete;
   NonCopyable& operator= (const NonCopyable&) =delete;
};

struct SourceLine //{{{2
{
   const char* const filename;
   const int linenumber;
};

#define KWR_STRINGIFICATE(x) #x
#define KWR_SOURCE_LINE_STR(file, line) file ":" KWR_STRINGIFICATE(line)
#define KWR_SOURCE_LINE  KWR_SOURCE_LINE_STR(__FILE__, __LINE__)
#define kwr_SourceLine ::kwr::SourceLine { __FILE__, __LINE__ }

//{{{1  lerp, coserp, step, pulse, clamp, smoothstep, spline

extern const double pi;

template<typename Type> 
Type lerp(Type a, Type b, double f)
{
   return a * (1.0 - f) + b * f;
}

template<typename Type> 
Type coserp(Type a, Type b, double x)
{
   return lerp(a, b, (1.0L - std::cos(x*pi)) * 0.5L);
}

double step(double a, double x);
double pulse(double a, double b, double x);
double clamp(double a, double b, double x);
double smoothstep(double a, double b, double x);
double spline(double x, double nknots, double *knots);

//{{{1 Random

class XorShift //{{{2
{
public:   
   explicit XorShift(uint32_t s=2463534242) throw() : seed(s) {}

   inline uint32_t Next() 
   {
      seed ^= (seed << 13);
      seed ^= (seed >> 17);
      seed ^= (seed << 5);
      return seed;
   }

private:
   uint32_t seed;
};

class MultiplyWithCarry //{{{2
{
public:
   explicit MultiplyWithCarry() : z(362436069), w(521288629) {}

   inline uint32_t Next() 
   {
      z = 36969 * (z & 65535) + (z >> 16);
      w = 18000 * (w & 65535) + (w >> 16);
      return ((z << 16) + w);
   }
 
private:
   uint32_t z, w;
};

class CongruentialGenerator //{{{2
{
public:
   explicit CongruentialGenerator() : jcong(380116160) {}

   inline uint32_t Next() 
   {
      return (jcong = 69069 * jcong + 1234567);
   }

private:
   uint32_t jcong;
};

class RandomUInt //{{{2
{
   public:
      typedef uint32_t ValueType;

      inline ValueType Next() 
      {
         return ((mwc.Next() ^ cong.Next()) + xorshift.Next());
      }

   private:
      XorShift xorshift;
      MultiplyWithCarry  mwc;
      CongruentialGenerator cong;
};

class RandomDouble //{{{2
{
   public:
      typedef double ValueType;
      static const uint32_t mask = 0x7FFFFFFF;
      static const ValueType divisor;

      explicit RandomDouble(double lower=0.0, double upper=1.0)
         : scale(upper-lower), translate(lower) {}

      inline ValueType Next()
      {
         return translate + scale * (ValueType)(random.Next() & mask) / divisor;
      }

   private:
      RandomUInt random;
      const ValueType translate, scale;
};

//{{{2 class RandomIterator 
template <class Randomizer=RandomUInt>
class RandomIterator
{
   public:
      RandomIterator() : value(random.Next()) {}
      RandomIterator iterator() const { return *this; }
      void operator++() { value = random.Next(); }
      typename Randomizer::ValueType operator*() const { return value; }
      operator bool() const { return true; }

   private:
      Randomizer random;
      typename Randomizer::ValueType value;
};

//{{{1 Iterators

 /*
   class iterator
   {
      SELF iterator() const { return *this; }
      void operator++() { min++; }
      T operator*() const { return min; }
      operator bool() const { return min > max; }
   };
 */

   template <typename T> class PointerIterator //{{{2
   {
   public:
      PointerIterator(T* first, size_t length) : element(first), end(first+length) {}
      PointerIterator<T> iterator() const { return *this; }

      void operator++() { ++element; }
      T& operator*() const { return *element; }
      operator bool() const { return element != end; }

   private:
      T* element;
      T* end;
   };

   template<typename Source, typename Dest> void IterCopy(Source source, Dest dest) //{{{2
   {
      auto s = source.iterator();
      auto d = dest.iterator();
      while(s && d)
      {
         *d = *s;
         ++s;
         ++d;
      }
   }

   template <typename T> class Counter //{{{2
   {
   public:
      Counter(const T& v) : value(v) {}
      Counter<T> iterator() const { return *this; }
      void operator++() { ++value; }
      T operator*() const { return value; }
      operator bool() const { return true; }

   private:
      T value;
   };

   template <typename T>
   Counter<T> count(T t) { return Counter<T>(t); }

   template <typename T>
   class Range //{{{2
   {
   public:
      Range(T tmin, T tmax) : min(tmin), max(tmax) {}
      size_t size() const { return max - min + 1; }
      void operator++() { ++min; }
      T operator*() const { return min; }
      operator bool() const { return min > max; }

   private:
      T min, max;
   };

   template <typename T>
   Range<T> range(T min, T max)
   {
      return Range<T>(min, max);
   }

//{{{1 Array
template <typename T> 
class Array : NonCopyable 
{
public:
   const size_t size;

   Array(size_t n) : size(n), data(new T[size]) {}

   PointerIterator<T> iterator() const 
   {
      return PointerIterator<T>(data, size);
   }

   template <typename I>
   Array<T> &operator=(const I& i)
   {
      return IterCopy(i, iterator()), *this;
   }

   T& operator[](size_t i)
   {
      // TODO: bounds checking
      return data[i];
   }
      
   void shuffle()
   {
      static RandomIterator<> r;
      auto i = iterator();
      while(i)
      {
         std::swap( *i, data[*r % size] );
         ++i;
         ++r;
      }
   }

   ~Array()
   {
      delete[] data;
   }

private:
   T *data;
};

//}}}1
} // kwr

// vim: foldmethod=marker:
