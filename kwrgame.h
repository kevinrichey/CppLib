#pragma once

#include "kwrsdl.h"


namespace kwr {  //{{{1

class Sprite //{{{2
{
   public:
      explicit Sprite(Renderer& renderer, const char* filename);
      void MoveTo(int x, int y);
      void Move(int dx, int dy);
      void Draw(Renderer& renderer);
      int Width() const;
      int Height() const;
      virtual ~Sprite() throw();

   private:
      Texture texture;
      SDL_Rect drawRect;
};

class GameScreen  //{{{2 
{
   public:
      virtual void Setup() =0;
      virtual void Draw() =0;
};

class GameDriver //{{{2
{
   public:
      explicit GameDriver();

      void Start();
      void Run();
      void Stop();

      virtual void Setup();
      virtual void HandleEvent(const SDL_Event& event);
      virtual void Update();
      virtual void Render();

   private:
      SDLInitialize sdlInit;
      bool running;

   protected:
      Window window;
      Renderer renderer;
};

}  //}}}1

// vim: foldmethod=marker:
