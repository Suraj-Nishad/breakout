#pragma once

#include "GameObject.h"
#include "PNGImage.h"

class Ball : public GameObject
{
public:
    Ball(GameControler &game);

    virtual ~Ball(void);

    void SetInitialPosition(int x, int y);

    void Start();

    void Stop();

    bool NotMoving();

protected:

    virtual Texture * GetTexture();

    virtual void EndContact( IContactObject *another_object );

    void CheckVelocity();

    PNGImage _ball_png;
};

