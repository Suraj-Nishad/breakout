#include "Texture.h"

Texture::Texture(void)
{
    _width = _height = 0;
    _x = _y = 0;
    _has_color = false;
    _r = _g = _b = 0;
}

Texture::~Texture(void)
{
}

Texture::Texture(const Texture &another_texture)
{
    _width = another_texture._width;
    _height = another_texture._height;
    _x = another_texture._x;
    _y = another_texture._y;
    _has_color = another_texture._has_color;
    _r = another_texture._r;
    _g = another_texture._g;
    _b = another_texture._b;
}

void Texture::Show(RenderEngine &engine)
{
    SDL_Texture *texture = GetTexture();
    if(texture == NULL)
        return;
    
    if(_has_color)
        SDL_SetTextureColorMod(texture, _r, _g ,_b);
    SDL_Rect dst = {_x, _y, _width, _height};
    SDL_RenderCopy(engine.Handle(), texture, NULL, &dst);
}

void Texture::SetPosition(int x, int y)
{
    _x = x;
    _y = y;

}

void Texture::SetColor(Uint8 r, Uint8 g, Uint8 b)
{
    _has_color = true;
    _r = r;
    _g = g;
    _b = b;
}

WeakCopyTexture::WeakCopyTexture(Texture &another_texture) : Texture(another_texture)
{
    _original_texture = &another_texture;
}

SDL_Texture *WeakCopyTexture::GetTexture()
{
    return _original_texture->GetTexture();
}
