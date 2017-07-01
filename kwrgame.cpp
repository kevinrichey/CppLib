// 

#include "kwrgame.h"

namespace kwr {  //{{{1

//{{{2 HitBox

bool HitBox::CollidesWith(const HitBox& target) const
{
   return 
      !( Left()   > target.Right()  ||
         Right()  < target.Left()   ||
         Top()    > target.Bottom() ||
         Bottom() < target.Top() );
}

HitBox HitBox::MovedBy(int x, int y) const
{
   return HitBox( SDL_Rect { Left()+x, Top()+y, Width(), Height() } );
}

//{{{2  Sprite class

Sprite::Sprite(Renderer& renderer, const BitmapSurface& bitmap)
   :
      texture(renderer, bitmap),
      hitbox( SDL_Rect { 0, 0, bitmap->w, bitmap->h } ),
      dx(0), dy(0)
{
}

Sprite::Sprite(Renderer& renderer, const char* filename)
   : 
      Sprite(renderer, BitmapSurface(filename))
{
}

void Sprite::MoveTo(int x, int y)
{
   hitbox.rect.x = x;
   hitbox.rect.y = y;
}

void Sprite::Move()
{
   hitbox.rect.x += dx;
   hitbox.rect.y += dy;
}

void Sprite::SetVelocityX(int v)
{
   dx = v;
}

void Sprite::SetVelocityY(int v)
{
   dy = v;
}

bool Sprite::WouldHit(const Sprite& target) const
{
   return hitbox.MovedBy(dx, dy).CollidesWith(target.hitbox);
}

void Sprite::Draw(Renderer& renderer)
{
   SDL_RenderCopy( renderer, texture, NULL, &hitbox.rect );
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

//{{{2  SimpleDrawWindow class

SimpleDrawWindow::SimpleDrawWindow()
{
}

void SimpleDrawWindow::Setup()
{
}

void SimpleDrawWindow::HandleEvent(const SDL_Event& event)
{
   if( event.type == SDL_QUIT )
   {
      Stop();
   }
}

void SimpleDrawWindow::Update()
{
}

}  //}}}1

// vim: foldmethod=marker:
