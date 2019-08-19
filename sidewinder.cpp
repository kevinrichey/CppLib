#include "maze.h"

using namespace kwr;
using namespace kwr::game;

BString BinaryTreeAlgo("binarytree");
BString SidewinderAlgo("sidewinder");

struct MazeOptions : public Options {
    kwr_Attrib(algo, BString, SidewinderAlgo);
    kwr_Attrib(seed, int, 1);
    kwr_Attrib(rows, int, 20);
    kwr_Attrib(columns, int, 20);

    void set(const Argument& arg)
    {
        if      (arg.name == seed.name)     seed.set(arg.value);
        else if (arg.name == rows.name)     rows.set(arg.value);
        else if (arg.name == columns.name)  columns.set(arg.value);
        else if (arg.name == algo.name)     algo.set(arg.value);
    }
};

void BinaryTreeMaze(MazeGrid& maze, MazeOptions& config)
{
    ComplimentaryMultiplyWithCarry cmwc(config.seed);
    RandomUniform uniform(0, 2, &cmwc);

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
}

void SidewinderMaze(MazeGrid& maze, MazeOptions& config)
{
    ComplimentaryMultiplyWithCarry cmwc(config.seed);
    RandomUniform uniform(0, 2, &cmwc);
    Array<Cell*> run(config.columns);

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
}

int main(int argc, char* args[])
{
    try {
        MazeOptions config;
        config.getargs(argc, args);

        MazeGrid maze(config.rows, config.columns);

        if (config.algo == BinaryTreeAlgo)       BinaryTreeMaze(maze, config);
        else if (config.algo == SidewinderAlgo)  SidewinderMaze(maze, config);

        SDL_Library sdl_lib;
        MazeWindow mazewin(&maze);
        mazewin.run();
    }
    catch(Error& error) {
        OutStream::error().print(error.what);
        return 1;
    }

    return 0;
}

