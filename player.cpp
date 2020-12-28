

#include "player.h"


Player::Player() {
  this->leave = true ;
  this->x = 0 ;
  this->y = 0 ;
  this->angle = 0 ;
  this->speed_angle = __DEFAULT_ANGLE_SPEED__ ;
  this->speed_step = __DEFAULT_STEP_SPEED__ ;
  this->remaining_missile = 127;
  this->current_missile_index = 1;
}

Player::Player(float x, float y) {
  this->leave = true ;
  this->x = x ;
  this->y = y ;
  this->angle = 0 ;
  this->speed_angle = __DEFAULT_ANGLE_SPEED__ ;
  this->speed_step = __DEFAULT_STEP_SPEED__ ;
  this->remaining_missile = 127;
  this->current_missile_index = 1;

}

Player::~Player() { }


void Player::set_x(float x) {
  this->x = x ;
}

void Player::set_y(float y) {
  this->y = y ;
}

float Player::get_x() {
  return this->x ;
}

float Player::get_y() {
  return this->y ;
}

 int Player::get_angle() {
  return this->angle ;
}

bool Player::get_leave() {
  return this->leave ;
}



// input_from_keyboard
// input: void
// return: void.
#define __MAX_SPEED__ 15
#define __ACC__ 0.1
#define __DEC__ 0.2
void Player::input_from_keyboard() {
  static float speed = 5 ;
  SDL_Event event;
  int delta_s, delta_a, trigger_missile ;
  float new_x, new_y ;
  delta_s = delta_a = trigger_missile = 0 ;
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  SDL_PumpEvents();
  SDL_PollEvent(&event);
  if (state[SDL_SCANCODE_LEFT])  delta_a = -1 ;
  if (state[SDL_SCANCODE_RIGHT]) delta_a = 1  ;
  if (state[SDL_SCANCODE_UP]) delta_s = 1 ;
  if (state[SDL_SCANCODE_DOWN]) delta_s = -1 ;
  if (state[SDL_SCANCODE_SPACE]) trigger_missile = 1;
  if (event.type == SDL_QUIT) {
	  this->leave = false ;
	} // end if
  // Update coordinates.
  // /!\ For now, collision engine is in the Player class but should be moved soon.

  sprite_array[PLAYER_SPRITE_INDEX].angle += delta_a*this->speed_angle ;
  if (sprite_array[PLAYER_SPRITE_INDEX].angle < 0) { sprite_array[PLAYER_SPRITE_INDEX].angle += 3600 ; }
  if (sprite_array[PLAYER_SPRITE_INDEX].angle >= 3600) { sprite_array[PLAYER_SPRITE_INDEX].angle -= 3600 ; }

  //std::cerr << this->angle << std::endl ;

  sprite_array[PLAYER_SPRITE_INDEX].delta_x = delta_s*this->speed_step*const_trig::fast_cos(sprite_array[PLAYER_SPRITE_INDEX].angle)*speed ;
  sprite_array[PLAYER_SPRITE_INDEX].delta_y = delta_s*this->speed_step*const_trig::fast_sin(sprite_array[PLAYER_SPRITE_INDEX].angle)*speed ;
  if (trigger_missile) {
    std::cerr << current_missile_index << std::endl ;
    sprite_array[current_missile_index].active = true ;
    sprite_array[current_missile_index].x = sprite_array[PLAYER_SPRITE_INDEX].x ;
    sprite_array[current_missile_index].y = sprite_array[PLAYER_SPRITE_INDEX].y ;
    sprite_array[current_missile_index].delta_x = this->speed_step*const_trig::fast_cos(sprite_array[PLAYER_SPRITE_INDEX].angle)*30;
    sprite_array[current_missile_index].delta_y = this->speed_step*const_trig::fast_sin(sprite_array[PLAYER_SPRITE_INDEX].angle)*30;
    current_missile_index++;
    if (current_missile_index > 127) {
      current_missile_index = 1;
    } // end if.
  } // end if.

} // end function
