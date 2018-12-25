#ifndef KWR_HEADER_KWRGAME_H
#define KWR_HEADER_KWRGAME_H

#include "kwrsdl.h"

namespace kwr {

struct HitBox {

    SDL_Rect rect;

    HitBox() =default;
    explicit HitBox(const SDL_Rect &r) : rect(r) {}

    int Left() const   { return rect.x; }
    int Right() const  { return rect.x + rect.w; }
    int Top() const    { return rect.y; }
    int Bottom() const { return rect.y + rect.h; }
    int Width() const  { return rect.w; }
    int Height() const { return rect.h; }

    HitBox MovedBy(int x, int y) const;
    bool CollidesWith(const HitBox &box) const;
};

class Sprite {
  public:
    explicit Sprite(Renderer& renderer, const BitmapSurface& bitmap);
    explicit Sprite(Renderer& renderer, const char* filename);
    void MoveTo(int x, int y);
    void Move();
    void SetVelocityX(int v);
    void SetVelocityY(int v);
    void Draw(Renderer& renderer);
    bool WouldHit(const Sprite& target) const;
    virtual ~Sprite() throw();

  private:
    Texture texture;
    HitBox hitbox;
    int dx, dy;
};

class GameDriver {

  public:

    explicit GameDriver(const SDL_Color &bg);
    explicit GameDriver(unsigned w, unsigned h, const SDL_Color &bg);

    void Start();
    void Run();
    void Stop();

    virtual void Setup();
    virtual void HandleEvent(const SDL_Event& event);
    virtual void Update();
    virtual void Clear();
    virtual void Render();

  private:

    SDLInitialize sdlInit;
    bool running;
    SDL_Color background;

  protected:

    Window window;
    Renderer renderer;
};

class SimpleDrawWindow : public GameDriver {

  public:

    SimpleDrawWindow();
    SimpleDrawWindow(int width, int height, const SDL_Color &bg);
    virtual void Setup();
    virtual void HandleEvent(const SDL_Event& event);
    virtual void Update();

};

}  // kwr

#endif

