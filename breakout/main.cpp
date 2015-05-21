#include <list>

#include "RenderEngine.h"
#include "PNGImage.h"
#include "PhysicsSimulator.h"
#include "GameArea.h"
#include "Ball.h"
#include "Paddle.h"

int main( int argc, char* argv[] )
{

    RenderEngine engine;
    PhysicsSimulator physics;
    GameArea game_area(physics, engine);

    PNGImage background("MCTestTaskBackground.png");
    background.Load(engine);

    Ball ball(engine, physics);
    Paddle paddle(engine, physics, game_area);

    bool quit = false;
    SDL_Event e;
    while (!quit)
	{
        std::list<IRenderElement *> textures;
        textures.push_back(&background);
        game_area.AddTexture(textures);
        ball.AddTexture(textures);
        paddle.AddTexture(textures);
        engine.Render(textures);
        physics.Step();

	    while(SDL_PollEvent(&e) != 0)
	    {
            switch(e.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEMOTION:
                paddle.SetX(e.motion.x);
            default:
                break;
            }
	    }
    }

    return 0;
}


