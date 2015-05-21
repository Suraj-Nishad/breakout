#include "GameArea.h"
#include "Texture.h"


void Line::Show( RenderEngine &engine )
{
    SDL_SetRenderDrawColor(engine.Handle(), 255,255,255,0);
    SDL_RenderDrawLine( engine.Handle(), _x0, _y0, _x1, _y1);
}

Line::Line(PhysicsSimulator &physics, int x0, int y0, int x1, int y1 )
{
    _x0 = x0;
    _y0 = y0;
    _x1 = x1;
    _y1 = y1;
    _point0.Set(physics.Pixel2Meter(x0), physics.Pixel2Meter(y0));
    _point1.Set(physics.Pixel2Meter(x1), physics.Pixel2Meter(y1));
}

GameArea::GameArea
(
    PhysicsSimulator &physics,
    RenderEngine &engine
 ) : _physics(physics)
{
    engine.CreateWindow("Breakout", 500, 700);

    _width = engine.Width()-2*GAME_AREA_MARGIN;    
    _height = engine.Height()-2*GAME_AREA_MARGIN;

    b2BodyDef body_def;
     _body = _physics.World().CreateBody(&body_def);

    _lines.push_back(new Line(physics, GAME_AREA_MARGIN, GAME_AREA_MARGIN, engine.Width()-GAME_AREA_MARGIN, GAME_AREA_MARGIN));
    _lines.push_back(new Line(physics, GAME_AREA_MARGIN, GAME_AREA_MARGIN, GAME_AREA_MARGIN, engine.Height()-GAME_AREA_MARGIN));
    _lines.push_back(new Line(physics, engine.Width()-GAME_AREA_MARGIN, engine.Height()-GAME_AREA_MARGIN, engine.Width()-GAME_AREA_MARGIN, GAME_AREA_MARGIN));
    _lines.push_back(new Line(physics, engine.Width()-GAME_AREA_MARGIN, engine.Height()-GAME_AREA_MARGIN, GAME_AREA_MARGIN, engine.Height()-GAME_AREA_MARGIN));

    b2EdgeShape edge;
    b2FixtureDef fixture;
    fixture.shape = &edge;
    fixture.friction = 0;

    for(unsigned int i=0; i<_lines.size(); i++)
    {
        edge.Set(_lines[i]->Point0(), _lines[i]->Point1());
        _body->CreateFixture(&fixture);
    }

}

GameArea::~GameArea(void)
{
    for(unsigned int i=0; i<_lines.size(); i++)
    {
        delete _lines[i];
    }
}

void GameArea::AddTexture( std::list<IRenderElement *> &elements )
{
    for(unsigned int i=0; i<_lines.size(); i++)
    {
        elements.push_back(_lines[i]);
    }
}
