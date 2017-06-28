#include <stdio.h>
#include <stdlib.h>
#include <exception>
#include <time.h>
#include "kwrlib.h"
#include "kwrsdl.h"
#include "kwrgame.h"

using namespace kwr;

class MyGame : public GameDriver
{
   public:
      MyGame();
      virtual void Setup();
      virtual void HandleEvent(const SDL_Event& event);
      virtual void Update();
      virtual void Render();
   private:
      Sprite hero;
      int dx;
      int dy;
      Random32 random;
      Sprite goblin;
      Sprite bluegoblin;
};

MyGame::MyGame()
   :
      hero(renderer, "HeroFace.bmp"),
      dx(0), dy(0),
      goblin(renderer, "GoblinFace.bmp"),
      bluegoblin(renderer, "BlueGoblinFace.bmp"),
      random((uint32_t)time(NULL))
{
}

void MyGame::Setup()
{
   goblin.MoveTo( random % 600, random % 460);
   bluegoblin.MoveTo(random % 600, random % 460);
}

void MyGame::HandleEvent(const SDL_Event& e)
{
   if( e.type == SDL_QUIT )
   {
      Stop();
   }

   if( e.type == SDL_KEYDOWN || e.type == SDL_KEYUP )
   {
      int dir = 0;
      if (e.key.state == SDL_PRESSED) dir = 2;

      switch( e.key.keysym.sym )
      {
         case SDLK_LEFT:
            dx = -dir;
            break;

         case SDLK_RIGHT:
            dx = dir;
            break;

         case SDLK_UP:
            dy = -dir;
            break;

         case SDLK_DOWN:
            dy = dir;
            break;
      }
   }
}

void MyGame::Update()
{
   hero.Move(dx, dy);
}

void MyGame::Render()
{
   hero.Draw(renderer);
   goblin.Draw(renderer);
   bluegoblin.Draw(renderer);
}

int main( int argc, char* args[] ) //{{{1
{
   MyGame driver;
   driver.Run();
   return 0;
}

// }}}1

// vim: foldmethod=marker
