#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <string>

class RenderEngine
{
public:
	RenderEngine();
	virtual ~RenderEngine(void);

	bool CreateWindow(const std::string &title = "Breakout",
                      int width = 640,
                      int height = 480);

    void SetBackgroundImage(const std::string &file);

    void Render();

protected:
    SDL_Texture *LoadPNGImage(const std::string &file);

private:
	SDL_Window *_window;
    SDL_Renderer *_renderer;
    SDL_Texture *_background;
};

