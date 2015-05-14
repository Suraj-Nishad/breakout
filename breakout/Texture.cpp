#include "Texture.h"

int Texture::MAX_X = 0x7fffffff;
int Texture::MAX_Y = 0x7fffffff;

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

void Texture::SetPosition(int x, int y)
{
    _x = (x<0?0:x); 
    _y = (y<0?0:y);

    int max_x = MAX_X-Width();
    int max_y = MAX_Y-Height();

    if(_x > max_x)
        _x = max_x;
    if(_y > max_y)
        _y = max_y;

}