#pragma once
#include "TextureGameObject.h"
#include "Texture.h"

class Piece;
class Bonus : public TextureGameObject
{
public:
    Bonus(GameControler &game, Piece &piece);

    virtual ~Bonus(void);

    virtual Texture * GetTexture();

    virtual void BeginContact(IGameObject *another_object);

    virtual GAME_OBJECT_TYPE Type();

protected:
    WeakCopyTexture _piece_texture;
};

