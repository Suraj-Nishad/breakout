#include "RenderEngine.h"
#include "PNGImage.h"
#include <list>

int main( int argc, char* argv[] )
{
    RenderEngine engine;
    PNGImage background("MCTestTaskBackground.png");
      

    engine.CreateWindow();
    background.Load(engine);

    bool quit = false;
    SDL_Event e;
    while (!quit)
	{
        std::list<Texture *> textures;
        textures.push_back(&background);

        engine.Render(textures);

	    if(SDL_WaitEvent(&e) != 0)
	    {
            switch(e.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEMOTION:
                break;
            default:
                break;
            }
	    }
    }

    return 0;
}


