#pragma once

typedef enum
{
    GAME_OBJECT_GROUND = 0x1,
    GAME_OBJECT_BALL = 0x2,
    GAME_OBJECT_PIECE = 0x4,
    GAME_OBJECT_PADDLE = 0x8,
    GAME_OBJECT_BONUS = 0x10,
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
