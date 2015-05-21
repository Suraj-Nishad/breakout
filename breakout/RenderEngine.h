#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include <list>

class IRenderElement;

class RenderEngine
{
public:
	RenderEngine();
	virtual ~RenderEngine(void);

	bool CreateWindow(const std::string &title = "Breakout",
                      int width = 640,
                      int height = 480);

    void Render(const std::list<IRenderElement *> &textures);

    inline SDL_Renderer *Handle() {return _renderer;}

    inline int Width() {return _width;}
    inline int Height() {return _height;}

private:
	SDL_Window *_window;
    SDL_Renderer *_renderer;
    int _width, _height;
};

