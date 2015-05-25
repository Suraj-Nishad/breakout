#include "GameControler.h"
#include "Texture.h"
#include "Ball.h"
#include "Paddle.h"
#include "Piece.h"
#include "GroundLine.h"
#include "Bonus.h"

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
    fixture.filter.categoryBits = GAME_OBJECT_GROUND; //this is not a ground line but act like a ground
    fixture.filter.maskBits = GAME_OBJECT_BALL; //only ball collide with this.
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
    std::list<Ball *>::iterator ball_itr;
    _physics.Step((float)timer_value/1000);

    if(_game_state == GAME_STATE_PLAYING && _balls.empty() == false)
    {
        ball_itr = _balls.begin();
        while(ball_itr != _balls.end())
        {
            if((*ball_itr)->IsDestroyed())
            {
                delete (*ball_itr);
                ball_itr = _balls.erase(ball_itr);
                if(_balls.empty())
                    BallOutOfGame();
            }
            else 
            {
                if((*ball_itr)->NotMoving())
                    (*ball_itr)->Start();
                ball_itr++;
            }
        }
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
            if((*itr)->HasBonus())
                _bonus.push_back(new Bonus(*this, *(*itr)));
            delete (*itr);
            itr = _pieces.erase(itr);
        }
    }
    if(_pieces.empty())
    {
        GameWin();
    }

    std::list<Bonus *>::iterator bonus_itr = _bonus.begin();
    while(bonus_itr != _bonus.end() && _balls.empty() == false)
    {
        if((*bonus_itr)->IsDestroyed() == false)
        {
            (*bonus_itr)->AddTexture(textures);
            bonus_itr++;
        }
        else
        {
            switch((*bonus_itr)->BonusType())
            {
            case BONUS_MULTIPLE_BALLS:
                {
                    Ball *front_ball = _balls.front();
                    front_ball->Replicates(_balls);
                }
                break;
            default:
                break;
            }
            delete (*bonus_itr);
            bonus_itr = _bonus.erase(bonus_itr);
        }
    }

    ball_itr = _balls.begin();
    while(ball_itr != _balls.end())
    {
        (*ball_itr)->AddTexture(textures);
        ball_itr++;
    }
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

    if(_game_state == GAME_STATE_NOT_PLAYING && _balls.empty() == false)
    {
        int paddle_x, paddle_y;
        _paddle->GetCenterPoint(paddle_x, paddle_y);
        _balls.front()->SetInitialPosition(paddle_x, paddle_y);
    }
}

void GameControler::DestroyGameObjects()
{
    ClearPieces();
    ClearBonus();
    ClearBalls();

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
        _balls.front()->Start();
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
    std::list<Ball *>::iterator b;
    for(b = _balls.begin(); b != _balls.end(); b++)
        (*b)->Stop();

    ClearBonus();
    
    _game_state = GAME_STATE_WIN;
}

void GameControler::GameOver()
{
    if(_game_state != GAME_STATE_WIN || _game_state != GAME_STATE_OVER)
    {
        ClearBonus();
        ClearBalls();
        _game_state = GAME_STATE_OVER;
    }
}

void GameControler::NewBallInGame()
{
    _lives_ball.pop_back();
    _game_state = GAME_STATE_NOT_PLAYING;
    _balls.push_back(new Ball(*this));
    int paddle_x, paddle_y;
    _paddle->GetCenterPoint(paddle_x, paddle_y);
    _balls.back()->SetInitialPosition(paddle_x, paddle_y);
}

void GameControler::BallOutOfGame()
{ 
    if(_game_state == GAME_STATE_PLAYING)
    {
        _game_state = GAME_STATE_LOSE;
        Music().PlayGameOver();
        while(_balls.empty() == false)
        {
            delete (_balls.front());
            _balls.pop_front();
        }

        if(_lives_ball.empty())
            GameOver();
    }
}

void GameControler::ClearBonus()
{
    for(std::list<Bonus *>::iterator itr = _bonus.begin();
        itr != _bonus.end();
        itr++)
    {
        delete (*itr);
    }
    _bonus.clear();
}

void GameControler::ClearPieces()
{
    for(std::list<Piece *>::iterator itr = _pieces.begin();
        itr != _pieces.end();
        itr++)
    {
        delete (*itr);
    }
    _pieces.clear();
}

void GameControler::ClearBalls()
{
    while(_balls.empty() == false)
    {
        delete (_balls.front());
        _balls.pop_front();
    }
}
