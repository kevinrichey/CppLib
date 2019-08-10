#include "kwrlib.h"
#include "kwrerr.h"
#include "kwrgame.h"
#include "kwrlegocolors.h"

using namespace std;
using namespace kwr;
using namespace kwr::game;

class DrawWindow : public GameDriver {
  public:
    DrawWindow(Dims dims, Font& f, CString t) : 
      GameDriver(dims, LegoColors::Black, "Draw Text"),
      font(f),
      text(t)
    {}

    void render() override
    {
        renderer.color = LegoColors::White;
        renderer.draw(text, {0,0}, font);
    }

  private:
    Font& font; 
    CString text;
};


// fonts/FreeSerif.ttf

int main(int argc, char* args[]) 
{
    try {
        SDL_Library sdl_lib;
        TTF_Library ttf_lib;
        //Trace::turn(off);

        auto out = OutStream::console();

        CString fontname("fonts/Inconsolata-Regular.ttf");
        int  fontsize = 24;
        Dims size {800,600};
        bool help = (argc == 1);

        // variable, type, arg name, default, description

        int i = 1;
        while (i < argc) {
            if (args[i][0] == '-') {

                if (args[i][1] == 'f')  fontname = args[++i];
                if (args[i][1] == 's')  fontsize = atoi(args[++i]);
                if (args[i][1] == 'w')  size.width = atoi(args[++i]);
                if (args[i][1] == 'h')  size.height = atoi(args[++i]);
                if (args[i][1] == '?')  help = true;

            }
            ++i;
        }

        if (help) {
            out.print("Display string from stdin in an SDL window.\n");
            out.print("  -f   Font filename (default Inconsolata-Regular).\n");
            out.print("  -s   Font size (default 24).\n");
            out.print("  -w   Window width (default 800).\n");
            out.print("  -h   Window height (default 600).\n");
            out.print("  -?   Print this help.\n");
            return 0;
        }

        Font font(fontname, fontsize);

        char text[5000];
        fgets(text, 5000, stdin);

        DrawWindow win(size, font, text);
        win.run();

    }
    catch(Error& error) {
        kwr_Trace(error.what);
        return 1;
    }

    return 0;
}

