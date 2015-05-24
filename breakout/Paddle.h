#pragma once

#include "GameControler.h"
#include "GameObject.h"
#include "PNGImage.h"

class Paddle : public GameObject
{
public:
    Paddle(GameControler &game);

    virtual ~Paddle(void);

    void SetX(int x);

    void GetCenterPoint(int &x, int &y);

protected:

    virtual Texture * GetTexture();

protected:
    PNGImage _paddle_png;

    int _min_x, _max_x;
};

