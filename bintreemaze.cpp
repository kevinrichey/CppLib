#include "maze.h"

using namespace kwr;
using namespace kwr::game;

int main(int argc, char* args[])
{
    try {
        SDL_Library sdl_lib;
        ComplimentaryMultiplyWithCarry cmwc(8423032);
        RandomUniform uniform(0, 2, &cmwc);
        MazeGrid maze(10, 10);

        for (int i = 0; i < maze.cells.size(); ++i) {
            Cell &cell = maze.cells[i];
            Cell* neighbors[2] = { cell.links[North].link, cell.links[East].link };
            Cell* neighbor = nullptr;

            if (neighbors[0] && neighbors[1]) {
                uniform.next();
                neighbor = neighbors[uniform.get()];
            }
            else if (neighbors[0]) {
                neighbor = neighbors[0];
            }
            else if (neighbors[1]) {
                neighbor = neighbors[1];
            }

            if (neighbor) cell.link(neighbor);
        }

        MazeWindow mazewin(&maze);
        mazewin.run();
    }
    catch(Error& error) {
        OutStream::error().print(error.what);
        return 1;
    }

    return 0;
}

