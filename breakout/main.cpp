#include "RenderEngine.h"

int main( int argc, char* argv[] )
{
    RenderEngine engine;

    engine.CreateWindow();
    engine.SetBackgroundImage("MCTestTaskBackground.png");

    bool quit = false;
    SDL_Event e;
    while (!quit)
	{
        engine.Render();

	    if(SDL_WaitEvent(&e) != 0)
	    {
		    if(SDL_QUIT == e.type)
			    quit = true;
	    }
    }

    return 0;
}


