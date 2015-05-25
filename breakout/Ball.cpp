#include "Ball.h"
#include "GameControler.h"
#include <math.h>

PNGImage Ball::_g_ball_png("ball.png");
void Ball::LoadPNG( RenderEngine &engine )
{
    _g_ball_png.Load(engine);
}

Ball::Ball(GameControler &game) : TextureGameObject(game, b2_dynamicBody), _ball_png(_g_ball_png)
{
    b2FixtureDef fixture;
    b2CircleShape circle;
    circle.m_radius = _game.Physics().Pixel2Meter(_ball_png.Width() / 2);
    fixture.shape = &circle;
    fixture.density = 0;
    fixture.restitution = 1;
    
    _body->CreateFixture(&fixture);
    _destroyed = false;
}

Ball::~Ball(void)
{
}

void Ball::EndContact( IGameObject *another_object )
{
    CheckVelocity();
}

Texture * Ball::GetTexture()
{
    return &_ball_png;
}

void Ball::SetInitialPosition( int x, int y )
{
    _body->SetTransform(b2Vec2(_game.Physics().Pixel2Meter(x), _game.Physics().Pixel2Meter(y-_ball_png.Height()/2)), _body->GetAngle());
}

void Ball::Start()
{
    float v = 1.0;
    if(rand() < 0)
        v = -1.0;

    _body->SetLinearVelocity(b2Vec2(v * (rand() % 20), rand() % 20));
    CheckVelocity();
}

void Ball::Stop()
{
    _body->SetLinearVelocity(b2Vec2_zero);
}

void Ball::CheckVelocity()
{
    const b2Vec2 &v = _body->GetLinearVelocity();

    float angle = atan(abs(v.y)/abs(v.x));
    float velocity = sqrt(v.x*v.x + v.y*v.y);

    if(velocity != 30.0)
    {
        velocity = 30.0;
        double x = 0.0, y = 0.0;

        if(angle < M_PI * 0.05)
            angle = M_PI * 0.05;
        if(abs(angle) < M_PI / 2 * 0.95)
            angle *= 0.95;
        if(v.x != 0.0)
            x = velocity * cos(angle) * v.x/abs(v.x);
        if(v.y != 0.0)
            y = velocity * sin(angle) * v.y/abs(v.y);
        else 
            y = 1.0;
        _body->SetLinearVelocity(b2Vec2((float)x, (float)y));
    }
}

bool Ball::NotMoving()
{
    const b2Vec2 &v = _body->GetLinearVelocity();
    return v.x == 0.0 && v.y == 0.0;
}

GAME_OBJECT_TYPE Ball::Type()
{
    return GAME_OBJECT_BALL;
}