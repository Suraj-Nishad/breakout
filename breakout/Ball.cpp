#include "Ball.h"
#include "GameArea.h"
#include <math.h>

Ball::Ball(GameArea &game) : GameObject(game, b2_dynamicBody), _ball_png("ball.png")
{
    _ball_png.Load(game.Renderer());
    _body->SetTransform(b2Vec2(_game.Physics().Pixel2Meter(300), _game.Physics().Pixel2Meter(300)), _body->GetAngle());
    b2FixtureDef fixture;
    b2CircleShape circle;
    circle.m_radius = _game.Physics().Pixel2Meter(_ball_png.Width() / 2);
    fixture.shape = &circle;
    fixture.density = 0;
    fixture.restitution = 1;
    
    _body->CreateFixture(&fixture);
    _destroyed = false;

    _body->SetLinearVelocity(b2Vec2(20.0, 20.0));
}

Ball::~Ball(void)
{
}

void Ball::EndContact( IContactObject *another_object )
{
    const b2Vec2 &v = _body->GetLinearVelocity();

    float angle = atan(abs(v.y)/abs(v.x));
    float velocity = sqrt(v.x*v.x + v.y*v.y);

    if(velocity < 20.0)
    {
        velocity = 20.0;
        double x = 0.0, y = 0.0;
        if(v.x != 0.0)
            x = velocity * cos(angle) * v.x/abs(v.x);
        if(v.y != 0.0)
            y = velocity * sin(angle) * v.y/abs(v.y);
        _body->SetLinearVelocity(b2Vec2((float)x, (float)y));
    }
}

Texture * Ball::GetTexture()
{
    return &_ball_png;
}
