#include <cstdio>
#include <limits>
#include "kwrlib.h"
#include "kwrsdl.h"
#include "kwrgame.h"

namespace kwr {

enum class ReturnCode { Success = 0, Error, Failure };


} // kwr namespace

using namespace kwr;

int main(int argc, char* args[]) 
{
    //kwr::Trace::turn(kwr::off);

    kwr_Scope("main()");
    kwr_Trace("Hello, world!");

    SDLInitialize sdl_init;

    SDL_Color background { 0, 0, 0, SDL_ALPHA_OPAQUE };
    SimpleDrawWindow win(1024, 768, background);
    win.Run();

    return static_cast<int>(kwr::ReturnCode::Success);
}

