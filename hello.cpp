#include <cstdio>
#include <limits>
#include "kwrlib.h"
#include "kwrsdl.h"
#include "kwrgame.h"

using namespace kwr;
using namespace kwr::game;

SDL_Color white { 255, 255, 255, SDL_ALPHA_OPAQUE };
SDL_Color red   { 255,   0,   0, SDL_ALPHA_OPAQUE };
SDL_Color black { 0, 0, 0, SDL_ALPHA_OPAQUE };

class FontWindow : public GameDriver {
  public:
    FontWindow() : GameDriver( {800, 600}, black, "Font Test") {}

    ~FontWindow()
    {
    }

    void setup() override
    {
        Surface surface { arialFont.Render("Hello, World!", white) };
        hellotex.create(renderer, surface);
    }

    void render() override
    {
        Dims size = hellotex.size();
        SDL_Rect rect { 300, 100, size.width, size.height };
        renderer.color = red;
        renderer.draw(rect);
        renderer.draw(hellotex, { 300, 100 });
    }

    Font arialFont { "fonts/arial.ttf", 25 };
    Texture hellotex;
};

int main(int argc, char* args[]) 
{
    Trace::turn(off);

    kwr_Scope("main()");
    kwr_Trace("Hello, world!");

    FontWindow win;
    win.run();

    return 0;
}

