#pragma once

#include "RenderEngine.h"
#include "PhysicsSimulator.h"

#include <string>
#include <vector>

class Texture;
class GameControler;

class GameObject : public IContactObject
{
public:
    GameObject(GameControler &game,
               b2BodyType body_type);

    virtual ~GameObject(void);

    void AddTexture(std::vector<IRenderElement *> &elements);

    virtual void BeginContact(IContactObject *another_object);

    virtual void EndContact(IContactObject *another_object);

    virtual void Destroy();

    virtual void IsDestroyed();

protected:
    
    virtual Texture *GetTexture() = 0;

    bool _destroyed;

    b2Body *_body;
    GameControler &_game;
};

