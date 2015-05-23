#include "Piece.h"
#include "GameArea.h"
PNGImage Piece::_g_piece_png("MCTestTaskPiece.png");

void Piece::LoadPNG( RenderEngine &engine )
{
    _g_piece_png.Load(engine);
}

Piece::Piece
(
    GameArea &game, 
    int x, 
    int y,
    int max_width
) : GameObject(game, b2_staticBody), _piece_texture(_g_piece_png)
{
    int width = rand() % (2*PNGWidth());
    if(width < PNGWidth()/2)
        width = PNGWidth()/2;
    if(width > max_width || (max_width - width) < PNGWidth()/2)
        width = max_width;

    _piece_texture.Width(width);
    _piece_texture.SetColor(rand() % 255, rand() % 255, rand() % 255);
    _body->SetTransform(b2Vec2(_game.Physics().Pixel2Meter(x+_piece_texture.Width()/2), _game.Physics().Pixel2Meter(y+_piece_texture.Height()/2)), _body->GetAngle());
    b2FixtureDef fixture;
    b2PolygonShape rect;
    rect.SetAsBox(_game.Physics().Pixel2Meter(_piece_texture.Width()/2), _game.Physics().Pixel2Meter(_piece_texture.Height()/2));
    //HACK: We don't really want a perfect box to avoid horizontal movement.
    rect.m_vertices[1].x *= 0.9;
    rect.m_vertices[2].x *= 0.9;
    
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
