#pragma once
#include "Box2D/Box2D.h"

class IContactObject;
class PhysicsSimulator : public b2ContactListener
{
public:
    PhysicsSimulator(void);
    virtual ~PhysicsSimulator(void);

    inline b2World &World() {return *_world;}
    
    void Step(float rate);

    inline float Pixel2Meter(int pixel) {return (float)(pixel/20.0);}
    inline int Meter2Pixel(float meter) {return (int)(meter*20);}

    virtual void BeginContact( b2Contact* contact );

    virtual void EndContact(b2Contact* contact);

protected:

    void Mapb2ContactToIContactObject(b2Contact *contact, IContactObject **obj_a, IContactObject **obj_b);

    b2World *_world;
};

class IContactObject
{
public:
    virtual void BeginContact(IContactObject *another_object) = 0;

    virtual void EndContact(IContactObject *another_object) = 0;

    virtual void Destroy() = 0;

    virtual void IsDestroyed() = 0;

protected:
    IContactObject() {}
    virtual ~IContactObject() {};
};
