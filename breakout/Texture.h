#pragma once
#include "SDL.h"
#include "RenderEngine.h"

class Texture
{
public:
    virtual ~Texture(void);

    inline int Width() {return _width;}
    inline int Height() {return _height;}
    inline int SetPosition(int x, int y) {_x = x; _y = y;}
    inline bool IsValid() {return _texture != NULL;}

    void Free();

    void Show(RenderEngine &engine);

protected:
    Texture(void);

    SDL_Texture *_texture;

    int _width, _height;
    int _x, _y;
};
