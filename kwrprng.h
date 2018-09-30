#pragma once

#include <cstdint>
//#include <cmath>

//======================================================================
// Pseudo-Random Number Generation

namespace kwr {

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
    typedef uint32_t Type;

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
    typedef uint32_t Type;

    explicit LinearCongruentialGenerator(uint32_t seed) : x(seed) {}
    inline uint32_t operator()() { return x = ((uint64_t)a * x + c) % m; }

  private:
    uint32_t x = 1;
};

typedef LinearCongruentialGenerator<48271U,0U,2147483647U> MINSTD;
typedef LinearCongruentialGenerator<69069U,362437U,2147483647U> MarsagliaLCG;

// https://en.wikipedia.org/wiki/Multiply-with-carry
class ComplimentaryMultiplyWithCarry
{
  public:
    typedef uint32_t Type;

    ComplimentaryMultiplyWithCarry(uint32_t seed);
    uint32_t operator()();
    uint32_t max() const;

  private:

    // Constants as Marsaglia recommended
    static const uint64_t a = 18782LL;     
    static const uint32_t r = 0xfffffffe;
    static const unsigned qSize = 4096;
    static const unsigned cMax = 809430660;

    uint32_t Q[qSize], c = 362436; /* random initial c < 809430660 */
    uint32_t i = qSize - 1;
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
    typedef double Type;
    static const uint32_t mask = 0x7FFFFFFF;

    RandomDouble() = default;

    template<typename PRNG>
    Type operator()(PRNG& prng)
    {
        return (Type)prng() / (Type)prng.max();
    }
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
            if (j != i) dest[i] = dest[j];
            dest[j] = source.get();
        }
    }

  private:
    PRNG rand;
};

}

