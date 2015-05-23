#pragma once
#include "gameobject.h"
#include "PNGImage.h"
#include "Texture.h"

class Piece : public GameObject
{
public:
    static void LoadPNG(RenderEngine &engine);

    static int PNGHeight() {return _g_piece_png.Height();}

    static int PNGWidth() {return _g_piece_png.Width();}

protected:
    static PNGImage _g_piece_png;

public:
    Piece(GameArea &game, int x, int y, int max_width);

    virtual ~Piece(void);

    int Width() {return _piece_texture.Width();}

    inline bool Destroyed() {return _destroyed;}

protected:
    virtual Texture * GetTexture();

    virtual void EndContact( IContactObject *another_object );

    WeakCopyTexture _piece_texture;
    bool _destroyed;
};


