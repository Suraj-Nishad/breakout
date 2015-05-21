#include "Ball.h"
#include <math.h>

Ball::Ball
(
    RenderEngine &engine, 
    PhysicsSimulator &physics
) : GameObject(engine, physics, "ball.png", b2_dynamicBody)
{
    _body->SetTransform(b2Vec2(physics.Pixel2Meter(300), physics.Pixel2Meter(300)), _body->GetAngle());
    b2FixtureDef fixture;
    b2CircleShape circle;
    circle.m_radius = physics.Pixel2Meter(_png_image.Width() / 2);
    fixture.shape = &circle;
    fixture.density = 0;
    fixture.restitution = 1;
    _body->CreateFixture(&fixture);
    SetVelocity(2, M_PI/4);
}

Ball::~Ball(void)
{
}

void Ball::SetVelocity(float velocity, float angle)
{
    double x = velocity * cos(angle);
    double y = velocity * sin(angle);

    _body->SetLinearVelocity(b2Vec2((float)x, (float)y));
}
