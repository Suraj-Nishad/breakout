#pragma once

#include "RenderEngine.h"
#include "PNGImage.h"
#include "PhysicsSimulator.h"
#include "GameArea.h"

#include <list>

class Ball
{
public:
    Ball(RenderEngine &engine, PhysicsSimulator &physics);
    virtual ~Ball(void);

    void AddTexture(std::list<IRenderElement *> &elements);

    void Contact(b2Body *another_body);

protected:
    PNGImage _ball_png;
    b2Body *_body;
    PhysicsSimulator &_physics;
};

