#include "RenderEngine.h"
#include <sstream>
#include <stdexcept>
#include "SDL_log.h"

RenderEngine::RenderEngine()
{
    //members init
	_window = NULL;
    _renderer = NULL;
    _background = NULL;

    //first, load the SDL lib
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::stringstream ss;
        ss << "SDL could not initialize. Error " << SDL_GetError();
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "%s", ss.str().c_str());
		throw std::runtime_error(ss.str());
    }

    //then, init the SDL_image to load .png files
	if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0)
	{
        std::stringstream ss;
        ss << "SDL_image could not initialize. Error " << IMG_GetError();
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "%s", ss.str().c_str());

		SDL_Quit();
		throw std::runtime_error(ss.str());
	}
}


RenderEngine::~RenderEngine(void)
{
    if(_background != NULL)
        SDL_DestroyTexture(_background);

    if(_renderer != NULL)
        SDL_DestroyRenderer(_renderer);

    if(_window != NULL)
        SDL_DestroyWindow(_window);

    //shutdown SDL_image library
	IMG_Quit();

    //shutdown SDL library
	SDL_Quit();
}

bool RenderEngine::CreateWindow
(
    const std::string &title /*= "Breakout"*/,
    int width /*= 640*/,
    int height /*= 480*/
)
{
    //we only have 1 window
    if(_window != NULL)
        return false;

    //create window at undefined position at the desktop
    _window = SDL_CreateWindow(title.c_str(),
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               width,
                               height,
                               SDL_WINDOW_SHOWN);
    if(_window == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Can't create main window. SDL_CreateWindow error %s", SDL_GetError());

        return false;
    }

    //Create renderer for window
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if(_renderer == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Can't get renderer from window. SDL_CreateRenderer error %s", SDL_GetError());

        SDL_DestroyWindow(_window);
        _window = NULL;

        return false;
    }

    //Initialize renderer color
    if(SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF ) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Can't initialize renderer. SDL_SetRenderDrawColor error %s", SDL_GetError());

        SDL_DestroyRenderer(_renderer);
        _renderer = NULL;

        SDL_DestroyWindow(_window);
        _window = NULL;
        
        return false;
    }

    return true;
}

SDL_Texture *RenderEngine::LoadPNGImage(const std::string &file)
{ 
    SDL_Surface *loaded = IMG_Load(file.c_str());
    if(loaded == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Can't load PNG file %s. IMG_Load error ", file.c_str(), SDL_GetError());
        return NULL;
    }

    //optimize loaded image to screen format
    SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, loaded);
    if(texture == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER,
                     "Can't create texture from PNG file %s. SDL_CreateTextureFromSurface error ", 
                     file.c_str(), 
                     SDL_GetError());
    }

    //get rid of the loaded image
    SDL_FreeSurface(loaded);

    return texture;
}

void RenderEngine::SetBackgroundImage(const std::string &file)
{
    if(_background != NULL)
        SDL_DestroyTexture(_background);
    _background = LoadPNGImage(file);
}

void RenderEngine::Render()
{
    //Clear screen
    SDL_RenderClear(_renderer);

    //Render texture to screen
    SDL_RenderCopy(_renderer, _background, NULL, NULL);


    //Update screen
    SDL_RenderPresent(_renderer);
}
