#include "Bonus.h"
#include "Piece.h"
#include "GameControler.h"

Bonus::Bonus(GameControler &game, Piece &piece) : TextureGameObject(game, b2_dynamicBody), _piece_texture(Piece::_g_piece_png)
{
    _piece_texture.CopyCharacteristics(*piece.GetTexture());
    _piece_texture.Width(Piece::Image().Width());
    _body->SetTransform(piece._body->GetPosition(), piece._body->GetAngle());

    //create a tiny circle that represents the bonus falling down.
    b2FixtureDef fixture;
    b2CircleShape circle;
    circle.m_radius = 0.01;
    fixture.shape = &circle;
    fixture.density = 0;
    fixture.restitution = 1;

    _body->CreateFixture(&fixture);
    _body->CreateFixture(&fixture);
    _body->SetLinearVelocity(b2Vec2(0.0, 20.0));
}


Bonus::~Bonus(void)
{
}

Texture * Bonus::GetTexture()
{
    return &_piece_texture;
}

void Bonus::BeginContact( IGameObject *another_object )
{
    Destroy();
}

GAME_OBJECT_TYPE Bonus::Type()
{
    return GAME_OBJECT_BONUS;
}
