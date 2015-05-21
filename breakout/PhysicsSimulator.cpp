#include "PhysicsSimulator.h"


PhysicsSimulator::PhysicsSimulator(void)
{
    b2Vec2 gravity(0,0);
    _world = new b2World(gravity);
    _world->SetContactListener(this);    
}


PhysicsSimulator::~PhysicsSimulator(void)
{
    delete _world;
}

void PhysicsSimulator::Step()
{
    _world->Step(1/60.0, 6, 2);
}

void PhysicsSimulator::BeginContact( b2Contact* contact )
{
    IContactObject *obj_a = NULL, *obj_b = NULL;
    void *user_data = contact->GetFixtureA()->GetBody()->GetUserData();
    if(user_data != NULL)
        obj_a = static_cast<IContactObject *>(user_data);

    user_data = contact->GetFixtureB()->GetBody()->GetUserData();
    if(user_data != NULL)
        obj_b = static_cast<IContactObject *>(user_data);

    if(obj_a != NULL && obj_b != NULL)
        obj_a->Contact(obj_b);
}
