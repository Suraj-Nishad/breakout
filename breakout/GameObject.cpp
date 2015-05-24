#include "GameObject.h"
#include "Texture.h"
#include "GameControler.h"
#include "PhysicsSimulator.h"

GameObject::GameObject
(
    GameControler &game, 
    b2BodyType body_type
) : _game(game)
{
    b2BodyDef body_def;
    body_def.type = body_type;
    body_def.userData = this;
    _body = _game.Physics().World().CreateBody(&body_def);
    _destroyed = false;
}


GameObject::~GameObject(void)
{
    _game.Physics().World().DestroyBody(_body);
}

void GameObject::BeginContact( IGameObject *another_object )
{
}

void GameObject::EndContact(IGameObject *another_object)
{

}

void GameObject::AddTexture( std::vector<IRenderElement *> &elements )
{
    Texture *this_texture = GetTexture();
    int x = _game.Physics().Meter2Pixel(_body->GetPosition().x)-(this_texture->Width() / 2);
    int y = _game.Physics().Meter2Pixel(_body->GetPosition().y)-(this_texture->Height() / 2);

    this_texture->SetPosition(x, y);
    elements.push_back(this_texture);
}

void GameObject::Destroy()
{
    _destroyed = true;
}

bool GameObject::IsDestroyed()
{
    return _destroyed;
}
