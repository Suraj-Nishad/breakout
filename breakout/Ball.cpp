#include "Ball.h"


Ball::Ball
(
    RenderEngine &engine, 
    PhysicsSimulator &physics
) : _ball_png("ball.png"), _physics(physics)
{
    _ball_png.Load(engine);

    b2BodyDef body_def;
    body_def.type = b2_dynamicBody;
    body_def.userData = this;
    body_def.position.Set(physics.Pixel2Meter(300), physics.Pixel2Meter(300));

    _body = physics.World().CreateBody(&body_def);
    b2FixtureDef fixture;
    b2CircleShape circle;
    circle.m_radius = physics.Pixel2Meter(_ball_png.Width() / 2);
    fixture.shape = &circle;
    fixture.density = 0;
    fixture.restitution = 1;
    _body->CreateFixture(&fixture);
    _body->SetLinearVelocity(b2Vec2(1.1, 1.1));
}

Ball::~Ball(void)
{
    _physics.World().DestroyBody(_body);
}

void Ball::AddTexture( std::list<IRenderElement *> &elements )
{
    int x = _physics.Meter2Pixel(_body->GetPosition().x)-(_ball_png.Width() / 2);
    int y = _physics.Meter2Pixel(_body->GetPosition().y)-(_ball_png.Height() / 2);

    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "ball x=%d, y=%d", x, y);
    _ball_png.SetPosition(x, y);
    elements.push_back(&_ball_png);
}

void Ball::Contact(b2Body *another_body)
{
}

void Ball::SetVelocity(float velocity, float angle)
{

}
