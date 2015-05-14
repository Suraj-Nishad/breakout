#pragma once
#include "SDL.h"
#include "RenderEngine.h"

class Texture
{
public:
    static int MAX_X, MAX_Y;

public:
    virtual ~Texture(void);

    inline int Width() {return _width;}
    inline int Height() {return _height;}
    void SetPosition(int x, int y);

    inline int X() {return _x;}
    inline int Y() {return _y;}


    inline bool IsValid() {return _texture != NULL;}

    void Free();

    void Show(RenderEngine &engine);

protected:
    Texture(void);

    SDL_Texture *_texture;

    int _width, _height;
    int _x, _y;
};
