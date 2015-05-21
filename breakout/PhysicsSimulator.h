#pragma once
#include "Box2D/Box2D.h"

class PhysicsSimulator : public b2ContactListener
{
public:
    PhysicsSimulator(void);
    virtual ~PhysicsSimulator(void);

    inline b2World &World() {return *_world;}
    
    void Step();

    inline float Pixel2Meter(int pixel) {return (float)(pixel/10.0);}
    inline int Meter2Pixel(float meter) {return (int)(meter*10);}

    virtual void BeginContact( b2Contact* contact );

protected:
    b2World *_world;
};

class IContactObject;

class IContactObject
{
public:
    virtual void Contact(IContactObject *another_object) = 0;
protected:
    IContactObject() {}
    virtual ~IContactObject() {};
};
