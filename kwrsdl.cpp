#include "kwrsdl.h"

namespace kwr { 


SDLInitialize::SDLInitialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw SDLError();
    }
}

SDLInitialize::~SDLInitialize()
{
    SDL_Quit();
}


Window::Window(int width, int height, const char* title)
{
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        throw SDLError();
    }
}

void Window::Update()
{
    SDL_UpdateWindowSurface(window);
}

Window::~Window()
{
    SDL_DestroyWindow(window);
}



BitmapSurface::BitmapSurface(const char* filename) :
  surface(SDL_LoadBMP(filename))
{
    if (!surface) {
        throw SDLError();
    }
}

BitmapSurface::~BitmapSurface() throw()
{
    SDL_FreeSurface( surface );
}



Renderer::Renderer(Window& window)
{
    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        throw SDLError();
    }
}

void Renderer::SetColor(const SDL_Color& color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

Renderer::~Renderer() throw()
{
    SDL_DestroyRenderer(renderer);
}


Texture::Texture(Renderer& renderer, const SDL_Surface* const sourceSurface) :
  texture(NULL)
{
    CreateFrom(renderer, const_cast<SDL_Surface*>(sourceSurface));
}

void Texture::CreateFrom(Renderer& renderer, SDL_Surface* sourceSurface)
{
    // TODO: require texture is NULL
    texture = SDL_CreateTextureFromSurface( renderer, sourceSurface );
    // TODO: ensure texture is not NULL
    if (!texture) {
        throw SDLError();
    }
}

Texture::~Texture()
{
    SDL_DestroyTexture(texture);
}

} // kwr

