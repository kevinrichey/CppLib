#include "kwrrnd.h"
#include <limits>

namespace kwr {


ComplimentaryMultiplyWithCarry::ComplimentaryMultiplyWithCarry(uint32_t seed) 
{
  // 4096 random 32-bit integers for Q[]
  MINSTD minstd(seed);
  for (int q = 0; q < qSize; ++q)
    Q[q] = minstd();

  // Init c to random < cMax
  while (c = minstd() >= cMax) {}
}

uint32_t ComplimentaryMultiplyWithCarry::operator()() 
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

uint32_t ComplimentaryMultiplyWithCarry::max() const 
{
  return std::numeric_limits<Type>::max(); 
}


}

