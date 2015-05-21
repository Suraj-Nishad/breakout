#include <list>

#include "RenderEngine.h"
#include "PNGImage.h"
#include "PhysicsSimulator.h"
#include "GameArea.h"
#include "Ball.h"
#include "Paddle.h"

Uint32 TimerCallback(Uint32 interval, void *param)
{
    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = 0;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return(interval);
}

int main( int argc, char* argv[] )
{
    GameArea game_area;

    SDL_AddTimer(1, TimerCallback, &game_area);

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
                game_area.SetMouseX(e.motion.x);
                break;
            case SDL_USEREVENT:
                game_area.Step();
                break;
            default:
                break;
            }
	    }
    }

    return 0;
}


