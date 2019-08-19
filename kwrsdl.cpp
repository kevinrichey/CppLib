#include "kwrsdl.h"
#include "kwrerr.h"

namespace kwr::game { 

void check(bool passes)
{
    if (!passes) throw Fault(kwr_FileLine, SDL_GetError(), "Fault");
}

void check(int code)
{
    if (code) throw Fault(kwr_FileLine, SDL_GetError(), "Fault");
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
    check(surface);
}

BitmapSurface::~BitmapSurface() throw()
{
    SDL_FreeSurface( surface );
}



Renderer::Renderer(Window& window) : 
  renderer( SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) ),
  color(*this)
{
    check(renderer);
}


Dims Renderer::size()
{
    Dims dims;
    check( SDL_GetRendererOutputSize(renderer, &dims.width, &dims.height) );
    return dims;
}

void Renderer::set(const SDL_Color& color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void Renderer::clear()
{
    SDL_RenderClear(renderer);
}

void Renderer::draw(SDL_Rect rect)
{
    check( SDL_RenderDrawRect(renderer, &rect) );
}

void Renderer::draw(class Texture& tex, SDL_Point pt)
{
    Dims size = tex.size();
    SDL_Rect rect { pt.x, pt.y, size.width, size.height };
    SDL_RenderCopy(renderer, tex.get(), NULL, &rect);
}

void Renderer::draw(SDL_Point p1, SDL_Point p2)
{
    check( SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y) );
}

void Renderer::draw(CString text, SDL_Point pt, Font& font)
{
    SDL_Color color;
    check( SDL_GetRenderDrawColor(renderer, &color.r, &color.g, &color.b, &color.a) );

    Surface surface(font.wrap(text, color, size().width - pt.x));
    Texture tex( textureFrom(surface) );
    draw(tex, pt);
}

void Renderer::fill(SDL_Rect rect)
{
    check( SDL_RenderFillRect(renderer, &rect) );
}

void Renderer::present()
{
    SDL_RenderPresent(renderer);
}

SDL_Texture* Renderer::textureFrom(Surface& surface)
{
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface.get());
    check(tex);
    return tex;
}

Renderer::~Renderer() throw()
{
    SDL_DestroyRenderer(renderer);
}

Surface::Surface(SDL_Surface* s) : surface(s) {}
Surface::~Surface() { SDL_FreeSurface(surface); }
SDL_Surface* Surface::get() { return surface; }


Texture::Texture(SDL_Texture* tex) : texture(tex) {}

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
  font(TTF_OpenFont(fontname.cstr(), size))
{
    check(font);
} 

Dims Font::size(CString text)
{
    Dims result;
    TTF_SizeUTF8(font, text.cstr(), &result.width, &result.height);
    return result;
}

Font::~Font()
{
    TTF_CloseFont(font);
}

SDL_Surface* Font::solid(CString text, SDL_Color color)
{
    SDL_Surface* surface = TTF_RenderUTF8_Solid(font, text.cstr(), color);
    check(surface);
    return surface;
}

SDL_Surface* Font::shade(CString text, SDL_Color forecol, SDL_Color backcol)
{
    SDL_Surface* surface = TTF_RenderUTF8_Shaded(font, text.cstr(), forecol, backcol);
    check(surface);
    return surface;
}

SDL_Surface* Font::blend(CString text, SDL_Color color)
{
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.cstr(), color);
    check(surface);
    return surface;
}

SDL_Surface* Font::wrap(CString text, SDL_Color color, int width)
{
  SDL_Surface* surface = TTF_RenderUTF8_Blended_Wrapped(font, text.cstr(), color, width);
  check(surface);
  return surface;
}


} // kwr::game

