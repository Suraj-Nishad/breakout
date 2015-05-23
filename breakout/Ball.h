#pragma once

#include "GameObject.h"
#include "PNGImage.h"

class Ball : public GameObject
{
public:
    Ball(GameArea &game);

    virtual ~Ball(void);

    inline bool Destroyed() {return _destroyed;}

    inline void Destroyed(bool value) {_destroyed = value;}

protected:

    virtual Texture * GetTexture();

    virtual void EndContact( IContactObject *another_object );


    bool _destroyed;

    PNGImage _ball_png;
};

