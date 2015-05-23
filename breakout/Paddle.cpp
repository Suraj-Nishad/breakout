#include "Paddle.h"
#include "GameArea.h"

Paddle::Paddle(GameArea &game_area) : GameObject(game_area, b2_kinematicBody), _paddle_png("MCTestTaskPaddle.png")
{
    _paddle_png.Load(_game.Renderer());
    _paddle_png.SetColor(255,0,0);
    _body->SetTransform(b2Vec2(_game.Physics().Pixel2Meter(_game.Renderer().Width()/2), _game.Physics().Pixel2Meter(_game.Renderer().Height()-_paddle_png.Height()-50)), _body->GetAngle());
    b2FixtureDef fixture;
    b2PolygonShape rect;
    rect.SetAsBox(_game.Physics().Pixel2Meter(_paddle_png.Width()/2), _game.Physics().Pixel2Meter(_paddle_png.Height()/2));
    //HACK: We don't really want a perfect box to avoid horizontal movement.
    rect.m_vertices[0].x *= 1.05;
    rect.m_vertices[3].x *= 1.05;

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

    _body->SetTransform(b2Vec2(_game.Physics().Pixel2Meter(x), _body->GetPosition().y), _body->GetAngle());
}

Texture * Paddle::GetTexture()
{
    return &_paddle_png;
}
