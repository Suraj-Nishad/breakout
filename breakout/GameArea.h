#pragma once

#include "PhysicsSimulator.h"
#include "RenderEngine.h"
#include "Texture.h"

#include <vector>
#include <list>

class Line : public IRenderElement
{
public:
    Line(PhysicsSimulator &physics, int x0, int y0, int x1, int y1);

    virtual void Show( RenderEngine &engine );

    inline b2Vec2 &Point0() {return _point0;}
    inline b2Vec2 &Point1() {return _point1;}

protected:
    int _x0, _y0, _x1, _y1;
    b2Vec2 _point0, _point1;
};

class GameArea
{
public:
    GameArea(PhysicsSimulator &physics, int width, int height);
    virtual ~GameArea(void);

    inline int WidthPixel() {return _width;}
    inline int HeightPixel() {return _height;}

    inline float WidthMeter() {return _physics.Pixel2Meter(_width);}
    inline float HeightMeter() {return _physics.Pixel2Meter(_height);}

    void AddTexture(std::list<IRenderElement *> &elements);

protected:
    b2Body *_body;
    PhysicsSimulator &_physics;
    int _width, _height;
    std::vector<Line *> _lines;

};


