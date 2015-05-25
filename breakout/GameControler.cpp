#include "GameControler.h"
#include "Texture.h"
#include "Ball.h"
#include "Paddle.h"
#include "Piece.h"
#include "GroundLine.h"

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

GameControler::GameControler() : _background("MCTestTaskBackground.png")
{
    _engine.CreateWindow("Breakout", 500, 700);
    Piece::LoadPNG(_engine);
    Ball::LoadPNG(_engine);

    _background.Load(_engine);

    _width = _engine.Width()-2*GAME_AREA_MARGIN;    
    _height = _engine.Height()-2*GAME_AREA_MARGIN;

    b2BodyDef body_def;
    _body = _physics.World().CreateBody(&body_def);

    _lines.push_back(new Line(_physics, GAME_AREA_MARGIN-1, GAME_AREA_MARGIN-1, _engine.Width()-GAME_AREA_MARGIN, GAME_AREA_MARGIN-1));
    _lines.push_back(new Line(_physics, _engine.Width()-GAME_AREA_MARGIN, GAME_AREA_MARGIN-1, _engine.Width()-GAME_AREA_MARGIN, _engine.Height()));
    _lines.push_back(new Line(_physics, GAME_AREA_MARGIN-1, GAME_AREA_MARGIN-1, GAME_AREA_MARGIN-1, _engine.Height()));

    b2EdgeShape edge;
    b2FixtureDef fixture;
    fixture.shape = &edge;
    fixture.friction = 0;

    for(unsigned int i=0; i<_lines.size(); i++)
    {
        edge.Set(_lines[i]->Point0(), _lines[i]->Point1());
        _body->CreateFixture(&fixture);
    }
        
    _ground = new GroundLine(*this);

    CreateGameObjects();
}

GameControler::~GameControler(void)
{
    for(unsigned int i=0; i<_lines.size(); i++)
    {
        delete _lines[i];
    }
    if(_ground)
        delete _ground;

    DestroyGameObjects();

    _physics.World().DestroyBody(_body);
}

void GameControler::Step(Uint32 timer_value)
{
    _physics.Step((float)timer_value/1000);

    if(_game_state == GAME_STATE_PLAYING && _ball != NULL)
    {
        if(_ball->IsDestroyed())
            BallOutOfGame();
        else if(_ball->NotMoving())
            _ball->Start();
    }
    

    std::vector<IRenderElement *> textures;
    //first load the background image
    textures.push_back(&_background);

    //then we push all the balls that represent lives
    for(unsigned int i=0; i<_lives_ball.size(); i++)
    {
        textures.push_back(_lives_ball[i]);
    }


    std::list<Piece *>::iterator itr = _pieces.begin();
    while(itr != _pieces.end())
    {
        if((*itr)->IsDestroyed() == false)
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

    if(_pieces.empty())
    {
        GameWin();
    }

    if(_ball) 
        _ball->AddTexture(textures);
    _paddle->AddTexture(textures);

    for(unsigned int i=0; i<_lines.size(); i++)
    {
        textures.push_back(_lines[i]);
    }

    _engine.Render(textures);

}

void GameControler::SetMouseX( int x )
{
    _paddle->SetX(x);

    if(_game_state == GAME_STATE_NOT_PLAYING && _ball != NULL)
    {
        int paddle_x, paddle_y;
        _paddle->GetCenterPoint(paddle_x, paddle_y);
        _ball->SetInitialPosition(paddle_x, paddle_y);
    }
}

void GameControler::DestroyGameObjects()
{
    for(std::list<Piece *>::iterator itr = _pieces.begin();
        itr != _pieces.end();
        itr++)
    {
        delete (*itr);
    }
    _pieces.clear();
    if(_ball)
        delete _ball;
    _ball = NULL;
    if(_paddle)
        delete _paddle;
    _paddle = NULL;
}

void GameControler::CreateGameObjects()
{
    _paddle = new Paddle(*this);
    
    for(int i=0; i<3; i++)
    {
        _lives_ball.push_back(new WeakCopyTexture(Ball::Image()));
        _lives_ball.back()->SetPosition(GAME_AREA_MARGIN + 2*i*GAME_AREA_MARGIN, 0);
    }


    int x, y, max_width;
    for(int i=0; i<4; i++)
    {
        x = GAME_AREA_MARGIN;
        y = GAME_AREA_MARGIN + i*Piece::PNGHeight();
        max_width = _width;

        while(max_width > 0) 
        {
            Piece *piece = new Piece(*this, x, y, max_width);
            x += piece->Width();
            max_width -= piece->Width();
            _pieces.push_back(piece);
        }
    }
    NewBallInGame();
    Music().PlayGameStart();
}

void GameControler::MouseClick()
{
    switch(_game_state)
    {
    case GAME_STATE_NOT_PLAYING:
        _ball->Start();
        _game_state = GAME_STATE_PLAYING;
        break;
    case GAME_STATE_LOSE:
        NewBallInGame();
        break;
    case GAME_STATE_WIN:
    case GAME_STATE_OVER:
        DestroyGameObjects();
        CreateGameObjects();
        break;
    default:
        break;
    }
}

void GameControler::GameWin()
{
    _ball->Stop();
    _game_state = GAME_STATE_WIN;
}

void GameControler::GameOver()
{
    if(_game_state != GAME_STATE_WIN || _game_state != GAME_STATE_OVER)
    {
        delete _ball;
        _ball = NULL;
        _game_state = GAME_STATE_OVER;
    }
}

void GameControler::NewBallInGame()
{
    _lives_ball.pop_back();
    _game_state = GAME_STATE_NOT_PLAYING;
    _ball = new Ball(*this);
    int paddle_x, paddle_y;
    _paddle->GetCenterPoint(paddle_x, paddle_y);
    _ball->SetInitialPosition(paddle_x, paddle_y);
}

void GameControler::BallOutOfGame()
{ 
    _game_state = GAME_STATE_LOSE;
    Music().PlayGameOver();
    delete _ball;
    _ball = NULL;
    if(_lives_ball.empty())
        GameOver();
}
