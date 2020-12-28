#ifndef __MOVE_AND_COLLIDE_H__
#define __MOVE_AND_COLLIDE_H__

#include "engine_constants.h"
#include "sprite.h"
#include "level.h"

extern Sprite sprite_array[NB_SPRITE_MAX];
extern Level current_level;

void move_and_collide();

#endif
