#include "kwrlib.h"
#include "kwrgame.h"
#include <utility>
#include <iostream>

using namespace kwr;
using namespace std;


static const SDL_Color BlackOpaque = { 0,0,0,SDL_ALPHA_OPAQUE};

class ValueNoise
{
   public:
      static const size_t size = 256;
      static const unsigned mask = 0xFF;

      ValueNoise(unsigned seed=100);
      double Lattice(unsigned x, unsigned y);
      double noise(double u, double v);

   private:
      RandomDouble randomizer { 324058231 };
      Array<unsigned> lattice { Range<>(0,255) };
      Array<double> valueTable { Times(size, randomizer) };
};

ValueNoise::ValueNoise(unsigned seed) 
{
   RandomUInt random(seed);
   lattice.shuffle(random);
}

double ValueNoise::Lattice(unsigned x, unsigned y) 
{
   return valueTable[ lattice[ (x + lattice[y&mask]) & mask ] ];
}

double ValueNoise::noise(double x, double y) 
{
   int u0 = x;
   int v0 = y;
   int u1 = x + 1;
   int v1 = y + 1;

   double uf = sCurve(x - (double)u0);
   double vf = sCurve(y - (double)v0);

   double tl = Lattice(u0, v0);
   double tr = Lattice(u1, v0);
   double tm = lerp(tl, tr, uf);

   double bl = Lattice(u0, v1);
   double br = Lattice(u1, v1);
   double bm = lerp(bl, br, uf);

   return lerp(tm, bm, vf);
}

class FractalNoise 
{
   public:
      FractalNoise(double a, double b, unsigned o, ValueNoise* vn) 
         : alpha(a), beta(b), octaves(o), value(vn)
      {}

      double noise(double x, double y);

   private:
      double alpha, beta;
      unsigned octaves;
      ValueNoise* value;
};

double FractalNoise::noise(double x, double y) 
{
   double result = 0.0;
   double scale = 1.0;

   for(int i = 0; i < octaves; i++)
   {
      result += value->noise(x, y) / scale;
      scale *= alpha;
      x *= beta;
      y *= beta;
   }

   return result;
}

class NoiseWindow : public SimpleDrawWindow 
{
   public:
      NoiseWindow();
      virtual void Setup();
      virtual void Render();
      virtual void Clear() {};
      void DrawNoise(double scale);
      void DrawNoise3D(double scale);
   private:
      ValueNoise value;
      FractalNoise fractal;

};

NoiseWindow::NoiseWindow() 
   : 
      SimpleDrawWindow(600, 600, BlackOpaque), 
      value(200),
      fractal(2, 1.5, 4, &value)
{
}

void NoiseWindow::Setup() 
{
}

void NoiseWindow::Render() 
{
   static double scale = 0.02;
   static bool draw = true;
   if (draw) 
   {
      //DrawNoise3D(scale);
      DrawNoise(scale);
      draw = false;
   }
}

void NoiseWindow::DrawNoise(double scale) 
{
   double v = 0.0;
   for(int y = 0.0; y < 600; y++, v += scale)
   {
      double u = 0.0;
      for(int x = 0; x < 600; x++, u += scale)
      {
         //double g = value.noise(u, v);
         double g = fractal.noise(u, v) * 0.7; 
         if(g > 1.0) g = 1.0;
         uint8_t shade = (uint8_t)(g * 255);
         SDL_Color color { shade, shade, shade, SDL_ALPHA_OPAQUE };
         renderer.SetColor(color); 
         SDL_RenderDrawPoint(renderer, x, y);
      }
   }
}

void NoiseWindow::DrawNoise3D(double scale) 
{
   double screen_scale = 600.0 / 2.0;
   double sea = 0.2;
   double grass = sea + 0.1;
   double mountain = grass + 0.3; 

   double v = 0.0;
   for(double wy = 3; wy >= 1; wy-=0.0022)
   {
      double u = 0.0;
      for(double wx = -1; wx <= 1; wx+=0.0022)
      {
         double g = fractal.noise(u, v);

         double elev = max(g/4, sea);
         double wz = elev - 1.0;

         double vx = wx;
         double vy = wy + 0.1;
         double vz = wz * 1.1;

         int sx  = (1.0 + (vx / vy)) * screen_scale;
         int sy  = (1.0 - (vz / vy)) * screen_scale;
         int sy2 = (1.0 - (-1 / vy)) * screen_scale;

         uint8_t gray = static_cast<uint8_t>( clamp(0, 255, g * 128) );

         if(elev < sea+0.000001)
         {
            renderer.SetColor( SDL_Color { 0, 0, 200, SDL_ALPHA_OPAQUE } );
         }
         else if(elev < sea+0.02)
         {
            renderer.SetColor( SDL_Color { 200, 200, 0, SDL_ALPHA_OPAQUE } );
         }
         else if(elev < grass)
         {
            double shade = 1.0 - (elev - sea) * 5;
            renderer.SetColor( SDL_Color { 0, (uint8_t)(shade * 255), 0, SDL_ALPHA_OPAQUE } );
         }
         else //if (elev < mountain)
         {
            double shade = .4 + (elev - grass) * 2;
            uint8_t c = (uint8_t)(shade * 255.0);
            renderer.SetColor( SDL_Color { c, c, c, SDL_ALPHA_OPAQUE } );
         }

         SDL_RenderDrawLine(renderer, sx, sy, sx, sy2);

         u += scale;
      }
      v += scale;
   }
}

int main( int argc, char* args[] ) 
{
   NoiseWindow noiseWindow;
   noiseWindow.Run();
   return 0;
}

// vim: foldmethod=syntax
