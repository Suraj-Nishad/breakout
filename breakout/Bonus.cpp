#include "Bonus.h"
#include "Piece.h"
#include "GameControler.h"

Bonus::Bonus(GameControler &game, Piece &piece) : TextureGameObject(game, b2_dynamicBody), _piece_texture(Piece::_g_piece_png)
{
    _bonus_type = BONUS_NONE; //no bonus until this collide with paddle
    _piece_texture.CopyCharacteristics(*piece.GetTexture());
    _body->SetTransform(piece._body->GetPosition(), piece._body->GetAngle());

    b2FixtureDef fixture;
    b2PolygonShape rect;
    rect.SetAsBox(_game.Physics().Pixel2Meter(_piece_texture.Width()/2), _game.Physics().Pixel2Meter(_piece_texture.Height()/2));

    fixture.shape = &rect;
    fixture.density = 0;
    fixture.restitution = 1;

    fixture.filter.categoryBits = GAME_OBJECT_BONUS;
    fixture.filter.maskBits = GAME_OBJECT_GROUND | GAME_OBJECT_PADDLE;

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
    if(another_object->Type() == GAME_OBJECT_PADDLE)
    {
        _bonus_type = BONUS_MULTIPLE_BALLS;
        Destroy();
    }
}

void Bonus::EndContact(IGameObject *another_object)
{
    if(IsDestroyed() == false)
        _body->SetLinearVelocity(b2Vec2(0.0, 20.0));

}
GAME_OBJECT_TYPE Bonus::Type()
{
    return GAME_OBJECT_BONUS;
}
