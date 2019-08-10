#include "maze.h"

using namespace kwr;
using namespace kwr::game;

int main(int argc, char* args[])
{
    try {
        SDL_Library sdl_lib;
        ComplimentaryMultiplyWithCarry cmwc(710472);
        RandomUniform uniform(0, 2, &cmwc);
        MazeGrid maze(20, 20);

        Cell* run[20];

        for (int row = 0; row < maze.rows; ++row) {
            int runlen = 0;
            for (int col = 0; col < maze.columns; ++col) {
                Cell &cell = *maze.get(row, col);
                run[runlen++] = &cell;

                bool at_eastern_boundary = (cell.links[East].link == nullptr);
                bool at_northern_boundary = (cell.links[North].link == nullptr);

                uniform.next();
                bool close_out = at_eastern_boundary || (!at_northern_boundary && uniform.get());

                if (close_out) {
                    RandomUniform chooser(0, runlen, &cmwc);
                    chooser.next();
                    Cell* member = run[chooser.get()];
                    member->link(member->links[North].link);
                    runlen = 0;
                }
                else {
                    cell.link(cell.links[East].link);
                }
            }
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

