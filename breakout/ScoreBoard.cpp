#include "RenderEngine.h"
#include "ScoreBoard.h"
#include "SDL.h"
#include <sstream>

ScoreBoard::ScoreBoard(RenderEngine &engine) : _engine(engine)
{
    _arial = TTF_OpenFont("arial.ttf", 14);
    _current_texture = NULL;
    _has_color = true;
    _r = _g = _b = 255;
    _score = 0;
}

ScoreBoard::~ScoreBoard(void)
{
    if(_arial)
        TTF_CloseFont(_arial);

    if(_current_texture != NULL)
        SDL_DestroyTexture(_current_texture);
}

SDL_Texture * ScoreBoard::GetTexture()
{
    if(_arial == NULL)
        return NULL;

    if(_current_texture != NULL)
        return _current_texture;

    std::stringstream ss;
    ss << "Score : " << _score;
    SDL_Color white;
    white.r = white.g = white.b = 255;

    //Render text surface
    SDL_Surface* surface = TTF_RenderText_Solid( _arial, ss.str().c_str(), white );

    if( surface != NULL )
    {
        //Create texture from surface pixels
        _current_texture = SDL_CreateTextureFromSurface( _engine.Handle(), surface );
        if( _current_texture == NULL )
        {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            SDL_QueryTexture(_current_texture, NULL, NULL, &_width, &_height);
        }

        //Get rid of old surface
        SDL_FreeSurface( surface );
    }
        
    return _current_texture;
}

void ScoreBoard::Add( unsigned int value )
{
    if(_current_texture != NULL)
        SDL_DestroyTexture(_current_texture);
    _current_texture = NULL;

    _score += value;
}

void ScoreBoard::Reset()
{
    if(_current_texture != NULL)
        SDL_DestroyTexture(_current_texture);
    _current_texture = NULL;

    _score = 0;
}
