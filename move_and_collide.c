#include "move_and_collide.h"

#include <iostream>

void move_and_collide() {
  unsigned int map_width = current_level.get_width();
  unsigned int map_height = current_level.get_height();
  unsigned int** wall_array = current_level.get_wall_array();
  for(int i=0;i<NB_SPRITE_MAX;i++) {
    if (sprite_array[i].active) {
      float new_x = sprite_array[i].x + sprite_array[i].delta_x;
      float new_y = sprite_array[i].y + sprite_array[i].delta_y;
      if (i==PLAYER_SPRITE_INDEX) { //
        if ((new_x >= 0 && new_x < map_width) && !wall_array[(int) sprite_array[PLAYER_SPRITE_INDEX].y][(int) new_x]) { sprite_array[PLAYER_SPRITE_INDEX].x = new_x ;}
        if ((new_y >= 0 && new_y < map_height) && !wall_array[(int) new_y][(int) sprite_array[PLAYER_SPRITE_INDEX].x]) { sprite_array[PLAYER_SPRITE_INDEX].y = new_y ;}
        sprite_array[PLAYER_SPRITE_INDEX].delta_x = 0 ;
        sprite_array[PLAYER_SPRITE_INDEX].delta_y = 0 ;
      }
      else {
        if (new_x < 0 || new_x >= map_width || new_y < 0 || new_y >= map_height) {
          sprite_array[i].active = false;
        }
        else if (wall_array[(int) new_y][(int) new_x]) {
          sprite_array[i].active = false;
        }
        else {
          sprite_array[i].x = new_x;
          sprite_array[i].y = new_y;
        }
      } // end if.
    } // end if.
  } // end for.
}
