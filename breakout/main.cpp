#include "RenderEngine.h"
#include "PNGImage.h"
#include <list>

int main( int argc, char* argv[] )
{
    RenderEngine engine;
    engine.CreateWindow();

    Texture::MAX_X = engine.Width();
    Texture::MAX_Y = engine.Height();

    PNGImage background("MCTestTaskBackground.png");
    background.Load(engine);

    PNGImage paddle("MCTestTaskPaddle.png");
    paddle.Load(engine);
    paddle.SetPosition(engine.Width() / 2, engine.Height() - 50);

    bool quit = false;
    SDL_Event e;
    while (!quit)
	{
        std::list<Texture *> textures;
        textures.push_back(&background);
        textures.push_back(&paddle);

        engine.Render(textures);

	    if(SDL_WaitEvent(&e) != 0)
	    {
            switch(e.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEMOTION:
                paddle.SetPosition(e.motion.x-paddle.Width()/2, paddle.Y());
                break;
            default:
                break;
            }
	    }
    }

    return 0;
}


