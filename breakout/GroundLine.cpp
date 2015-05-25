#include "GroundLine.h"
#include "GameControler.h"
#include "Ball.h"

GroundLine::GroundLine(GameControler &game) : _game(game)
{
    b2BodyDef body_def;
    body_def.type = b2_staticBody;
    body_def.userData = this;
    _ground = _game.Physics().World().CreateBody(&body_def);

    b2EdgeShape edge;
    b2FixtureDef fixture;
    fixture.shape = &edge;
    fixture.friction = 0;
    fixture.filter.categoryBits = GAME_OBJECT_GROUND;
    fixture.filter.maskBits = GAME_OBJECT_BALL | GAME_OBJECT_BONUS;

    edge.Set(b2Vec2(0.0, _game.HeightMeter()+2*_game.MarginMeter()),
             b2Vec2(_game.WidthMeter()+2*_game.MarginMeter(), _game.HeightMeter()+2*_game.MarginMeter()));
    _ground->CreateFixture(&fixture);
}


GroundLine::~GroundLine(void)
{
    _game.Physics().World().DestroyBody(_ground);
}

void GroundLine::BeginContact( IGameObject *another_object )
{
    //Whatever touch gound line it is destroyed
    another_object->Destroy();
}

void GroundLine::EndContact( IGameObject *another_object )
{
}

void GroundLine::Destroy()
{
}

bool GroundLine::IsDestroyed()
{
    return false;
}

GAME_OBJECT_TYPE GroundLine::Type()
{
    return GAME_OBJECT_GROUND;
}
