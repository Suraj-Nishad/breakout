#pragma once

#include "RenderEngine.h"
#include "PhysicsSimulator.h"

#include <string>
#include <vector>

class Texture;
class GameArea;

class GameObject : public IContactObject
{
public:
    GameObject(GameArea &game,
               b2BodyType body_type);

    virtual ~GameObject(void);

    void AddTexture(std::vector<IRenderElement *> &elements);

    virtual void BeginContact(IContactObject *another_object);

    virtual void EndContact(IContactObject *another_object);

protected:
    
    virtual Texture *GetTexture() = 0;

    b2Body *_body;
    GameArea &_game;
};

