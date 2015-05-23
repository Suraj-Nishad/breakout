#include "Ball.h"
#include <math.h>

Ball::Ball
(
    RenderEngine &engine, 
    PhysicsSimulator &physics
) : GameObject(engine, physics, b2_dynamicBody), _ball_png("ball.png")
{
    _ball_png.Load(engine);
    _body->SetTransform(b2Vec2(physics.Pixel2Meter(300), physics.Pixel2Meter(300)), _body->GetAngle());
    b2FixtureDef fixture;
    b2CircleShape circle;
    circle.m_radius = physics.Pixel2Meter(_ball_png.Width() / 2);
    fixture.shape = &circle;
    fixture.density = 0;
    fixture.restitution = 1;
    _body->CreateFixture(&fixture);
    SetVelocity(20.0, M_PI/4);
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

void Ball::GetVelocity(float &velocity, float &angle)
{
    const b2Vec2 &v = _body->GetLinearVelocity();

    angle = atan(v.y/v.x);
    if(angle == 0.0 && v.x < 0.0)
        angle = M_PI;
    velocity = sqrt(v.x*v.x + v.y*v.y);
}

void Ball::EndContact(IContactObject * /*another_object*/)
{  
    float velocity, angle;
    bool set_new_velocity = false;

    GetVelocity(velocity, angle);
    
    if(angle == M_PI)
    {
        angle = M_PI - 0.1;
        set_new_velocity = true;
    }
    if(angle == 0)
    {
        angle = 0.1;
        set_new_velocity = true;
    }
    if(velocity<20.0)
    {
        velocity = 20.0;
        set_new_velocity = true;
    }

    if(set_new_velocity)
        SetVelocity(velocity, angle);
}

Texture * Ball::GetTexture()
{
    return &_ball_png;
}
