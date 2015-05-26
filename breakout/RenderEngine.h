#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include <vector>

class IRenderElement;

class RenderEngine
{
public:
	RenderEngine();

    void CaptureMouseCursor();

    virtual ~RenderEngine(void);

    void ReleaseMouseCursor();

	bool CreateWindow(const std::string &title = "Breakout",
                      int width = 640,
                      int height = 480);

    void Render(const std::vector<IRenderElement *> &textures);

    inline SDL_Renderer *Handle() {return _renderer;}

    inline int Width() {return _width;}
    inline int Height() {return _height;}

private:
	SDL_Window *_window;
    SDL_Renderer *_renderer;
    int _width, _height;
};

