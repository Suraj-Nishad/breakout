#include "Texture.h"

Texture::Texture(void)
{
    _texture = NULL;
    _width = 0;
    _height = 0;
    _x = 0;
    _y = 0;
}

Texture::~Texture(void)
{
    Free();
}

void Texture::Free()
{
    if(_texture != NULL)
        SDL_DestroyTexture(_texture);
    _texture = NULL;
}

void Texture::Show(RenderEngine &engine)
{
    if(IsValid() == false)
        return;

    SDL_Rect dst = {_x, _y, _width, _height};
    SDL_RenderCopy(engine.Handle(), _texture, NULL, &dst);
}
