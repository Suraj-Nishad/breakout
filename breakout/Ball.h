#pragma once

#include "TextureGameObject.h"
#include "PNGImage.h"
#include <list>

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

    Ball(const Ball *another_ball);
    
    virtual ~Ball(void);

    void SetInitialPosition(int x, int y);

    void Start();

    void Stop();

    bool NotMoving();

    void GetVelocity(float &velocity, float &angle);

    void SetVelocity(float velocity, float angle);

    void Replicates(std::list<Ball *> &ball_list);

protected:

    void Initialize();

    virtual Texture * GetTexture();

    virtual void EndContact( IGameObject *another_object );

    void CheckVelocity();

    WeakCopyTexture _ball_png;
};

