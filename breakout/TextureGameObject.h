#pragma once

#include "RenderEngine.h"
#include "PhysicsSimulator.h"
#include "IGameObject.h"

#include <string>
#include <vector>

class Texture;
class GameControler;

class TextureGameObject : public IGameObject
{
public:
    TextureGameObject(GameControler &game,
               b2BodyType body_type);

    virtual ~TextureGameObject(void);

    void AddTexture(std::vector<IRenderElement *> &elements);

    virtual void BeginContact(IGameObject *another_object);

    virtual void EndContact(IGameObject *another_object);

    virtual void Destroy();

    virtual bool IsDestroyed();

protected:
    
    virtual Texture *GetTexture() = 0;

    bool _destroyed;

    b2Body *_body;
    GameControler &_game;
};

