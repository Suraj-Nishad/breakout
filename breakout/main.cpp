#include <list>

#include "RenderEngine.h"
#include "PNGImage.h"
#include "PhysicsSimulator.h"
#include "GameControler.h"
#include "Ball.h"
#include "Paddle.h"

int main( int argc, char* argv[] )
{

    srand(SDL_GetTicks());

    GameControler game;

    bool quit = false;
    SDL_Event e;
    while (!quit)
	{
	    if(SDL_WaitEvent(&e) != 0)
	    {
            switch(e.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEMOTION:
                game.SetMouseX(e.motion.x);
                break;
            case SDL_MOUSEBUTTONUP:
                game.MouseClick();
                break;
            case SDL_USEREVENT:
                game.HandleUserEvent(e.user);
                break;
            case SDL_KEYUP:
                if(e.key.keysym.sym == SDLK_ESCAPE)
                    game.BallOutOfGame();
                break;
            default:
                break;
            }
	    }
    }

    return 0;
}


