#ifndef KWR_HEADER_KWRSDL_H
#define KWR_HEADER_KWRSDL_H

#include <exception>
#include <SDL.h>

namespace kwr {

class SDLError : public std::exception {
  public:
    SDLError() throw() : sdlError(SDL_GetError()) {}
    virtual ~SDLError() throw() {}
    virtual const char* what() const throw() { return sdlError; }

  private:
    const char* sdlError;
};

class SDLInitialize {
  public:
    explicit SDLInitialize();
    ~SDLInitialize();
};

class Window {
  public:
    Window(int width, int height, const char* title);
    operator SDL_Window*() throw() { return window; }
    void Update();
    ~Window() throw();

    explicit Window(const Window&) =delete;
    Window& operator= (const Window&) =delete;

  private:
    SDL_Window* window;
};

class Renderer {
  public:
    explicit Renderer(Window& window);
    operator SDL_Renderer*() { return renderer; }
    void SetColor(const SDL_Color& color);
    ~Renderer() throw();

    explicit Renderer(const Renderer&) =delete;
    Renderer& operator=(const Renderer&) =delete;

  private:
    SDL_Renderer* renderer;
};

class BitmapSurface {
  public:
    explicit BitmapSurface(const char* filename);
    operator SDL_Surface*() { return surface; }
    operator const SDL_Surface*() const { return surface; }
    SDL_Surface *operator->() { return surface; }
    const SDL_Surface * const operator->() const { return surface; }
    virtual ~BitmapSurface() throw();

    explicit BitmapSurface(const BitmapSurface&) =delete;
    BitmapSurface& operator=(const BitmapSurface&) =delete;

  private:
    SDL_Surface* surface;
};

class Texture {
  public:
    explicit Texture() : texture(NULL) {}
    explicit Texture(Renderer& renderer, const SDL_Surface* const sourceSurface);
    operator SDL_Texture*() throw() { return texture; }
    void CreateFrom(Renderer& renderer, SDL_Surface* sourceSurface);
    virtual ~Texture() throw();

  private:
    SDL_Texture* texture;
};

} // kwr

#endif
