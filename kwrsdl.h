#pragma once
#include <exception>
#include <SDL.h>
#include "kwrlib.h"

namespace kwr {

class SDLError : public std::exception //{{{1
{
   public:
      SDLError() throw() : sdlError(SDL_GetError()) {}
      virtual ~SDLError() throw() {}
      virtual const char* what() const throw() { return sdlError; }

   private:
      const char* sdlError;
};

class SDLInitialize //{{{1
{
   public:
      explicit SDLInitialize();
      ~SDLInitialize();
};

class Window : NonCopyable  //{{{1 
{
   public:
      Window(unsigned width, unsigned height, const char* title);
      operator SDL_Window*() throw() { return window; }
      void Update();
      ~Window() throw();

   private:
      SDL_Window* window;
};

class Renderer : NonCopyable //{{{1
{
   public:
      explicit Renderer(Window& window);
      operator SDL_Renderer*() { return renderer; }
      void SetColor(const SDL_Color& color);
      ~Renderer() throw();
   private:
      SDL_Renderer* renderer;
};

class BitmapSurface : NonCopyable //{{{1
{
   public:
      BitmapSurface(const char* filename);
      operator SDL_Surface*() { return surface; }
      operator const SDL_Surface*() const { return surface; }
      SDL_Surface *operator->() { return surface; }
      const SDL_Surface * const operator->() const { return surface; }
      virtual ~BitmapSurface() throw();

	private:
		SDL_Surface* surface;
};

class Texture : NonCopyable //{{{1
{
   public:
      explicit Texture() : texture(NULL) {}
      explicit Texture(Renderer& renderer, const SDL_Surface* const sourceSurface);
      operator SDL_Texture*() throw() { return texture; }
      void CreateFrom(Renderer& renderer, SDL_Surface* sourceSurface);
      virtual ~Texture() throw();

   private:
      SDL_Texture* texture;
};

}

// vim: foldmethod=marker
