#include "kwrlib.h"
#include "kwrgame.h"

using namespace kwr;
using namespace std;

static const unsigned lattice_width = 256;

static const SDL_Color BlackOpaque = { 0,0,0,SDL_ALPHA_OPAQUE};

class NoiseWindow : public SimpleDrawWindow
{
   public:
      NoiseWindow();
      virtual void Setup();
      virtual void Render();
      void DrawNoise(const SDL_Rect &drawRect, uint32_t (NoiseWindow::*Interp)(uint32_t, uint32_t, double) );
      void Perlin(const SDL_Rect &drawRect, uint32_t (NoiseWindow::*Interp)(uint32_t, uint32_t, double) );
   private:
      uint32_t seed;
      uint32_t lattice[lattice_width][lattice_width];

      uint32_t Ceiling(uint32_t a, uint32_t b, double f);
      uint32_t LinearInterp(uint32_t a, uint32_t b, double f);
      uint32_t CosineInterp(uint32_t a, uint32_t b, double f);
};

NoiseWindow::NoiseWindow()
   : SimpleDrawWindow(900, 600, BlackOpaque)
{
}

void NoiseWindow::Setup()
{
   seed = 12345678;
   for(unsigned x=0; x<lattice_width; x++)
      for(unsigned y=0; y<lattice_width; y++)
         lattice[y][x] = XorShift32(seed);
}

void NoiseWindow::Render()
{
   DrawNoise(SDL_Rect { 0,0, 256,256 }, &NoiseWindow::Ceiling );
   DrawNoise(SDL_Rect { 300,0, 256,256 }, &NoiseWindow::LinearInterp );
   DrawNoise(SDL_Rect { 600,0, 256,256 }, &NoiseWindow::CosineInterp );

   Perlin(SDL_Rect { 300,300, 256,256 }, &NoiseWindow::LinearInterp );
   Perlin(SDL_Rect { 600,300, 256,256 }, &NoiseWindow::CosineInterp );
}

void NoiseWindow::DrawNoise(const SDL_Rect &rect, uint32_t (NoiseWindow::*Interp)(uint32_t, uint32_t, double) )
{
   double scale = 32L;
   for(int x=0; x<rect.w; x++)
   {
      for(int y=0; y<rect.h; y++)
      {
         double xf = (double)x / scale;
         double yf = (double)y / scale;
         unsigned xi = static_cast<unsigned>(xf);
         unsigned yi = static_cast<unsigned>(yf);
         double frac_x = xf - (double)xi;
         double frac_y = yf - (double)yi;

         uint8_t gray = 
            (this->*Interp)( 
                  (this->*Interp)(lattice[yi][xi], lattice[yi][xi+1], frac_x),
                  (this->*Interp)(lattice[yi+1][xi], lattice[yi+1][xi+1], frac_x),
                  frac_y) >> 24;

         SDL_SetRenderDrawColor(renderer, gray, gray, gray, SDL_ALPHA_OPAQUE);
         SDL_RenderDrawPoint(renderer, x+rect.x, y+rect.y);
      }
   }
}

void NoiseWindow::Perlin(const SDL_Rect &rect, uint32_t (NoiseWindow::*Interp)(uint32_t, uint32_t, double) )
{
   double scale = 32L;
   for(int x=0; x<rect.w; x++)
   {
      for(int y=0; y<rect.h; y++)
      {
         uint64_t total = 0;

         for(int i=0; i<5; i++)
         {
            auto freq = pow(2L, i);
            auto amp = pow(0.25L, i);

            double xf = (double)x / scale * freq;
            double yf = (double)y / scale * freq;
            unsigned xi = static_cast<unsigned>(xf);
            unsigned yi = static_cast<unsigned>(yf);
            double frac_x = xf - (double)xi;
            double frac_y = yf - (double)yi;

            total += 
               (this->*Interp)( 
                     (this->*Interp)(lattice[yi][xi], lattice[yi][xi+1], frac_x),
                     (this->*Interp)(lattice[yi+1][xi], lattice[yi+1][xi+1], frac_x),
                     frac_y) * amp;
         }

         uint8_t gray = total >> 24;

         SDL_SetRenderDrawColor(renderer, gray, gray, gray, SDL_ALPHA_OPAQUE);
         SDL_RenderDrawPoint(renderer, x+rect.x, y+rect.y);
      }
   }
}

uint32_t NoiseWindow::Ceiling(uint32_t a, uint32_t b, double f)
{
   return a;
}

uint32_t NoiseWindow::LinearInterp(uint32_t a, uint32_t b, double f)
{
   //TODO: Ensure 0.0 <= f <= 1.0
   return a * (1.0L - f) + b * f;
}

uint32_t NoiseWindow::CosineInterp(uint32_t a, uint32_t b, double x)
{
   static const double pi = 3.1415927L; 
   return LinearInterp(a, b, (1.0L - cos(x*pi)) * 0.5L);
}

int main( int argc, char* args[] )
{
   NoiseWindow noiseWindow;
   noiseWindow.Run();
   return 0;
}

// vim: foldmethod=marker
