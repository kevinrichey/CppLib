#include "kwrsdl.h"

namespace kwr { //{{{1

//{{{2  SDLInitialize

SDLInitialize::SDLInitialize()
{
   if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) throw SDLError();
}

SDLInitialize::~SDLInitialize()
{
   SDL_Quit();
}

//{{{2 Window

Window::Window(unsigned width, unsigned height, const char* title)
{
   window = SDL_CreateWindow( title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN );
   if( window == NULL ) throw SDLError();
}

void Window::Update()
{
   SDL_UpdateWindowSurface( window );
}

Window::~Window()
{
   SDL_DestroyWindow( window );
}

//{{{2 BitmapSurface

BitmapSurface::BitmapSurface(const char* filename)
	: surface( SDL_LoadBMP(filename) )
{
   if( !surface ) throw SDLError();
}

BitmapSurface::~BitmapSurface() throw()
{
	SDL_FreeSurface( surface );
}

//{{{2 Renderer

Renderer::Renderer(Window& window)
{
   renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
   if( renderer == NULL ) throw SDLError();
}

Renderer::~Renderer() throw()
{
   SDL_DestroyRenderer( renderer );
}

//{{{2 Texture

Texture::Texture(Renderer& renderer, SDL_Surface* sourceSurface)
   : texture( NULL )
{
   CreateFrom(renderer, sourceSurface);
}

void Texture::CreateFrom(Renderer& renderer, SDL_Surface* sourceSurface)
{
   // TODO: require texture is NULL
   texture = SDL_CreateTextureFromSurface( renderer, sourceSurface );
   // TODO: ensure texture is not NULL
   if( !texture) throw SDLError();
}

Texture::~Texture()
{
   SDL_DestroyTexture( texture );
}

} //}}}1

