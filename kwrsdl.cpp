#include "kwrsdl.h"

namespace kwr::game { 

void check(bool passes)
{
    if (!passes) throw Error();
}

void check(int code)
{
    check(code == 0);
}

SDL_Library::SDL_Library()
{
    check( SDL_Init(SDL_INIT_VIDEO) );
}

SDL_Library::~SDL_Library()
{
    SDL_Quit();
}


TTF_Library::TTF_Library()
{
    check( TTF_Init() );
}

TTF_Library::~TTF_Library()
{
    TTF_Quit();
}


Window::Window(Dims dimensions, CString title) :
  window( SDL_CreateWindow(title.cstr(), 
                           SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                           dimensions.width, dimensions.height, SDL_WINDOW_SHOWN) )
{
    check(window);
}

void Window::update()
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
        throw Error();
    }
}

BitmapSurface::~BitmapSurface() throw()
{
    SDL_FreeSurface( surface );
}



Renderer::Renderer(Window& window) : 
  renderer( SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) ),
  color(*this)
{
    //renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        throw Error();
    }
}

void Renderer::set(const SDL_Color& color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void Renderer::clear()
{
    SDL_RenderClear(renderer);
}

void Renderer::draw(SDL_Rect& rect)
{
    SDL_RenderDrawRect(renderer, &rect);
}

void Renderer::draw(class Texture& tex, SDL_Point pt)
{
    Dims size = tex.size();
    SDL_Rect rect { pt.x, pt.y, size.width, size.height };
    SDL_RenderCopy(renderer, tex.get(), NULL, &rect);
}

void Renderer::present()
{
    SDL_RenderPresent(renderer);
}

Renderer::~Renderer() throw()
{
    SDL_DestroyRenderer(renderer);
}

Surface::Surface(SDL_Surface* s) : surface(s) {}
Surface::~Surface() { SDL_FreeSurface(surface); }
SDL_Surface* Surface::get() { return surface; }

Texture::Texture(Renderer& renderer, Surface& surface)
{
    create(renderer, surface);
}

Texture::Texture(Renderer& renderer, SDL_Surface* sourceSurface) :
  texture(NULL)
{
    CreateFrom(renderer, sourceSurface);
}

void Texture::create(Renderer& renderer, Surface& surface)
{
    // TODO: require texture is NULL
    check( texture = SDL_CreateTextureFromSurface( renderer.get(), surface.get() ) );
}

void Texture::CreateFrom(Renderer& renderer, SDL_Surface* sourceSurface)
{
    // TODO: require texture is NULL
    texture = SDL_CreateTextureFromSurface( renderer.get(), sourceSurface );
    // TODO: ensure texture is not NULL
    if (!texture) {
        throw Error();
    }
}

Texture::~Texture()
{
    SDL_DestroyTexture(texture);
}

Dims Texture::size() 
{
    Dims result;
    check( SDL_QueryTexture(texture, NULL, NULL, &result.width, &result.height) );
    return result;
}

Font::Font(CString fontname, int size) : 
  font(TTF_OpenFont(fontname.cstr(), 25))
{
    if (!font) throw Error();
} 

Font::~Font()
{
    TTF_CloseFont(font);
}

SDL_Surface* Font::Render(CString text, SDL_Color color)
{
    return TTF_RenderText_Solid(font, text.cstr(), color);
}

} // kwr::game

