#include "GroundLine.h"
#include "GameArea.h"
#include "Ball.h"

GroundLine::GroundLine(GameArea &game) : _game(game)
{
    b2BodyDef body_def;
    body_def.type = b2_staticBody;
    body_def.userData = this;
    _ground = _game.Physics().World().CreateBody(&body_def);

    b2EdgeShape edge;
    b2FixtureDef fixture;
    fixture.shape = &edge;
    fixture.friction = 0;

    edge.Set(b2Vec2(0.0, _game.HeightMeter()+2*_game.MarginMeter()),
             b2Vec2(_game.WidthMeter()+2*_game.MarginMeter(), _game.HeightMeter()+2*_game.MarginMeter()));
    _ground->CreateFixture(&fixture);
}


GroundLine::~GroundLine(void)
{
    _game.Physics().World().DestroyBody(_ground);
}

void GroundLine::BeginContact( IContactObject *another_object )
{
    //another object must be a ball
    static_cast<Ball *>(another_object)->Destroyed(true);
}

void GroundLine::EndContact( IContactObject *another_object )
{
}
