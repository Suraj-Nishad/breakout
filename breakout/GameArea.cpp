#include "GameArea.h"
#include "Texture.h"
#include "Ball.h"
#include "Paddle.h"

Line::Line(PhysicsSimulator &_physics, int x0, int y0, int x1, int y1 )
{
    _x0 = x0;
    _y0 = y0;
    _x1 = x1;
    _y1 = y1;
    _r = _g = _b = 255;
    _point0.Set(_physics.Pixel2Meter(x0), _physics.Pixel2Meter(y0));
    _point1.Set(_physics.Pixel2Meter(x1), _physics.Pixel2Meter(y1));
}

void Line::Show( RenderEngine &_engine )
{
    SDL_SetRenderDrawColor(_engine.Handle(), _r,_g,_b,0);
    SDL_RenderDrawLine( _engine.Handle(), _x0, _y0, _x1, _y1);
}

GameArea::GameArea() : _background("MCTestTaskBackground.png")
{
    _engine.CreateWindow("Breakout", 500, 700);
    _background.Load(_engine);

    _width = _engine.Width()-2*GAME_AREA_MARGIN;    
    _height = _engine.Height()-2*GAME_AREA_MARGIN;

    b2BodyDef body_def;
    _body = _physics.World().CreateBody(&body_def);
    _ground = _physics.World().CreateBody(&body_def);

    _lines.push_back(new Line(_physics, GAME_AREA_MARGIN, GAME_AREA_MARGIN, _engine.Width()-GAME_AREA_MARGIN, GAME_AREA_MARGIN));
    _lines.push_back(new Line(_physics, GAME_AREA_MARGIN, GAME_AREA_MARGIN, GAME_AREA_MARGIN, _engine.Height()-GAME_AREA_MARGIN));
    _lines.push_back(new Line(_physics, _engine.Width()-GAME_AREA_MARGIN, _engine.Height()-GAME_AREA_MARGIN, _engine.Width()-GAME_AREA_MARGIN, GAME_AREA_MARGIN));

    //ground line
    _lines.push_back(new Line(_physics, _engine.Width()-GAME_AREA_MARGIN, _engine.Height()-GAME_AREA_MARGIN, GAME_AREA_MARGIN, _engine.Height()-GAME_AREA_MARGIN));

    b2EdgeShape edge;
    b2FixtureDef fixture;
    fixture.shape = &edge;
    fixture.friction = 0;

    for(unsigned int i=0; i<_lines.size(); i++)
    {
        edge.Set(_lines[i]->Point0(), _lines[i]->Point1());
        if(i!=3)
            _body->CreateFixture(&fixture);
        else
            _ground->CreateFixture(&fixture);
    }

    _ball = new Ball(_engine, _physics);
    _paddle = new Paddle(_engine, _physics, *this);
}

GameArea::~GameArea(void)
{
    for(unsigned int i=0; i<_lines.size(); i++)
    {
        delete _lines[i];
    }
    if(_ball)
        delete _ball;
    if(_paddle)
        delete _paddle;
    _physics.World().DestroyBody(_body);
}

void GameArea::AddTexture( std::list<IRenderElement *> &elements )
{
    for(unsigned int i=0; i<_lines.size(); i++)
    {
        elements.push_back(_lines[i]);
    }
}

void GameArea::Step()
{
    _physics.Step();

    std::list<IRenderElement *> textures;
    textures.push_back(&_background);
    this->AddTexture(textures);
    _ball->AddTexture(textures);
    _paddle->AddTexture(textures);
    _engine.Render(textures);

}

void GameArea::SetMouseX( int x )
{
    _paddle->SetX(x);
}
