#pragma once

#include "GameControler.h"
#include "TextureGameObject.h"
#include "PNGImage.h"

class Paddle : public TextureGameObject
{
public:
    Paddle(GameControler &game);

    virtual ~Paddle(void);

    void SetX(int x);

    void GetCenterPoint(int &x, int &y);

    virtual GAME_OBJECT_TYPE Type();

protected:

    virtual Texture * GetTexture();

protected:
    PNGImage _paddle_png;

    int _min_x, _max_x;
};

