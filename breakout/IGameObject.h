#pragma once

typedef enum
{
    GAME_OBJECT_GROUND = 0,
    GAME_OBJECT_BALL,
    GAME_OBJECT_PIECE,
    GAME_OBJECT_PADDLE,
    GAME_OBJECT_BONUS
} GAME_OBJECT_TYPE;

class IGameObject;
class IGameObject
{
public:
    virtual void BeginContact(IGameObject *another_object) = 0;

    virtual void EndContact(IGameObject *another_object) = 0;

    virtual void Destroy() = 0;

    virtual bool IsDestroyed() = 0;

    virtual GAME_OBJECT_TYPE Type() = 0;

protected:
    IGameObject() {}
    virtual ~IGameObject() {};
};
