

#include "player.h"


Player::Player() {
  this->leave = true ;
  this->x = 0 ;
  this->y = 0 ;
  this->angle = 0 ;
  this->speed_angle = __DEFAULT_ANGLE_SPEED__ ;
  this->speed_step = __DEFAULT_STEP_SPEED__ ;
}

Player::Player(float x, float y) {
  this->leave = true ;
  this->x = x ;
  this->y = y ;
  this->angle = 0 ;
  this->speed_angle = __DEFAULT_ANGLE_SPEED__ ;
  this->speed_step = __DEFAULT_STEP_SPEED__ ;
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

void Player::set_level(Level* my_level) {
  this->current_level = my_level;
}


// input_from_keyboard
// input: void
// return: void.
#define __MAX_SPEED__ 15
#define __ACC__ 0.1
#define __DEC__ 0.2
void Player::input_from_keyboard() {
  static float speed = 0 ;
  SDL_Event event;
  int delta_s, delta_a ;
  float new_x, new_y ;
  delta_s = delta_a = 0 ;
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  SDL_PumpEvents();
  SDL_PollEvent(&event);
  if (state[SDL_SCANCODE_LEFT])  delta_a = -1 ;
  if (state[SDL_SCANCODE_RIGHT]) delta_a = 1  ;
  if (state[SDL_SCANCODE_UP]) delta_s = 1 ;
  if (state[SDL_SCANCODE_DOWN]) delta_s = -1 ;
  if (event.type == SDL_QUIT) {
	  this->leave = false ;
	} // end if
  // Update coordinates.
  // /!\ For now, collision engine is in the Player class but should be moved soon.

  if (delta_s == 0) {
    speed -= __DEC__ ;
    if (speed < 0) {speed = 0;}
  }

  std::cerr << sprite_array[0].angle << std::endl;

  speed += delta_s*__ACC__ ;
  if (speed > __MAX_SPEED__) { speed = __MAX_SPEED__ ;}

  unsigned int** wall_array = this->current_level->get_wall_array() ;
  unsigned int map_width = this->current_level->get_width() ;
  unsigned int map_height = this->current_level->get_height() ;

  sprite_array[0].angle += delta_a*this->speed_angle ;
  if (sprite_array[0].angle) { sprite_array[0].angle += 3600 ; }
  if (sprite_array[0].angle >= 3600) { sprite_array[0].angle -= 3600 ; }

  //std::cerr << this->angle << std::endl ;

  new_x = sprite_array[0].x+this->speed_step*const_trig::fast_cos(sprite_array[0].angle)*speed ;
  if ((new_x >= 0 && new_x < map_width) && !wall_array[(int) sprite_array[0].y][(int) new_x]) { sprite_array[0].x = new_x ;}
  new_y = sprite_array[0].y+this->speed_step*const_trig::fast_sin(sprite_array[0].angle)*speed ;
  if ((new_y >= 0 && new_y < map_height) && !wall_array[(int) new_y][(int) sprite_array[0].x]) { sprite_array[0].y = new_y ;}



} // end function
