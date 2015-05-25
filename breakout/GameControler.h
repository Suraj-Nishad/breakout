#pragma once

#include "PhysicsSimulator.h"
#include "RenderEngine.h"
#include "MusicPlayer.h"
#include "Texture.h"
#include "PNGImage.h"
#include <vector>
#include <list>

class Ball;
class Paddle;
class Piece;
class GroundLine;
class Bonus;

class Line : public IRenderElement
{
public:
    Line(PhysicsSimulator &physics, int x0, int y0, int x1, int y1);

    virtual void Show( RenderEngine &engine );

    inline b2Vec2 &Point0() {return _point0;}
    inline b2Vec2 &Point1() {return _point1;}

    inline void SetColor(int r, int g, int b) {_r = r; _g = g; _b = b;}

protected:
    int _x0, _y0, _x1, _y1;
    int _r, _g, _b;
    b2Vec2 _point0, _point1;
};

#define GAME_AREA_MARGIN 20

typedef enum
{
    GAME_STATE_NOT_PLAYING = 0,
    GAME_STATE_PLAYING,
    GAME_STATE_LOSE,
    GAME_STATE_WIN,
    GAME_STATE_OVER
} GAME_STATE;
class GameControler
{
public:
    GameControler();
    virtual ~GameControler(void);

    void CreateGameObjects();
    void DestroyGameObjects();

    void NewBallInGame();
    void BallOutOfGame();

    void GameWin();
    void GameOver();

    void SetMouseX(int x);
    void MouseClick();

    void Step(Uint32 timer_value);


    inline int WidthPixel() {return _width;}
    inline int HeightPixel() {return _height;}
    inline int MarginPixel() {return GAME_AREA_MARGIN;}

    inline float WidthMeter() {return _physics.Pixel2Meter(_width);}
    inline float HeightMeter() {return _physics.Pixel2Meter(_height);}
    inline float MarginMeter() {return _physics.Pixel2Meter(GAME_AREA_MARGIN);}

    inline PhysicsSimulator &Physics() {return _physics;}
    inline RenderEngine &Renderer() {return _engine;}
    inline MusicPlayer &Music() {return _music;}

protected:
    RenderEngine _engine;
    PhysicsSimulator _physics;
    MusicPlayer _music;
    PNGImage _background;

    b2Body *_body;
    int _width, _height;
    std::vector<Line *> _lines;

    std::list<Piece *> _pieces;
    std::list<Bonus *> _bonus;
    Ball *_ball;
    Paddle *_paddle;
    GroundLine *_ground;

    GAME_STATE _game_state;

    std::vector<Texture *> _lives_ball;
};


