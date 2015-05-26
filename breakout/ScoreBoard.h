#pragma once

#include "SDL_ttf.h"
#include "Texture.h"

class RenderEngine;

class ScoreBoard : public Texture
{
public:
    ScoreBoard(RenderEngine &engine);

    virtual ~ScoreBoard(void);

    virtual SDL_Texture * GetTexture();

    void Add(unsigned int value);

    void Reset();

protected:
    RenderEngine &_engine;
    TTF_Font *_arial;
    unsigned int _score;
    SDL_Texture *_current_texture;
};

