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
      Sprite goblin;
      Sprite bluegoblin;
};

MyGame::MyGame()
   :
      GameDriver( SDL_Color { 0, 100, 0, 0 } ),
      hero(renderer, "bin/HeroFace.bmp"),
      goblin(renderer, "bin/GoblinFace.bmp"),
      bluegoblin(renderer, "bin/BlueGoblinFace.bmp")
{
}

void MyGame::Setup()
{
   int width = 0, height = 0;
   SDL_GetRendererOutputSize(renderer, &width, &height);
   hero.MoveTo(width/2, height/2);
   goblin.MoveTo(width/4, height/4);
   bluegoblin.MoveTo(width-width/4, height/4);
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
