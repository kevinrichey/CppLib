#include "kwrlib.h"
#include "kwrgame.h"
#include <utility>
#include <iostream>

using namespace kwr;
using namespace std;

static const SDL_Color BlackOpaque = { 0,0,0,SDL_ALPHA_OPAQUE};

class LatticeNoise //{{{2
{
   public:
      static const unsigned size;
      static const unsigned mask;
      static const unsigned min;
      static const unsigned max;

      LatticeNoise();
      unsigned operator()(unsigned x, unsigned y);
   
   private:
      Array<unsigned> lattice;
};

const unsigned LatticeNoise::size = 256;
const unsigned LatticeNoise::mask = 0xFF;
const unsigned LatticeNoise::min  = 0;
const unsigned LatticeNoise::max  = 255;

LatticeNoise::LatticeNoise()
   : lattice(size)
{
   lattice = count(0);
   lattice.shuffle();
}

unsigned LatticeNoise::operator()(unsigned x, unsigned y) 
{
   return lattice[ (x + lattice[y&mask]) & mask ];
}

class ValueNoise //{{{2
{
   public:
      static const unsigned size = 256;

      ValueNoise();
      double operator()(unsigned x, unsigned y);

   private:
      LatticeNoise lattice;
      Array<double> valueTable;
};

ValueNoise::ValueNoise()
   : valueTable(size)
{
   valueTable = RandomIterator<RandomDouble>();
}

double ValueNoise::operator()(unsigned x, unsigned y)
{
   return valueTable[lattice(x,y)];
}

class NoiseWindow : public SimpleDrawWindow //{{{2
{
   public:
      NoiseWindow();
      virtual void Setup();
      virtual void Render();
      void DrawNoise(int left, int top, double (*interp)(double, double, double));
      void DrawNoiseSplerp(int left, int top);
      void Perlin(const SDL_Rect &drawRect, uint32_t (NoiseWindow::*Interp)(uint32_t, uint32_t, double) );
   private:
      LatticeNoise lattice;
      ValueNoise value;

      uint32_t Ceiling(uint32_t a, uint32_t b, double f);
};

NoiseWindow::NoiseWindow() //{{{3
   : SimpleDrawWindow(900, 600, BlackOpaque)
{
}

void NoiseWindow::Setup() //{{{3
{
}

void NoiseWindow::Render() //{{{3
{
   DrawNoise(0, 0, &lerp );
   DrawNoise(300, 0, &coserp);
   DrawNoiseSplerp(600, 0);
}

void NoiseWindow::DrawNoise(int left, int top, double (*interp)(double, double, double)) //{{{3
{
   double du = 0.04;
   double dv = 0.04;

   int right = left + 256;
   int bottom = top + 256;

   double u = 0.0;
   for(int x = left; x < right; x++)
   {
      double v = 0.0;
      for(int y = top; y < bottom; y++)
      {
         unsigned ui = static_cast<unsigned>(u);
         unsigned vi = static_cast<unsigned>(v);
         double uf = u - (double)ui;
         double vf = v - (double)vi;

         double tl = value(ui,vi);
         double tr = value(ui+1, vi);
         double bl = value(ui, vi+1);
         double br = value(ui+1, vi+1);

         uint8_t gray = static_cast<uint8_t>(
            (interp( 
               interp(tl, tr, uf),  
               interp(bl, br, uf), 
               vf)) * 255);

         renderer.SetColor( SDL_Color { gray, gray, gray, SDL_ALPHA_OPAQUE } );
         SDL_RenderDrawPoint(renderer, x, y);

         v += dv;
      }
      u += du;
   }
}

void NoiseWindow::DrawNoiseSplerp(int left, int top) //{{{3
{
   double du = 0.04;
   double dv = 0.04;

   int right = left + 256;
   int bottom = top + 256;

   double u = 0.0;
   for(int x = left; x < right; x++)
   {
      double v = 0.0;
      for(int y = top; y < bottom; y++)
      {
         unsigned ui = static_cast<unsigned>(u);
         unsigned vi = static_cast<unsigned>(v);
         double uf = u - (double)ui;
         double vf = v - (double)vi;

         double xknots[4], yknots[4];

         for(int j = -1; j <= 2; j++)
         {
            for(int i = -1; i <= 2; i++)
            {
               xknots[i+1] = value(ui+i, vi+j);
            }
            yknots[j+1] = spline(uf, 4, xknots);
         }

         double g = spline(vf, 4, yknots);
         if(g > 1.0) g = 0.99999;
         if(g < 0.0) g = 0.00001;
         uint8_t gray = static_cast<uint8_t>(g * 255);

         renderer.SetColor( SDL_Color { gray, gray, gray, SDL_ALPHA_OPAQUE } );
         SDL_RenderDrawPoint(renderer, x, y);

         v += dv;
      }
      u += du;
   }
}
void NoiseWindow::Perlin(const SDL_Rect &rect, uint32_t (NoiseWindow::*Interp)(uint32_t, uint32_t, double) ) //{{{3
{
   double scale = 32.0;
   for(int x=0; x<rect.w; x++)
   {
      for(int y=0; y<rect.h; y++)
      {
         uint64_t total = 0;

         for(int i=0; i<5; i++)
         {
            auto freq = pow(2.0, i);
            auto amp = pow(0.25, i);

            double xf = (double)x / scale * freq;
            double yf = (double)y / scale * freq;
            unsigned xi = static_cast<unsigned>(xf);
            unsigned yi = static_cast<unsigned>(yf);
            double frac_x = xf - (double)xi;
            double frac_y = yf - (double)yi;

            total += 
               (this->*Interp)( 
                     (this->*Interp)(lattice(yi, xi), lattice(yi, xi+1), frac_x),
                     (this->*Interp)(lattice(yi+1, xi), lattice(yi+1, xi+1), frac_x),
                     frac_y) * amp;
         }

         uint8_t gray = total >> 24;

         renderer.SetColor( SDL_Color { gray, gray, gray, SDL_ALPHA_OPAQUE } );
         SDL_RenderDrawPoint(renderer, x+rect.x, y+rect.y);
      }
   }
}

uint32_t NoiseWindow::Ceiling(uint32_t a, uint32_t b, double f) //{{{3
{
   return a;
}

int main( int argc, char* args[] ) //{{{2
{
   NoiseWindow noiseWindow;
   noiseWindow.Run();
   return 0;
}

// vim: foldmethod=marker
