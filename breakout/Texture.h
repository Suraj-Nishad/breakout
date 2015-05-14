#pragma once
#include "SDL.h"
#include "RenderEngine.h"

class WeakCopyTexture;

class Texture
{
friend WeakCopyTexture;

public:
    static int MAX_X, MAX_Y;

public:
    virtual ~Texture(void);

    inline int Width() {return _width;}
    inline void Width(int w) {_width = w;}
    inline int Height() {return _height;}
    inline void Height(int h) {_height = h;}
    void SetPosition(int x, int y);
    void SetColor(Uint8 r, Uint8 g, Uint8 b);

    inline int X() {return _x;}
    inline int Y() {return _y;}

    inline bool IsValid() {return GetTexture() != NULL;}

    void Show(RenderEngine &engine);

protected:
    Texture(void);
    Texture(const Texture &another_texture);
    virtual SDL_Texture *GetTexture() = 0;

    int _width, _height;
    int _x, _y;
    
    //texture color
    bool _has_color;
    Uint8 _r, _g, _b;
};

class WeakCopyTexture : public Texture
{
public:
    WeakCopyTexture(Texture &another_texture);

protected:
    virtual SDL_Texture *GetTexture();
    Texture *_original_texture;
};


