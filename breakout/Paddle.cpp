#include "Paddle.h"

Paddle::Paddle
(
    RenderEngine &engine, 
    PhysicsSimulator &physics,
    GameArea &game_area
) : GameObject(engine, physics, b2_kinematicBody), _paddle_png("MCTestTaskPaddle.png")
{
    _paddle_png.Load(engine);
    _paddle_png.SetColor(255,0,0);
    _body->SetTransform(b2Vec2(physics.Pixel2Meter(engine.Width()/2), physics.Pixel2Meter(engine.Height()-_paddle_png.Height()-50)), _body->GetAngle());
    b2FixtureDef fixture;
    b2PolygonShape rect;
    rect.SetAsBox(physics.Pixel2Meter(_paddle_png.Width()/2), physics.Pixel2Meter(_paddle_png.Height()/2));
    fixture.shape = &rect;
    fixture.density = 0;
    fixture.restitution = 1;
    _body->CreateFixture(&fixture);

    _min_x = game_area.MarginPixel() + _paddle_png.Width()/2;
    _max_x = game_area.MarginPixel() + game_area.WidthPixel() - _paddle_png.Width()/2;
}

Paddle::~Paddle(void)
{
}

void Paddle::SetX(int x)
{
    if(x<_min_x)
        x = _min_x;
    if(x>_max_x)
        x = _max_x;

    _body->SetTransform(b2Vec2(_physics.Pixel2Meter(x), _body->GetPosition().y), _body->GetAngle());
}

Texture * Paddle::GetTexture()
{
    return &_paddle_png;
}
