#include <cstdio>
#include "kwrgame.h"

static const SDL_Color BlackOpaque = { 0,0,0,SDL_ALPHA_OPAQUE};

int main(int argc, char* argv[])
{
    printf("Hello, SDL.\n");

    kwr::SimpleDrawWindow window(600, 400, BlackOpaque);
    window.Run();

    return 0;
}

