#pragma once
#include "physicssimulator.h"

class GameArea;
class GroundLine : public IContactObject
{
public:
    GroundLine(GameArea &game);

    virtual ~GroundLine(void);

    virtual void BeginContact( IContactObject *another_object );

    virtual void EndContact( IContactObject *another_object );

protected:
    b2Body *_ground;
    GameArea &_game;

};

