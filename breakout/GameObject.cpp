#include "GameObject.h"
#include "Texture.h"

GameObject::GameObject
(
    RenderEngine &engine, 
    PhysicsSimulator &physics, 
    b2BodyType body_type
) : _physics(physics)
{
    b2BodyDef body_def;
    body_def.type = body_type;
    body_def.userData = this;
    _body = physics.World().CreateBody(&body_def);
}


GameObject::~GameObject(void)
{
    _physics.World().DestroyBody(_body);
}

void GameObject::BeginContact( IContactObject *another_object )
{
}

void GameObject::EndContact(IContactObject *another_object)
{

}

void GameObject::AddTexture( std::list<IRenderElement *> &elements )
{
    Texture *this_texture = GetTexture();
    int x = _physics.Meter2Pixel(_body->GetPosition().x)-(this_texture->Width() / 2);
    int y = _physics.Meter2Pixel(_body->GetPosition().y)-(this_texture->Height() / 2);

    this_texture->SetPosition(x, y);
    elements.push_back(this_texture);
}
