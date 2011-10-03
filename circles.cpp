#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "SDL.h"

class MyGame
{
public:
	MyGame()
	{
		SDL_Init(SDL_INIT_VIDEO);
	}

	~MyGame()
	{
		SDL_Quit();
	}
};

class VideoError : std::exception
{
	private:
	const char* errorMessage;

	public:
	VideoError(const char* message)
		: errorMessage(message)
	{}

	const char* what() const throw()
	{
		return errorMessage;
	}
};

class Screen
{
	private:
	public:
	SDL_Surface* surface;

	Screen(int width, int height)
		: surface(SDL_SetVideoMode(width, height, 0, SDL_SWSURFACE))
	{
		if(!surface)
		{
			throw VideoError("Unable to set video mode.");
		}
	}

	void Lock()
	{
		if(SDL_MUSTLOCK(surface))
		{
			if(SDL_LockSurface(surface) < 0)
			{
				throw VideoError("Unable to lock surface.");
			}
		}
	}

	void Unlock()
	{
		if(SDL_MUSTLOCK(surface))
		{
			SDL_UnlockSurface(surface);
		}
	}

	Uint32* GetPixelAt(int x, int y)
	{
		return (Uint32*)surface->pixels + y*surface->pitch/4 + x;
	}
};

void fill_circle(SDL_Surface *surface, int cx, int cy, int radius, Uint32 pixel)
{
    // Note that there is more to altering the bitrate of this 
    // method than just changing this value.  See how pixels are
    // altered at the following web page for tips:
    //   http://www.libsdl.org/intro.en/usingvideo.html
    static const int BPP = 4;
 
    double r = (double)radius;
 
    for (double dy = 1; dy <= r; dy += 1.0)
    {
        // This loop is unrolled a bit, only iterating through half of the
        // height of the circle.  The result is used to draw a scan line and
        // its mirror image below it.
 
        // The following formula has been simplified from our original.  We
        // are using half of the width of the circle because we are provided
        // with a center and we need left/right coordinates.
 
        double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
        int x = cx - dx;
 
        // Grab a pointer to the left-most pixel for each half of the circle
        Uint8 *target_pixel_a = (Uint8 *)surface->pixels + ((int)(cy + r - dy)) * surface->pitch + x * BPP;
        Uint8 *target_pixel_b = (Uint8 *)surface->pixels + ((int)(cy - r + dy)) * surface->pitch + x * BPP;
 
        for (; x <= cx + dx; x++)
        {
            *(Uint32 *)target_pixel_a = pixel;
            *(Uint32 *)target_pixel_b = pixel;
            target_pixel_a += BPP;
            target_pixel_b += BPP;
        }
    }
}

void DrawCircle(Screen& screen, int cx, int cy, int radius)
{
	Uint32 color = SDL_MapRGB(screen.surface->format, 255, 255, 255);
	int radiusSq = radius*radius;
	int top = cy-radius, bottom=cy+radius;

	Uint32* pix = screen.GetPixelAt(cx,top);
	for(int y=radius; y>=-radius; --y)
	{
		int ySqr = y*y;
		Uint32 *right = pix;
		Uint32 *left = right;
		for(int x=0; (x*x + ySqr) < radiusSq; ++x)
		{
			*left-- = *right++ = color;
		}
		pix += screen.surface->pitch/4;
	}

}

template<class T>
class Range
{
	private:
	T minimum, maximum;

	public:
	Range(T min, T max) : minimum(min), maximum(max) {}
	T Random() const
	{
		return rand() % Length() + minimum;
	}
	T Length() const
	{
		return maximum - minimum + 1;
	}
};

int main()
{
	MyGame mygame;

	try
	{
		Screen screen(640, 480);
		std::cout << "BPP: " << (int)screen.surface->format->BitsPerPixel << std::endl;
		std::cout << "Must lock? " << SDL_MUSTLOCK(screen.surface) << std::endl;

		SDL_Event event;
		bool running = true;
		while (running)
		{
			if (SDL_PollEvent(&event))
			{
				switch(event.type)
				{
					case SDL_QUIT:
						running = false;
						break;
				}
			}

			int pixel = SDL_MapRGB(screen.surface->format,
				Range<int>(0,255).Random(),
				Range<int>(0,255).Random(),
				Range<int>(0,255).Random());

			int x = Range<int>(26,620).Random();
			int y = Range<int>(26,460).Random();
			
			screen.Lock();
			fill_circle(screen.surface, x, y, 25, pixel);
			screen.Unlock();
			SDL_Flip(screen.surface);
		}
	}
	catch(VideoError &error)
	{
		std::cout << error.what() << std::endl;
	}

	return 0;
}

