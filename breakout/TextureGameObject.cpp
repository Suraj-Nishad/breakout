#include "TextureGameObject.h"
#include "Texture.h"
#include "GameControler.h"
#include "PhysicsSimulator.h"

TextureGameObject::TextureGameObject
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


TextureGameObject::~TextureGameObject(void)
{
    _game.Physics().World().DestroyBody(_body);
}

void TextureGameObject::BeginContact( IGameObject *another_object )
{
}

void TextureGameObject::EndContact(IGameObject *another_object)
{

}

void TextureGameObject::AddTexture( std::vector<IRenderElement *> &elements )
{
    Texture *this_texture = GetTexture();
    int x = _game.Physics().Meter2Pixel(_body->GetPosition().x)-(this_texture->Width() / 2);
    int y = _game.Physics().Meter2Pixel(_body->GetPosition().y)-(this_texture->Height() / 2);

    this_texture->SetPosition(x, y);
    elements.push_back(this_texture);
}

void TextureGameObject::Destroy()
{
    _destroyed = true;
}

bool TextureGameObject::IsDestroyed()
{
    return _destroyed;
}
