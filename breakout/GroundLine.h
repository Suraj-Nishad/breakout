#pragma once
#include "PhysicsSimulator.h"
#include "IGameObject.h"

class GameControler;
class GroundLine : public IGameObject
{
public:
    GroundLine(GameControler &game);

    virtual ~GroundLine(void);

    virtual void BeginContact( IGameObject *another_object );

    virtual void EndContact( IGameObject *another_object );

    virtual void Destroy();

    virtual bool IsDestroyed();

    virtual GAME_OBJECT_TYPE Type();

protected:
    b2Body *_ground;
    GameControler &_game;

};

