#ifndef KWR_HEADER_KWRSDL_H
#define KWR_HEADER_KWRSDL_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "kwrlib.h"

namespace kwr::game {

class Error {
  public:
    Error(int code=0) : errCode(code), errMessage(SDL_GetError()) {}
    int     code() const { return errCode; }
    CString message() const { return errMessage; }

  private:
    int     errCode;
    CString errMessage;
};

void check(bool passes);
void check(int code);

template <typename T>
void check(T* pointer)
{
    check(pointer != nullptr);
}

class SDL_Library {
  public:
    SDL_Library();
    ~SDL_Library();
    /// TODO: Add sub-system inits.
};

class TTF_Library {
  public:
    TTF_Library();
    ~TTF_Library();
};

struct Dims { int width, height; };

class Window : public Complex {
  public:
    Window(Dims dimensions, CString title);
    SDL_Window* get() { return window; }
    void update();
    ~Window();

  private:
    SDL_Window* window;
};

template <class ClassType, class PropType, void(ClassType::*setter)(const PropType&)>
class Property {
  public:
    Property(ClassType& obj) : object(obj) {}
    void operator=(const PropType& color)
    {
        (object.*setter)(color);
    }
  private:
    ClassType& object;
};

class Renderer : public Complex {
  public:
    explicit Renderer(Window& window);
    SDL_Renderer* get() { return renderer; }
    void set(const SDL_Color& color);
    void clear();
    void draw(class Texture& tex, SDL_Point pt);
    void draw(SDL_Rect& rect);
    void present();
    ~Renderer();

    /// TODO: swap, move, dispose, reset, release

  private:
    SDL_Renderer* renderer;

  public:
    Property<Renderer, SDL_Color, &Renderer::set> color;

};

class BitmapSurface : public Complex {
  public:
    explicit BitmapSurface(const char* filename);
    operator SDL_Surface*() { return surface; }
    operator const SDL_Surface*() const { return surface; }
    SDL_Surface *operator->() { return surface; }
    const SDL_Surface * const operator->() const { return surface; }
    virtual ~BitmapSurface() throw();

  private:
    SDL_Surface* surface;
};

class Surface : public Complex {
  public:
    Surface(SDL_Surface* s);
    ~Surface();
    SDL_Surface* get();

    /// TODO: swap, move, dispose, reset, release

  private:
    SDL_Surface* surface = nullptr;
};

class Texture : public Complex {
  public:
    Texture() = default;
    Texture(Renderer& renderer, Surface& surface);
    Texture(Renderer& renderer, SDL_Surface* sourceSurface);
    SDL_Texture* get() { return texture; }
    void create(Renderer& renderer, Surface& surface);
    void CreateFrom(Renderer& renderer, SDL_Surface* sourceSurface);
    Dims size();
    ~Texture() throw();

    /// TODO: swap, move, dispose, reset, release

  private:
    SDL_Texture* texture = nullptr;
};

class Font : public Complex {
  public:
    Font(CString fontname, int size);
    ~Font();

    SDL_Surface* Render(CString text, SDL_Color color);

  private:
    TTF_Font* font {};
};

} // kwr::game

#endif
