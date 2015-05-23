#pragma once

#include "RenderEngine.h"
#include "PhysicsSimulator.h"

#include <string>
#include <list>

class Texture;

class GameObject : public IContactObject
{
public:
    GameObject(RenderEngine &engine, 
               PhysicsSimulator &physics,
               b2BodyType body_type);

    virtual ~GameObject(void);

    void AddTexture(std::list<IRenderElement *> &elements);

    virtual void BeginContact(IContactObject *another_object);

    virtual void EndContact(IContactObject *another_object);

protected:
    
    virtual Texture *GetTexture() = 0;

    b2Body *_body;
    PhysicsSimulator &_physics;
};

