#pragma once
#include "Texture.h"
#include <string>
#include "RenderEngine.h"

class PNGImage : public Texture
{
public:
    PNGImage(const std::string &filename);

    virtual ~PNGImage(void);

    bool Load(RenderEngine &engine);

    void UnLoad();

protected:
    virtual SDL_Texture *GetTexture();

    std::string _filename;
    
    SDL_Texture *_texture;
};

