#pragma once
#include <cstdint>
#include <cmath>
#include <utility>
#include <limits>
#include <iostream>

namespace kwr {

struct NonCopyable
{
  NonCopyable() =default;
  explicit NonCopyable (const NonCopyable&) =delete;
  NonCopyable& operator= (const NonCopyable&) =delete;
};

struct SourceLine
{
  const char* const filename;
  const int linenumber;
};

#define KWR_STRINGIFICATE(x) #x
#define KWR_SOURCE_LINE_STR(file, line) file ":" KWR_STRINGIFICATE(line)
#define KWR_SOURCE_LINE  KWR_SOURCE_LINE_STR(__FILE__, __LINE__)
#define kwr_SourceLine ::kwr::SourceLine { __FILE__, __LINE__ }

extern const double pi;

template<typename Type> 
inline Type lerp(Type a, Type b, double f)
{
  return a + f * (b - a);
}

template<typename Type> 
Type coserp(Type a, Type b, double x)
{
  return lerp(a, b, (1.0L - std::cos(x*pi)) * 0.5L);
}

template <typename Type>
inline Type sCurve(Type x)
{
  return (x*x * (3.0 - 2.0*x));
}

double step(double a, double x);
double pulse(double a, double b, double x);
double clamp(double a, double b, double x);
double smoothstep(double a, double b, double x);
double spline(double x, double nknots, double *knots);

/* XorShift template options
   | 1, 3,10| 1, 5,16| 1, 5,19| 1, 9,29| 1,11, 6| 1,11,16| 1,19, 3| 1,21,20| 1,27,27|
   | 2, 5,15| 2, 5,21| 2, 7, 7| 2, 7, 9| 2, 7,25| 2, 9,15| 2,15,17| 2,15,25| 2,21, 9|
   | 3, 1,14| 3, 3,26| 3, 3,28| 3, 3,29| 3, 5,20| 3, 5,22| 3, 5,25| 3, 7,29| 3,13, 7|
   | 3,23,25| 3,25,24| 3,27,11| 4, 3,17| 4, 3,27| 4, 5,15| 5, 3,21| 5, 7,22| 5, 9,7 |
   | 5, 9,28| 5, 9,31| 5,13, 6| 5,15,17| 5,17,13| 5,21,12| 5,27, 8| 5,27,21| 5,27,25|
   | 5,27,28| 6, 1,11| 6, 3,17| 6,17, 9| 6,21, 7| 6,21,13| 7, 1, 9| 7, 1,18| 7, 1,25|
   | 7,13,25| 7,17,21| 7,25,12| 7,25,20| 8, 7,23| 8,9,23 | 9, 5,1 | 9, 5,25| 9,11,19|
   | 9,21,16|10, 9,21|10, 9,25|11, 7,12|11, 7,16|11,17,13|11,21,13|12, 9,23|13, 3,17|
   |13, 3,27|13, 5,19|13,17,15|14, 1,15|14,13,15|15, 1,29|17,15,20|17,15,23|17,15,26|
*/

template <int a, int b, int c>
class XorShift
{
  public:   
    typedef uint32_t result_type;

    explicit XorShift(uint32_t s) throw() : seed(s) {}

    inline uint32_t operator()() 
    {
      seed ^= (seed << a);
      seed ^= (seed >> b);
      seed ^= (seed << c);
      return seed;
    }

  private:
    uint32_t seed;
};

typedef XorShift<13,7,17> XorShift0;  // Marsaglia's favorite XorShift parameters

template<uint32_t a, uint32_t c, uint32_t m>
class LinearCongruentialGenerator
{
  public:
    typedef uint32_t result_type;

    explicit LinearCongruentialGenerator(uint32_t seed) : x(seed) {}
    inline uint32_t operator()() { return x = ((uint64_t)a * x + c) % m; }

  private:
    uint32_t x = 1;
};

typedef LinearCongruentialGenerator<48271U,0U,2147483647U> MINSTD;
typedef LinearCongruentialGenerator<69069U,362437U,2147483647U> MarsagliaLCG;

class ComplimentaryMultiplyWithCarry
{
  // https://en.wikipedia.org/wiki/Multiply-with-carry

  public:
    typedef uint32_t result_type;

    ComplimentaryMultiplyWithCarry(uint32_t seed) 
    {
      // 4096 random 32-bit integers for Q[]
      MINSTD minstd(seed);
      for (int q = 0; q < qSize; ++q)
        Q[q] = minstd();

      // Init c to random < cMax
      while (c = minstd() >= cMax) {}
    }

    inline uint32_t operator()() 
    {
      uint64_t t;
      uint32_t x;

      i = (i + 1) & (qSize-1);
      t = a * Q[i] + c;
      c = (t >> 32); 
      x = t + c; 

      if (x < c) {
        x++;
        c++;
      }

      return (Q[i] = r - x);    
    }

  private:

    // Constants as Marsaglia recommended
    static const uint64_t a = 18782LL;     
    static const uint32_t r = 0xfffffffe;
    static const unsigned qSize = 4096;
    static const unsigned cMax = 809430660;

    uint32_t Q[qSize], c = 362436; /* random initial c < 809430660 */
    uint32_t i = qSize - 1;
};

template <typename Function>
class FuncIterator
{
  public:
    FuncIterator() = delete;
    FuncIterator(Function f) : func(f) {}

    FuncIterator iterator() const { return *this; }
    void operator++() { value = func(); }
    typename Function::result_type operator*() const { return value; }
    operator bool() const { return true; }

  private:
    Function func;
    typename Function::result_type value = func();
};

class RandomUInt 
{
  public:
    typedef uint32_t ValueType;

    RandomUInt() : RandomUInt(xorseed) {}

    explicit RandomUInt(uint32_t seed) : 
      minstd(seed), xorshift(seed), mwc(seed) {}

    inline uint32_t operator()() 
    { 
      return (mwc() ^ minstd()) + (xorseed=xorshift());
    }

    uint32_t max() const { return std::numeric_limits<uint32_t>::max(); }

  private:
    static uint32_t xorseed;
    XorShift<13,7,17> xorshift;
    ComplimentaryMultiplyWithCarry  mwc;
    MINSTD minstd;
};

class RandomUniform
{
  public:
    typedef uint32_t Type;

    RandomUniform(Type lower, Type end) :
      dist(end - lower), start(lower) 
    {
      // Require lower < end
    }

    template<typename PRNG>
    Type operator()(PRNG& prng)
    {
      if (!dist) return start;
      Type r;
      while ((r = prng()) < threshold) {};
      return start + (r % dist);
    }

  private:
    Type start, dist;
    Type threshold = dist ? -dist % dist : dist;
};

class RandomDouble
{
  public:
    typedef double result_type;
    typedef double Type;
    static const uint32_t mask = 0x7FFFFFFF;

    RandomDouble() = default;
    explicit RandomDouble(uint32_t seed) : random(seed) {};

    inline result_type operator()()
    {
      return (result_type)random() / (result_type)random.max();
      //return (result_type)(random() & mask) / (result_type)mask;
    }

  private:
    RandomUInt random;
};

template <typename PRNG>
class FisherYatesShuffler
{
  public:
    FisherYatesShuffler(PRNG r) : rand(r) {}

    template <typename S, typename D>
    void operator()(S source, D dest)
    {
      for (int i = 0; i < dest.size(); ++i, source.pop()) {
        int j = RandomUniform(0, i)(rand);
        if (j != i) dest.put(i, dest.get(j));
        dest.put(j, source.get());
      }
    }

  private:
    PRNG rand;
};

template <class Randomizer=RandomUInt>
class RandomIterator
{
  public:
    RandomIterator() = default;
    RandomIterator iterator() const { return *this; }
    void operator++() { value = random.Next(); }
    typename Randomizer::ValueType operator*() const { return value; }
    operator bool() const { return true; }

  private:
    Randomizer random;
    typename Randomizer::ValueType value = random.Next();
};

/* class iterator
   {
      SELF iterator() const { return *this; }
      void operator++() { min++; }
      T operator*() const { return min; }
      operator bool() const { return min > max; }
   };
*/

template <typename T> class PointerIterator
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

template<typename Source, typename Dest> 
void IterCopy(Source source, Dest dest) 
{
  auto s = source.iterator();
  auto d = dest.iterator();
  while (s && d) {
    *d = *s;
    ++s;
    ++d;
  }
}

template <typename T> 
class Counter
{
  public:
    Counter(T v) : value(v) {}
    Counter<T> iterator() const { return *this; }
    size_t length() const { return std::numeric_limits<T>::max(); }
    void operator++() { ++value; }
    T operator*() const { return value; }

  private:
    T value;
};

template <typename T>
Counter<T> count(T t) { return Counter<T>(t); }

//**** Ranges *******************************************
/*
// Constants
//   front = 0   // index of first element
//   back = -1   // index of last element
//
// Editing:
//   pop(n=front)    // Remove n+1 elements from front
//   pop(-n)     // Remove n elements from end (n is negative)
//   crop(f,e)   // Shrink range to elements f through e
//   crop(f,-e)  // Shring range to elements f through end - e
//
// Length:
//   empty()   // true if range is exhausted
//   finite()  // true if size is finite
//   size()    // number of remaining elements
// 
// Access:
//   get(n=front)  // return n'th element
//   put(e, n=front)  // set current element to e
//   append(e)   // adds new element e to end 
//
// Traversal:
//   start(i)   // iterator to first element, increment by i
//   last(-i)   // Return iterator to last element, increment by -i
//   end()     // Return iterator to last+1 element.
//
// Iteration:
//   next(n=step)   // move up n positions
//   get()     // Return element at current position
//   put(e)    // Set element at current position
//   more()    // true if there is a next element
*/

template <typename T=int> 
class Range 
{
  public:
    Range(T tmin, T tmax) : min(tmin), max(tmax) {}
    Range(T tmin) : min(tmin), max((T)-1) {}
    size_t size() const { return max - min + 1; }
    void pop() { ++min; }
    T get() const { return min; }
    Range<T> to(T tmax) { return Range<T>(min, tmax); }

  private:
    T min, max;
};

template <typename T>
Range<T> from(T f) { return Range<T>(f); }

template <typename T> 
class PointerRange 
{
  public:
    PointerRange(T* b, T* e) : begin(b), end(e) {}
    size_t size() const { return end - begin; }
    void pop() { ++begin; }
    T get(size_t i=0) const { return begin[i]; }
    void put(T value) { *begin = value; }
    void put(size_t i, T value) { begin[i] = value; }

  private:
    T* begin;
    T* end;
};

template <typename T> 
PointerRange<T> over(T* pointer, size_t size)
{
  return PointerRange<T>(pointer, pointer+size);
}

template <typename PRNG, typename D>
class RandomRange
{
  public:
    RandomRange(PRNG p, D dist, size_t c) :
      rand(p), distribution(dist), count(c),
      value( distribution(rand) )
    {}

    size_t size() const { return count; }
    void pop() { --count; value = distribution(rand); }
    typename D::Type get() const { return value; }

  private:
    PRNG rand;
    D distribution;
    size_t count;
    typename D::Type value;
};

template <typename Function>
class Timer
{
  public:
    Timer(size_t c, Function f) : count(c), func(f) {}
    size_t length() const { return count; }
    void operator++() { --count; }
    typename Function::result_type operator*() { return func(); }

  private:
    unsigned count;
    Function func;
};

template <typename Function>
Timer<Function> Times(size_t count, Function f)
{
  return Timer<Function>(count, f);
}

template <typename T> 
class Array 
{
  public:

    explicit Array(size_t n) : first(new T[n]), end(first+n) {}

    Array(Array<T>& a) : first(new T[a.size()]), end(first+a.size())
    {
      int s = size();
      for (int i=0; i<s; ++i) 
        first[i] = a[i]; 
    }

    Array(Array<T>&& a) : first(std::move(a.first)), end(std::move(a.end))
    {
      a.first = a.end = nullptr;
    }

    Array<T>& operator=(Array<T>&& a) 
    {
      delete[] first;
      first = std::move(a.first);
      end = std::move(a.end);
      a.first = a.end = nullptr;
      return *this;
    }

    Array<T>& operator=(Array<T>& a) 
    {
      Array<T> temp(a);
      std::swap(first, a.first);
      std::swap(end, a.end);
      return *this;
    }

    template <typename FromIterator>
    Array(FromIterator f) : Array(f.size())
    {
      for (auto *i = first; i != end; ++i, f.pop()) {
        *i = f.get();
      }
    }

    template <typename F, typename S>
    Array(F function, S source) : Array(source.size())
    {
      function( source, over(first, size()) );
    }

    size_t size() const { return end-first; }

    PointerIterator<T> iterator() const 
    {
      return PointerIterator<T>(first, size());
    }

    T& operator[](size_t i)
    {
      // TODO: bounds checking
      return first[i];
    }

    template<typename RNG>
    void shuffle(RNG rand)
    {
      for (auto *i = first; i != end; i++) {
        std::swap( *i, first[rand() % size()] );
      }
    }

    ~Array()
    {
      delete[] first;
      first = end = nullptr;
    }

  private:
    T *first, *end;
};

} // kwr

// vim: foldmethod=syntax:
