#include "GameArea.h"
#include "Texture.h"
#include "Ball.h"
#include "Paddle.h"
#include "Piece.h"

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
    Piece::LoadPNG(_engine);
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


    int x, y, max_width;
    for(int i=0; i<4; i++)
    {
        x = GAME_AREA_MARGIN;
        y = GAME_AREA_MARGIN + i*Piece::PNGHeight();
        max_width = _width;
        
        while(max_width > 0) 
        {
            Piece *piece = new Piece(_engine, _physics, x, y, max_width);
            x += piece->Width();
            max_width -= piece->Width();
            _pieces.push_back(piece);
        }
    }
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

void GameArea::Step(Uint32 timer_value)
{
    _physics.Step((float)timer_value/1000);

    std::list<IRenderElement *> textures;    
    
    for(unsigned int i=0; i<_lines.size(); i++)
    {
        textures.push_back(_lines[i]);
    }

    textures.push_back(&_background);

    std::list<Piece *>::iterator itr = _pieces.begin();
    while(itr != _pieces.end())
    {
        if((*itr)->Destroyed() == false)
        {
            (*itr)->AddTexture(textures);
            itr++;
        }
        else
        {
            delete (*itr);
            itr = _pieces.erase(itr);
        }        
    }

    _ball->AddTexture(textures);
    _paddle->AddTexture(textures);

    _engine.Render(textures);

}

void GameArea::SetMouseX( int x )
{
    _paddle->SetX(x);
}
