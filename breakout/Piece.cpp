#include "Piece.h"
#include "GameControler.h"

#pragma warning(disable: 4244)

PNGImage Piece::_g_piece_png("MCTestTaskPiece.png");

void Piece::LoadPNG( RenderEngine &engine )
{
    _g_piece_png.Load(engine);
}

Piece::Piece
(
    GameControler &game, 
    int x, 
    int y,
    int max_width
) : TextureGameObject(game, b2_staticBody), _piece_texture(_g_piece_png)
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
    //HACK: We don't want a perfect rectangular to avoid horizontal movements. 
    float hx, hy;
    hx = _game.Physics().Pixel2Meter(_piece_texture.Width()/2);
    hy = _game.Physics().Pixel2Meter(_piece_texture.Height()/2);
    b2Vec2 vertices[4];
    vertices[0].Set(-hx * 0.8, -hy);
    vertices[1].Set(hx * 0.8, -hy);
    vertices[2].Set(hx * 1.1, hy);
    vertices[3].Set(-hx * 1.1, hy);
    rect.Set(vertices, 4);
    
    fixture.shape = &rect;
    fixture.density = 0;
    fixture.restitution = 1;
    fixture.filter.categoryBits = GAME_OBJECT_PIECE;
    fixture.filter.maskBits = GAME_OBJECT_BALL;
    
    _body->CreateFixture(&fixture);

    _has_bonus = (rand() % 10) == 0;
}

Piece::~Piece(void)
{
}

Texture * Piece::GetTexture()
{
    return &_piece_texture;
}

void Piece::EndContact( IGameObject *another_object )
{
    //only ball contacts a piece
    Destroy();

    _game.Music().PlayBounce();
}

GAME_OBJECT_TYPE Piece::Type()
{
    return GAME_OBJECT_PIECE;
}
