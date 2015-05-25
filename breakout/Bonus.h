#pragma once
#include "TextureGameObject.h"
#include "Texture.h"

class Piece;

typedef enum
{
    BONUS_NONE = 0,
    BONUS_MULTIPLE_BALLS = 1,

    BONUS_NUMBER_OF_BONUS
} BONUS_TYPE;
class Bonus : public TextureGameObject
{
public:
    Bonus(GameControler &game, Piece &piece);

    virtual ~Bonus(void);

    virtual Texture * GetTexture();

    virtual void BeginContact(IGameObject *another_object);

    virtual void EndContact(IGameObject *another_object);

    virtual GAME_OBJECT_TYPE Type();

    inline BONUS_TYPE BonusType() {return _bonus_type;}

protected:
    WeakCopyTexture _piece_texture;
    BONUS_TYPE _bonus_type;
};

