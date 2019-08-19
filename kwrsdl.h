#ifndef KWR_HEADER_KWRSDL_H
#define KWR_HEADER_KWRSDL_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "kwrlib.h"

namespace kwr::game {

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

class Window : public Object {
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

class Surface : public Object {
  public:
    Surface(SDL_Surface* s);
    ~Surface();
    SDL_Surface* get();

    /// TODO: swap, move, dispose, reset, release

  private:
    SDL_Surface* surface = nullptr;
};

class Texture : public Object {
  public:
    Texture() = default;
    Texture(SDL_Texture* tex);
    SDL_Texture* get() { return texture; }
    Dims size();
    ~Texture() throw();

    /// TODO: swap, move, dispose, reset, release

  private:
    SDL_Texture* texture = nullptr;
};

class Font : public Object {
  public:
    Font(CString fontname, int size);
    Dims size(CString text);
    SDL_Surface* solid(CString text, SDL_Color color);
    SDL_Surface* shade(CString text, SDL_Color forecol, SDL_Color backcol);
    SDL_Surface* blend(CString text, SDL_Color color);
    SDL_Surface* wrap(CString text, SDL_Color color, int width);
    ~Font();

  private:
    TTF_Font* font {};
};

class Renderer : public Object {
  public:
    explicit Renderer(Window& window);
    SDL_Renderer* get() { return renderer; }
    Dims size();
    void set(const SDL_Color& color);
    void clear();
    void draw(Texture& tex, SDL_Point pt);
    void draw(SDL_Rect rect);
    void draw(SDL_Point p1, SDL_Point p2);
    void draw(CString text, SDL_Point pt, Font& font);
    void fill(SDL_Rect rect);
    void present();

    SDL_Texture* textureFrom(Surface& surface);

    ~Renderer();

    /// TODO: swap, move, dispose, reset, release

  private:
    SDL_Renderer* renderer;

  public:
    Property<Renderer, SDL_Color, &Renderer::set> color;

};

class BitmapSurface : public Object {
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

} // kwr::game

#endif
