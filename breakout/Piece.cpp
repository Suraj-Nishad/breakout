#include "Piece.h"

PNGImage Piece::_g_piece_png("MCTestTaskPiece.png");

void Piece::LoadPNG( RenderEngine &engine )
{
    _g_piece_png.Load(engine);
}

Piece::Piece
(
    RenderEngine &engine, 
    PhysicsSimulator &physics, 
    int x, 
    int y,
    int max_width
) : GameObject(engine, physics, b2_staticBody), _piece_texture(_g_piece_png)
{
    int width = rand() % (2*PNGWidth());
    if(width < PNGWidth()/2)
        width = PNGWidth()/2;
    if(width > max_width || (max_width - width) < PNGWidth()/2)
        width = max_width;

    _piece_texture.Width(width);
    _piece_texture.SetColor(rand() % 255, rand() % 255, rand() % 255);
    _body->SetTransform(b2Vec2(physics.Pixel2Meter(x+_piece_texture.Width()/2), physics.Pixel2Meter(y+_piece_texture.Height()/2)), _body->GetAngle());
    b2FixtureDef fixture;
    b2PolygonShape rect;
    rect.SetAsBox(physics.Pixel2Meter(_piece_texture.Width()/2), physics.Pixel2Meter(_piece_texture.Height()/2));
    fixture.shape = &rect;
    fixture.density = 0;
    fixture.restitution = 1;
    _body->CreateFixture(&fixture);
    _destroyed = false;
}

Piece::~Piece(void)
{
}

Texture * Piece::GetTexture()
{
    return &_piece_texture;
}

void Piece::EndContact( IContactObject *another_object )
{
    //only ball contacts a piece
    _destroyed = true;
}
