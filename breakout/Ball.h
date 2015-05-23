#pragma once

#include "GameObject.h"
#include "PNGImage.h"

class Ball : public GameObject
{
public:
    Ball(RenderEngine &engine, PhysicsSimulator &physics);

    virtual ~Ball(void);

    void SetVelocity(float velocity, float angle);

    void GetVelocity(float &velocity, float &angle);

    void EndContact(IContactObject *another_object);

    virtual Texture * GetTexture();

protected:

    PNGImage _ball_png;
};

