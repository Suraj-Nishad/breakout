#include "GameControler.h"
#include "Texture.h"
#include "Ball.h"
#include "Paddle.h"
#include "Piece.h"
#include "GroundLine.h"
#include "Bonus.h"

#define USEREVENT_NEW_FRAME 0
#define USEREVENT_AUTO_MOUSE_CLICK 1


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

GameControler::GameControler() : _background("MCTestTaskBackground.png"), _score_board(_engine)
{
    //Create the game window
    _engine.CreateWindow("Breakout", 500, 700);

    //pre load PNG files.
    Piece::LoadPNG(_engine);
    Ball::LoadPNG(_engine);
    _background.Load(_engine);

    //game area should be smaller than actual window area.
    _width = _engine.Width()-2*GAME_AREA_MARGIN;    
    _height = _engine.Height()-2*GAME_AREA_MARGIN;

    //create a static object and delimit it with side lines.
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
        
    //Ground line is a special object and it is invisible.
    _ground = new GroundLine(*this);
    _game_state = GAME_STATE_NOT_PLAYING;

    CreateGameObjects();

    _refresh_frame_timer_id = SDL_AddTimer(15, GameControler::TimerCallBack, (void *)USEREVENT_NEW_FRAME);
    _auto_mouse_click_timer_id = -1;
}

GameControler::~GameControler(void)
{
    for(unsigned int i=0; i<_lines.size(); i++)
    {
        delete _lines[i];
    }
    if(_ground)
        delete _ground;

    SDL_RemoveTimer(_refresh_frame_timer_id);
    SDL_RemoveTimer(_auto_mouse_click_timer_id);

    DestroyGameObjects();

    _physics.World().DestroyBody(_body);
}

void GameControler::Step(Uint32 timer_value)
{
    //Step the physics simulation
    _physics.Step((float)timer_value/1000);  

    //and now iterates over all elements and create the textures vector to render.
    std::vector<IRenderElement *> textures;

    //first load the background image
    textures.push_back(&_background);

    //then we push all the balls that represent lifes
    for(unsigned int i=0; i<_lifes_ball.size(); i++)
    {
        textures.push_back(_lifes_ball[i]);
    }

    //check all the pieces
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
            _score_board.Add((*itr)->Width() / 10);
            if((*itr)->HasBonus())
                _bonus.push_back(new Bonus(*this, *(*itr)));
            delete (*itr);
            itr = _pieces.erase(itr);
        }
    }

    //if no more pieces, user win!!
    if(_pieces.empty())
    {
        GameWin();
    }

    //check all bonus that are "falling"
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
            ApplyBonus((*bonus_itr)->BonusType());

            delete (*bonus_itr);
            bonus_itr = _bonus.erase(bonus_itr);
        }
    }

    //check for ball(s) state.
    if(_balls.empty() == false)
    {
        //check for the ball(s) state
        std::list<Ball *>::iterator ball_itr = _balls.begin();
        while(ball_itr != _balls.end())
        {
            if((*ball_itr)->IsDestroyed())
            {
                //Destroy the ball
                delete (*ball_itr);
                ball_itr = _balls.erase(ball_itr);

                //if no more balls in game, user lose!
                if(_balls.empty())
                    BallOutOfGame();
            }
            else 
            {
                //check if balls is still moving......
                if(_game_state == GAME_STATE_PLAYING && (*ball_itr)->NotMoving())
                    (*ball_itr)->Start();

                (*ball_itr)->AddTexture(textures);
                ball_itr++;
            }
        }
    }


    //add the paddle...
    _paddle->AddTexture(textures);

    //.. and the side lines
    for(unsigned int i=0; i<_lines.size(); i++)
        textures.push_back(_lines[i]);

    //.. and the score board
    textures.push_back(&_score_board);


    //render everything!
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

    for(unsigned int i=0;i<_lifes_ball.size(); i++)
        delete _lifes_ball[i];
    _lifes_ball.clear();

    if(_paddle)
        delete _paddle;

    _paddle = NULL;
}

void GameControler::CreateGameObjects( unsigned int number_of_life /*= 3*/ )
{
    //Add paddle...
    _paddle = new Paddle(*this);
    
    //.. a number of balls that represent lifes.
    for(unsigned int i=0; i<number_of_life; i++)
    {
        AddNewLife();
    }
    
    //Fill the top with 4 rows of pieces.
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
    
    //Add a new ball to the game...
    NewBallInGame();
    Music().PlayGameStart();
}

void GameControler::MouseClick()
{
    if(_auto_mouse_click_timer_id != -1)
    {
        SDL_RemoveTimer(_auto_mouse_click_timer_id);
        _auto_mouse_click_timer_id = -1;
    }
    switch(_game_state)
    {
    case GAME_STATE_NOT_PLAYING:
        //Waiting user to start!
        _balls.front()->Start();
        _game_state = GAME_STATE_PLAYING;
        break;
    case GAME_STATE_LOSE:
        //Add a new ball to the game...
        _paddle->Show();
        NewBallInGame();
        break;
    case GAME_STATE_WIN:
        {
            //users won. Recreate game objects with the number of lifes.
            unsigned int lifes = _lifes_ball.size();
            DestroyGameObjects();
            CreateGameObjects(lifes+1);
        }
        break;
    case GAME_STATE_OVER:
        //Restart the game!
        DestroyGameObjects();
        CreateGameObjects();
        _score_board.Reset();
        break;
    default:
        break;
    }
}

void GameControler::GameWin()
{
    if(_game_state == GAME_STATE_PLAYING)
    {
        //User won! Stop all balls
        std::list<Ball *>::iterator b;
        for(b = _balls.begin(); b != _balls.end(); b++)
            (*b)->Stop();

        //Drop all running bonuses
        ClearBonus();
    
        _game_state = GAME_STATE_WIN;
        Music().PlayWin();
        _auto_mouse_click_timer_id = SDL_AddTimer(3000, TimerCallBack, (void *)USEREVENT_AUTO_MOUSE_CLICK);
    }
}

void GameControler::GameOver()
{
    if(_game_state != GAME_STATE_WIN || _game_state != GAME_STATE_OVER)
    {
        //GAME OVER!! Drops all bonus and cleanup all balls.
        ClearBonus();
        ClearBalls();
        _game_state = GAME_STATE_OVER;
    }
}

void GameControler::NewBallInGame()
{
    //Pop one life (ball) and put it to the game!
    _lifes_ball.pop_back();
    _game_state = GAME_STATE_NOT_PLAYING;
    _balls.push_back(new Ball(*this));

    //put balls at the top of paddle and wait user to click to start.
    int paddle_x, paddle_y;
    _paddle->GetCenterPoint(paddle_x, paddle_y);
    _balls.back()->SetInitialPosition(paddle_x, paddle_y);
}

void GameControler::BallOutOfGame()
{ 
    if(_game_state == GAME_STATE_PLAYING)
    {
        //no more balls in game. User lose!!!
        _game_state = GAME_STATE_LOSE;
        Music().PlayGameOver();
        _paddle->Disapear();

        //do some cleanups
        ClearBalls();

        //if no more lifes, game is over!
        if(_lifes_ball.empty())
            GameOver();

        _auto_mouse_click_timer_id = SDL_AddTimer(3000, TimerCallBack, (void *)USEREVENT_AUTO_MOUSE_CLICK);
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

void GameControler::ApplyBonus( BONUS_TYPE type )
{
    switch(type)
    {
    case BONUS_MULTIPLE_BALLS:
        {
            Ball *front_ball = _balls.front();
            front_ball->Replicates(_balls);
            Music().PlayBonus();
        }
        break;
    case BONUS_EXTRA_LIFE:
        {
            AddNewLife();

            Music().PlayExtraLife();
        }
        break;
    default:
        break;
    }
}

void GameControler::AddNewLife()
{
    _lifes_ball.push_back(new WeakCopyTexture(Ball::Image()));
    _lifes_ball.back()->SetPosition(WidthPixel() - 2*(_lifes_ball.size()-1)*GAME_AREA_MARGIN, 0);
}

Uint32 GameControler::TimerCallBack( Uint32 interval, void *param )
{
    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = (Sint32)param;
    userevent.data1 = (void *)interval;
    userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    if(userevent.code == USEREVENT_NEW_FRAME)
       return(interval);
    else
       return 0;
}

void GameControler::HandleUserEvent(const SDL_UserEvent &e)
{
    switch(e.code)
    {
    case USEREVENT_NEW_FRAME:
        Step((Uint32)e.data1);
        break;
    case USEREVENT_AUTO_MOUSE_CLICK:
        _auto_mouse_click_timer_id = -1;
        MouseClick();
        break;
    default:
        break;
    }
}