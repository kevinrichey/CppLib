#include "kwrlib.h"
#include "kwrsdl.h"
#include "kwrgame.h"
#include "kwrerr.h"
#include "kwrlegocolors.h"
#include "kwrprng.h"

namespace kwr {
using namespace kwr::game;

class Cell;

class CellLink {
  public:
    Cell* link = nullptr;
    bool  open = false;
};

const int North = 0;
const int East  = 1;
const int West  = 2;
const int South = 3;

class Cell
{
  public:
    void init(int r, int c) { row = r; column = c; }

    void link(Cell* cell, bool bidi = true)
    {
        if (!cell) return;
        for (int i = 0; i < 4; ++i) {
            if (links[i].link == cell) {
                links[i].open = true;
                break;
            }
        }
        if (bidi) cell->link(this, false);
    }

    int row, column;
    CellLink links[4];
};

class MazeGrid {
  public:
    MazeGrid(int rs, int cs) : 
      rows(rs), columns(cs), cells(rows*columns)
    {
        prepare();
        configure();
    }

    void prepare()
    {
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < columns; ++c) {
                get(r, c)->init(r, c); 
            }
        }    
    }

    void configure()
    {
        for (int i = 0; i < cells.size(); ++i) {
            Cell& cell = cells[i];
            cell.links[North].link = get(cell.row-1, cell.column);
            cell.links[South].link = get(cell.row+1, cell.column);
            cell.links[West].link  = get(cell.row,   cell.column-1);
            cell.links[East].link  = get(cell.row,   cell.column+1);
        }
    }

    Cell* get(int r, int c)
    {
        if (r < 0 || r >= rows) return nullptr;
        if (c < 0 || c >= columns) return nullptr;
        return &cells[r*columns + c];
    }

    int rows, columns;
    Array<Cell> cells;
};

class MazeWindow : public GameDriver {
  public:
    MazeWindow(MazeGrid* m) : 
      GameDriver( {800, 800}, LegoColors::Black, "Maze"),
      maze(m)
    {}

    void render() override
    {
        int cell_size = 750 / maze->rows;
        int margin = 25;
        SDL_Color wall_color = LegoColors::White;
        renderer.color = wall_color;

        for (int i = 0; i < maze->cells.size(); ++i) {
            Cell& cell = maze->cells[i];
            int x1 = cell.column * cell_size + margin;
            int y1 = cell.row * cell_size + margin;
            int x2 = (cell.column+1) * cell_size + margin;
            int y2 = (cell.row+1) * cell_size + margin;

            if (cell.row == 0) renderer.draw({x1,y1}, {x2,y1});
            if (cell.column == 0) renderer.draw({x1,y1}, {x1,y2});
            if (!cell.links[East].open)   renderer.draw({x2,y1}, {x2,y2});
            if (!cell.links[South].open)  renderer.draw({x1,y2}, {x2,y2});
        }
    }
    
  private:
    MazeGrid* maze;
};

} // kwr
