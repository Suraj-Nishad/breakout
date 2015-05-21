#include "GameObject.h"


GameObject::GameObject
(
    RenderEngine &engine, 
    PhysicsSimulator &physics, 
    const std::string &png_filename,
    b2BodyType body_type
) : _png_image(png_filename), _physics(physics)
{
    _png_image.Load(engine);
    b2BodyDef body_def;
    body_def.type = body_type;
    body_def.userData = this;
    _body = physics.World().CreateBody(&body_def);
}


GameObject::~GameObject(void)
{
    _physics.World().DestroyBody(_body);
}

void GameObject::Contact( IContactObject *another_object )
{
}

void GameObject::AddTexture( std::list<IRenderElement *> &elements )
{
    int x = _physics.Meter2Pixel(_body->GetPosition().x)-(_png_image.Width() / 2);
    int y = _physics.Meter2Pixel(_body->GetPosition().y)-(_png_image.Height() / 2);

    _png_image.SetPosition(x, y);
    elements.push_back(&_png_image);
}
