#include <cstdio>
#include <limits>
#include <cmath>
#include "kwrlib.h"
#include "kwrerr.h"
#include "kwrsdl.h"
#include "kwrgame.h"
#include "kwrlegocolors.h"

using namespace kwr;
using namespace kwr::game;

SDL_Color black   {   0,   0,   0, SDL_ALPHA_OPAQUE };


class FontWindow : public GameDriver {
  public:
    FontWindow() : GameDriver( {800, 600}, black, "Hello, World") {}

    Font libsans { "fonts/LiberationSans-Regular.ttf", 32 };

    CString helloworld { "Hello, World!" };
    Dims    size  { libsans.size(helloworld) };
    Surface hellosurf { libsans.solid(helloworld, LegoColors::White) };
    Texture hellotex { renderer.textureFrom(hellosurf) };

    CString nightmoon { "Goodnight, Moon." };
    Surface nightsurf { libsans.shade(nightmoon, LegoColors::Bright_Blue, LegoColors::Grey) };
    Texture nighttex  { renderer.textureFrom(nightsurf) };

    CString brightsun { "Bright Sun" };
    Surface sunsurf   { libsans.blend(brightsun, LegoColors::Bright_Orange) };
    Texture suntex    { renderer.textureFrom(sunsurf) };

    SDL_Color rgbcolor = sdlcol( rgb({ 140.0, 0.55, 0.7647 }) );
    CString comp  { "This is Light Green" };
    Surface hslsurf  { libsans.blend(comp, rgbcolor) };
    Texture hsltex   { renderer.textureFrom(hslsurf) };

    CString sentence { "This will be a longer string of text\nwith a newline in the middle." };

    void setup() override
    {
    }

    void render() override
    {
        SDL_Rect rect { 100, 100, size.width, size.height };
        renderer.color = LegoColors::Bright_Red;
        renderer.draw(rect);
        renderer.draw(hellotex, {100,100});
        renderer.draw(nighttex, {100,200});
        renderer.color = LegoColors::Dark_Green;
        renderer.fill({100,300, suntex.size().width,suntex.size().height});
        renderer.draw(suntex,   {100,300});
        renderer.draw(hsltex,  {500, 300});
        renderer.color = LegoColors::Medium_Reddish_Violet;
        renderer.draw({500,350}, {750,400});
        renderer.color = LegoColors::Bright_Purple;
        renderer.draw(sentence, {500,50}, libsans);
    }

    ~FontWindow()
    {
    }

};

int main(int argc, char* args[]) 
{
    try {
        SDL_Library sdl_lib;
        TTF_Library ttf_lib;
        Trace::turn(off);
        FontWindow win;
        win.run();
    }
    catch(Error& error) {
        OutStream::error().print(error.what);
        return 1;
    }

    return 0;
}

