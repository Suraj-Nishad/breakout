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

void PhysicsSimulator::Step(float rate)
{
    _world->Step(rate, 6, 2);
}

void PhysicsSimulator::Mapb2ContactToIContactObject
(
    b2Contact *contact, 
    IContactObject **obj_a, 
    IContactObject **obj_b
)
{
    void *user_data = contact->GetFixtureA()->GetBody()->GetUserData();
    if(user_data != NULL)
        *obj_a = static_cast<IContactObject *>(user_data);
    else
        *obj_a = NULL;

    user_data = contact->GetFixtureB()->GetBody()->GetUserData();
    if(user_data != NULL)
        *obj_b = static_cast<IContactObject *>(user_data);
    else
        *obj_b = NULL;
}

void PhysicsSimulator::BeginContact( b2Contact* contact )
{
    IContactObject *obj_a = NULL, *obj_b = NULL;
    Mapb2ContactToIContactObject(contact, &obj_a, &obj_b);
    if(obj_a != NULL && obj_b != NULL)
    {
        obj_a->BeginContact(obj_b);
        obj_b->BeginContact(obj_a);
    }
}

void PhysicsSimulator::EndContact(b2Contact* contact)
{
    IContactObject *obj_a = NULL, *obj_b = NULL;
    Mapb2ContactToIContactObject(contact, &obj_a, &obj_b);
    if(obj_a != NULL && obj_b != NULL)
    {
        obj_a->EndContact(obj_b);
        obj_b->EndContact(obj_a);
    }
}
