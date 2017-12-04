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
      Random32 random;
      Sprite goblin;
      Sprite bluegoblin;
};

MyGame::MyGame()
   :
      GameDriver( SDL_Color { 0, 100, 0, 0 } ),
      hero(renderer, "bin/HeroFace.bmp"),
      goblin(renderer, "bin/GoblinFace.bmp"),
      bluegoblin(renderer, "bin/BlueGoblinFace.bmp"),
      random((uint32_t)time(NULL))
{
}

void MyGame::Setup()
{
   hero.MoveTo( random % 600, random % 460);
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
            hero.SetVelocityX(-dir);
            break;

         case SDLK_RIGHT:
            hero.SetVelocityX(dir);
            break;

         case SDLK_UP:
            hero.SetVelocityY(-dir);
            break;

         case SDLK_DOWN:
            hero.SetVelocityY(dir);
            break;
      }
   }

}

void MyGame::Update()
{
   if (!hero.WouldHit(goblin) && !hero.WouldHit(bluegoblin))
      hero.Move();
}

void MyGame::Render()
{
   hero.Draw(renderer);
   goblin.Draw(renderer);
   bluegoblin.Draw(renderer);
}


int main( int argc, char* args[] )
{
   MyGame driver;
   driver.Run();
   return 0;
}

// vim: foldmethod=marker
