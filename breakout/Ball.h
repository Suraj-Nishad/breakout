#pragma once

#include "GameObject.h"

class Ball : public GameObject
{
public:
    Ball(RenderEngine &engine, PhysicsSimulator &physics);

    virtual ~Ball(void);

    void SetVelocity(float velocity, float angle);
};

