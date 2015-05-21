#pragma once

#include "GameArea.h"
#include "GameObject.h"

class Paddle : public GameObject
{
public:
    Paddle(RenderEngine &engine, PhysicsSimulator &physics, GameArea &game_area);

    virtual ~Paddle(void);

    void SetX(int x);
protected:
    int _min_x, _max_x;
};

