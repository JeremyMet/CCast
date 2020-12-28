

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <math.h>


#include "SDL2/SDL.h"
#include "level.h"
#include "const_trig.h"
#include "level.h"
#include "sprite.h"
#include "engine_constants.h"

#include <iostream>


#define __DEFAULT_ANGLE_SPEED__ 11 // 1 does actually mean 0.1 degree
#define __DEFAULT_STEP_SPEED__ 0.01
#define __NB_AMMO__ 128

extern Sprite sprite_array[NB_SPRITE_MAX];

class Player {

  private:

    bool leave ;
    float x ;
    float y ;
    float speed_step ;
    int angle, speed_angle ;
	  Level* current_level;


  public:

    Player() ;
    Player(float x, float y);
    ~Player();

    void set_x(float x);
    void set_y(float y);
    float get_x();
    float get_y() ;
    int get_angle() ;
    bool get_leave();
    void set_level(Level* my_level);
    void input_from_keyboard();


} ;

#endif
