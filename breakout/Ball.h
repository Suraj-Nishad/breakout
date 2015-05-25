#pragma once

#include "TextureGameObject.h"
#include "PNGImage.h"

class Ball : public TextureGameObject
{

public:
    static void LoadPNG(RenderEngine &engine);

    static Texture &Image() {return _g_ball_png;}

    static int PNGHeight() {return _g_ball_png.Height();}

    static int PNGWidth() {return _g_ball_png.Width();}

    virtual GAME_OBJECT_TYPE Type();

protected:
    static PNGImage _g_ball_png;

public:
    Ball(GameControler &game);

    virtual ~Ball(void);

    void SetInitialPosition(int x, int y);

    void Start();

    void Stop();

    bool NotMoving();

protected:

    virtual Texture * GetTexture();

    virtual void EndContact( IGameObject *another_object );

    void CheckVelocity();

    WeakCopyTexture _ball_png;
};

