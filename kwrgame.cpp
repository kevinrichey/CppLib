// 

#include "kwrgame.h"

namespace kwr {  //{{{1

//{{{2  Sprite class

Sprite::Sprite(Renderer& renderer, const char* filename)
{
   BitmapSurface bitmap(filename);
   texture.CreateFrom(renderer, bitmap);
   drawRect.x = 0;
   drawRect.y = 0;
   drawRect.w = bitmap->w;
   drawRect.h = bitmap->h;
}

void Sprite::MoveTo(int x, int y)
{
   drawRect.x = x;
   drawRect.y = y;
}

void Sprite::Move(int dx, int dy)
{
   drawRect.x += dx;
   drawRect.y += dy;
}

void Sprite::Draw(Renderer& renderer)
{
   SDL_RenderCopy( renderer, texture, NULL, &drawRect );
}

int Sprite::Width() const
{
   return drawRect.w;
}

int Sprite::Height() const
{
   return drawRect.h;
}

Sprite::~Sprite() throw()
{
}

//{{{2  GameDriver class

GameDriver::GameDriver()
   :
      window(640, 480, "Hello World"),
      renderer(window),
      running(false)
{
}

void GameDriver::Start()
{
   running = true;
}

void GameDriver::Run()
{
   try
   {
      Setup();
      Start();

      while(running)
      {
         SDL_Event e;
         while( SDL_PollEvent( &e ) != 0 ) 
            HandleEvent(e);
         
         Update();

         SDL_SetRenderDrawColor( renderer, 0x00, 100, 0x00, 0xFF );        
         SDL_RenderClear(renderer);

         Render();
         
         SDL_RenderPresent( renderer );
      }
      
   }
   catch(SDLError& sdlError)
   {
      fprintf( stderr, "SDLError: %s\n", sdlError.what());
   }
}

void GameDriver::Stop()
{
   running = false;
}

void GameDriver::Setup()
{
}

void GameDriver::HandleEvent(const SDL_Event& event)
{
}

void GameDriver::Render()
{
}

void GameDriver::Update()
{
}

}  //}}}1

// vim: foldmethod=marker:
