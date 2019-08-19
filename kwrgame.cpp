#include "kwrgame.h"
#include "kwrerr.h"

namespace kwr::game {

RGB rgb(HSL hsl)
{
    double a = hsl.saturation * std::min(hsl.luminance, 1.0-hsl.luminance);

    auto f = [hsl, a](double n) -> double 
    {
        double k = fmod(n + hsl.hue/30.0, 12.0);
        return hsl.luminance - (a * std::max( min(k-3.0, 9.0-k, 1.0), -1.0));
    };

    return RGB { f(0), f(8), f(4) };
}

SDL_Color sdlcol(RGB rgb)
{
    return { 
        (Uint8)(rgb.red   * 255.0),
        (Uint8)(rgb.green * 255.0),
        (Uint8)(rgb.blue  * 255.0)
    };
}

HitBox HitBox::MovedBy(int x, int y) const
{
    return HitBox( SDL_Rect { Left()+x, Top()+y, Width(), Height() } );
}

bool HitBox::CollidesWith(const HitBox& target) const
{
    return 
      !( Left()   > target.Right()  ||
         Right()  < target.Left()   ||
         Top()    > target.Bottom() ||
         Bottom() < target.Top() );
}

//---------------------------------------------------------------------- 
//       Sprite class

Sprite::Sprite(Renderer& renderer, BitmapSurface& bitmap) :
  //texture(renderer, bitmap),
  hitbox( SDL_Rect { 0, 0, bitmap->w, bitmap->h } ),
  dx(0), dy(0)
{
}

void Sprite::MoveTo(int x, int y)
{
    hitbox.rect.x = x;
    hitbox.rect.y = y;
}

void Sprite::Move()
{
    hitbox.rect.x += dx;
    hitbox.rect.y += dy;
}

void Sprite::SetVelocityX(int v)
{
    dx = v;
}

void Sprite::SetVelocityY(int v)
{
    dy = v;
}

bool Sprite::WouldHit(const Sprite& target) const
{
    return hitbox.MovedBy(dx, dy).CollidesWith(target.hitbox);
}

void Sprite::Draw(Renderer& renderer)
{
    SDL_RenderCopy( renderer.get(), texture.get(), NULL, &hitbox.rect );
}

Sprite::~Sprite() throw()
{
}

//----------------------------------------------------------------------
//       GameDriver class

GameDriver::GameDriver(Dims size, SDL_Color bg, CString title) :
  background(bg),
  window(size, title),
  renderer(window)
{
}

void GameDriver::start()
{
    running = true;
}

void GameDriver::run()
{
    try {
        setup();
        start();

        while (running) {

            SDL_Event e;
            while (SDL_PollEvent(&e) != 0) {
                handle(e);
            }

            update();
            clear();
            render();

            renderer.present();
        }

    }
    catch(Error& error) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", error.what.cstr(), NULL);
    }
}

void GameDriver::stop()
{
    running = false;
}

void GameDriver::setup()
{
}

void GameDriver::handle(const SDL_Event& event)
{
    if (event.type == SDL_QUIT) {
        stop();
    }
}

void GameDriver::clear()
{
    renderer.color = background;
    renderer.clear();
}

void GameDriver::render()
{
}

void GameDriver::update()
{
}

} // kwr::game

