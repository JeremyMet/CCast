#include <sstream>
#include <string>
#include <unistd.h>
#include <math.h>


#include <time.h>
#include <iostream>

#include "SDL2/SDL.h"


#include "level.h"
#include "player.h"
#include "ray_engine.h"
#include "move_and_collide.h"


#define __GAME_NAME__ "Raycing by TersaKen Studio"
#define __GAME_VERSION__ "v0.1"

#include "engine_constants.h"



/*
  sprite_array[0] = player 0
  sprite_array[N-1] = player N-1
  sprite_array[N] to sprite_array[N+NB-1]
*/

Sprite sprite_array[NB_SPRITE_MAX] ;
Level current_level("level.lvl");

int main(int argc, char* argv[])
{
    Player input(2, 2) ;

    sprite_array[0] = {};
    sprite_array[0].x = 2 ;
    sprite_array[0].y = 2 ;
    sprite_array[0].active = true ;
    Ray_engine inst_Ray_engine ;

    for(int i=1;i<128;i++) {
      sprite_array[i] = {};
      sprite_array[i].type = 1;
    }


    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;
        if (SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer) == 0) {
			       SDL_SetWindowTitle(window, __GAME_NAME__ " " __GAME_VERSION__);
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) ;
            SDL_bool done = SDL_FALSE;
            inst_Ray_engine.init(renderer) ;
            while (input.get_leave()) {
              clock_t begin = clock();
              // Ctrl
              input.input_from_keyboard() ;
              move_and_collide();
              inst_Ray_engine.render(renderer) ;
              clock_t end = clock();
              // std::cerr << CLOCKS_PER_SEC/(double)(end - begin) << std::endl ;
            } // end main loop

        }

        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
    }

    SDL_Quit();
    return 0;
}
