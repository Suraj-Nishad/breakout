#include "Paddle.h"
#include "GameControler.h"

Paddle::Paddle(GameControler &game) : TextureGameObject(game, b2_kinematicBody), _paddle_png("MCTestTaskPaddle.png")
{
    _paddle_png.Load(_game.Renderer());
    _paddle_png.SetColor(255,0,0);
    _disapear = false;
    _texture_alpha = 255;
    _body->SetTransform(b2Vec2(_game.Physics().Pixel2Meter(_game.Renderer().Width()/2), _game.Physics().Pixel2Meter(_game.Renderer().Height()-_paddle_png.Height()-50)), _body->GetAngle());
    b2FixtureDef fixture;
    b2PolygonShape rect;

    //HACK: We don't want a perfect rectangular to avoid horizontal movements. 
    float hx, hy;
    hx = _game.Physics().Pixel2Meter(_paddle_png.Width()/2);
    hy = _game.Physics().Pixel2Meter(_paddle_png.Height()/2);
    b2Vec2 vertices[4];
    vertices[0].Set(-hx * 0.8, -hy);
    vertices[1].Set(hx * 0.8, -hy);
    vertices[2].Set(hx * 1.2, hy);
    vertices[3].Set(-hx * 1.2, hy);
    rect.Set(vertices, 4);

    fixture.shape = &rect;
    fixture.density = 0;
    fixture.restitution = 1;
    fixture.filter.categoryBits = GAME_OBJECT_PADDLE;
    fixture.filter.maskBits = GAME_OBJECT_BALL | GAME_OBJECT_BONUS;

    _body->CreateFixture(&fixture);

    _min_x = game.MarginPixel() + _paddle_png.Width()/2;
    _max_x = game.MarginPixel() + game.WidthPixel() - _paddle_png.Width()/2;
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
    if(_disapear)
    {
        if(_texture_alpha > 0)
            _texture_alpha -= 5;
    }
    _paddle_png.SetAlpha(_texture_alpha);
  
    return &_paddle_png;
}

void Paddle::GetCenterPoint(int &x, int &y)
{
    Texture *this_texture = GetTexture();
    x = _game.Physics().Meter2Pixel(_body->GetPosition().x);
    y = _game.Physics().Meter2Pixel(_body->GetPosition().y)-(this_texture->Height() / 2);
}

GAME_OBJECT_TYPE Paddle::Type()
{
    return GAME_OBJECT_PADDLE;
}

void Paddle::Show()
{
    _disapear = false;
    _texture_alpha = 255;
}
