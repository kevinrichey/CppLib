#include "kwrlib.h"
#include "kwrgame.h"
#include <utility>
#include <iostream>
#include <random>
#include <ctime>
#include <chrono>
#include <functional>

using namespace kwr;
using namespace std;

static const SDL_Color BlackOpaque = { 0,0,0,SDL_ALPHA_OPAQUE};

class TestWindow : public GameDriver
{
   public:
      TestWindow();
      virtual void Render();
      virtual void Clear() {}

      bool drawn = false;
};

TestWindow::TestWindow() : GameDriver(800, 800, BlackOpaque) { }

uint64_t NanoTimer(int loops, function<void (void)> func)
{
   auto begin = std::chrono::high_resolution_clock::now();
   for(int i=0; i<loops; i++) func();
   auto finish = std::chrono::high_resolution_clock::now();
   std::chrono::nanoseconds elapsed ( finish - begin );
   return elapsed.count();
}

void TestWindow::Render() 
{
   if(!drawn)
   {
      uint32_t seed = 123456789U;
      MINSTD minstd(seed);
      MarsagliaLCG mlcg(seed);
      XorShift<13,7,17> xorshift(seed);
      ComplimentaryMultiplyWithCarry cmwc(seed);

      SDL_Color white { 255, 255, 255, SDL_ALPHA_OPAQUE };
      renderer.SetColor(white); 

      clock_t start, end;
      int loops = 20000;

      cout << "MINSTD:   " << NanoTimer(loops, [this,&minstd]{ SDL_RenderDrawPoint(renderer, minstd() % 400, minstd() % 400); } ) / 1000 << endl;
      cout << "MLCG:     " << NanoTimer(loops, [this,&mlcg]{ SDL_RenderDrawPoint(renderer, 400+mlcg() % 400, mlcg() % 400); } ) / 1000 << endl;
      cout << "XorShift: " << NanoTimer(loops, [this,&xorshift]{ SDL_RenderDrawPoint(renderer, xorshift() % 400, 400+xorshift() % 400); } ) / 1000 << endl;
      cout << "CMWC:     " << NanoTimer(loops, [this,&cmwc]{ SDL_RenderDrawPoint(renderer, 400+cmwc() % 400, 400+cmwc() % 400); } ) / 1000 << endl;
      
      drawn = true;
   }
}

int main( int argc, char* args[] )
{
   TestWindow testwin;
   testwin.Run();
   return 0;
}

