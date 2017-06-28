#pragma once
#include <cstdint>

namespace kwr {

//{{{1 Random32

#define kwr_XORSHIFT32(seed)  (seed^=(seed<<13), seed^=(seed>>17), seed^=(seed<<5))

inline uint32_t XorShift32(uint32_t& seed) { return kwr_XORSHIFT32(seed); }

class Random32
{
   public:   
      explicit Random32(uint32_t s) throw() : seed(s) {}

      inline uint32_t operator%(uint32_t r)
      { 
         return kwr_XORSHIFT32(seed) % r; 
      }

   private:
      uint32_t seed;
};

struct NonCopyable //{{{1
{
   NonCopyable() =default;
   explicit NonCopyable (const NonCopyable&) =delete;
   NonCopyable& operator= (const NonCopyable&) =delete;
};

struct SourceLine //{{{1
{
   const char* const filename;
   const int linenumber;
};

#define KWR_STRINGIFICATE(x) #x
#define KWR_SOURCE_LINE_STR(file, line) file ":" KWR_STRINGIFICATE(line)
#define KWR_SOURCE_LINE  KWR_SOURCE_LINE_STR(__FILE__, __LINE__)
#define kwr_SourceLine ::kwr::SourceLine { __FILE__, __LINE__ }

} 
// vim: foldmethod=marker:
