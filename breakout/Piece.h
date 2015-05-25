#pragma once
#include "TextureGameObject.h"
#include "PNGImage.h"
#include "Texture.h"

class Bonus;
class Piece : public TextureGameObject
{
friend Bonus;
public:
    static void LoadPNG(RenderEngine &engine);

    static Texture &Image() {return _g_piece_png;}

    static int PNGHeight() {return _g_piece_png.Height();}

    static int PNGWidth() {return _g_piece_png.Width();}

    virtual GAME_OBJECT_TYPE Type();

    inline bool HasBonus() {return false;}

protected:
    static PNGImage _g_piece_png;

public:
    Piece(GameControler &game, int x, int y, int max_width);

    virtual ~Piece(void);

    int Width() {return _piece_texture.Width();}

    virtual Texture * GetTexture();

protected:
    virtual void EndContact( IGameObject *another_object );

    WeakCopyTexture _piece_texture;
};


