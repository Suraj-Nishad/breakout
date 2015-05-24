#pragma once
#include "physicssimulator.h"

class GameControler;
class GroundLine : public IContactObject
{
public:
    GroundLine(GameControler &game);

    virtual ~GroundLine(void);

    virtual void BeginContact( IContactObject *another_object );

    virtual void EndContact( IContactObject *another_object );

protected:
    b2Body *_ground;
    GameControler &_game;

};

