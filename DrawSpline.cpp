#include "kwrlib.h"
#include "kwrgame.h"

using namespace kwr;
using namespace std;

class SplineWindow : public GameDriver
{
   public:
      SplineWindow() : 
         GameDriver(600, 300, SDL_Color { 0,0,0 } )
         {}

      virtual void Render();
};

void SplineWindow::Render()
{
   double knots[] = { 0.5, 1.0, 3.0, 2.0, 2.5, 1.0 };
   
   SDL_Color white { 255, 255, 255, 255 };
   renderer.SetColor(white);

   for(int x=50; x<300; x++)
   {
      double f = ((double)x - 50.0L) / 250.0L;
      int y = spline(f, 6, knots) * 50.0L;
      SDL_RenderDrawPoint(renderer, x, y);
   }
}

int main( int argc, char* args[])
{
   SplineWindow driver;
   driver.Run();
   return 0;
}
