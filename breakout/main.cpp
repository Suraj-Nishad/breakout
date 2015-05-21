#include <list>

#include "RenderEngine.h"
#include "PNGImage.h"
#include "PhysicsSimulator.h"
#include "GameArea.h"
#include "Ball.h"

int main( int argc, char* argv[] )
{

    RenderEngine engine;
    engine.CreateWindow();
    PhysicsSimulator physics;
    GameArea game_area(physics, engine.Width(), engine.Height());

    Texture::MAX_X = engine.Width();
    Texture::MAX_Y = engine.Height();

    PNGImage background("MCTestTaskBackground.png");
    background.Load(engine);

    Ball ball(engine, physics);

    bool quit = false;
    SDL_Event e;
    while (!quit)
	{
        std::list<IRenderElement *> textures;
        textures.push_back(&background);
        game_area.AddTexture(textures);
        ball.AddTexture(textures);
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
            default:
                break;
            }
	    }
    }

    return 0;
}


