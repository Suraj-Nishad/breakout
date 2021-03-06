#include "MusicPlayer.h"
#include "SDL.h"
#include <sstream>

MusicPlayer::MusicPlayer(void)
{
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        std::stringstream ss;
        ss << "SDL_mixer could not initialize. Error " << Mix_GetError();
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "%s", ss.str().c_str());
        throw std::runtime_error(ss.str());
    }

    _game_start = Mix_LoadWAV("Ark Intro Music.aif");
    _game_over = Mix_LoadWAV("Ark Game Over Music 2.aif");
    _bounce = Mix_LoadWAV("Ark Bounce 2.aif");
    _extra_life = Mix_LoadWAV("Ark Longship.aif");
    _bonus = Mix_LoadWAV("Ark Metal Bounce.aif");
    _win = Mix_LoadWAV("Ark Game Start Music.aif");
}


MusicPlayer::~MusicPlayer(void)
{
    Mix_FreeChunk(_game_start);
    Mix_FreeChunk(_game_over);
    Mix_FreeChunk(_bounce);
    Mix_FreeChunk(_extra_life);
    Mix_FreeChunk(_bonus);
    Mix_FreeChunk(_win);
    //shutdown SDL_mixer library
    Mix_Quit();
}

void MusicPlayer::PlayChunck(Mix_Chunk *chunck)
{
    if(chunck) 
    {
        Mix_PlayChannel(-1, chunck, 0);
    }
}
