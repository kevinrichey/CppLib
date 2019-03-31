#ifndef KWR_HEADER_KWRGAME_H
#define KWR_HEADER_KWRGAME_H

#include "kwrsdl.h"

namespace kwr::game {

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
    explicit Sprite(Renderer& renderer, BitmapSurface& bitmap);
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

    GameDriver(Dims size, SDL_Color bg, CString title);

    void start();
    void run();
    void stop();

    virtual void setup();
    virtual void handle(const SDL_Event& event);
    virtual void update();
    virtual void clear();
    virtual void render();

  private:

    SDL_Library sdl_lib;
    TTF_Library ttf_lib;
    SDL_Color background;
    bool running = false;

  protected:

    Window window;
    Renderer renderer;
};

}  // kwr

#endif

