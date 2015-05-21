#include "Paddle.h"

Paddle::Paddle
(
    RenderEngine &engine, 
    PhysicsSimulator &physics,
    GameArea &game_area
) : GameObject(engine, physics, "MCTestTaskPaddle.png", b2_kinematicBody)
{
    _body->SetTransform(b2Vec2(physics.Pixel2Meter(engine.Width()/2), physics.Pixel2Meter(engine.Height()-_png_image.Height()-50)), _body->GetAngle());
    b2FixtureDef fixture;
    b2PolygonShape rect;
    rect.SetAsBox(physics.Pixel2Meter(_png_image.Width()/2), physics.Pixel2Meter(_png_image.Height()/2));
    fixture.shape = &rect;
    fixture.density = 0;
    fixture.restitution = 1;
    _body->CreateFixture(&fixture);

    _min_x = game_area.MarginPixel();
    _max_x = _min_x + game_area.WidthPixel() - _png_image.Width();
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

    _body->SetTransform(b2Vec2(_physics.Pixel2Meter(x+_png_image.Width()/2), _body->GetPosition().y), _body->GetAngle());
}
