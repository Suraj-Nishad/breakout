#pragma once

#include "RenderEngine.h"
#include "PNGImage.h"
#include "PhysicsSimulator.h"

#include <string>
#include <list>

class GameObject : public IContactObject
{
public:
    GameObject(RenderEngine &engine, 
               PhysicsSimulator &physics, 
               const std::string &png_filename,
               b2BodyType body_type);

    virtual ~GameObject(void);

    void AddTexture(std::list<IRenderElement *> &elements);

    virtual void Contact( IContactObject *another_object );

protected:
    PNGImage _png_image;
    b2Body *_body;
    PhysicsSimulator &_physics;
};

