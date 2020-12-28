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



#define __GAME_NAME__ "Raycing by TersaKen Studio"
#define __GAME_VERSION__ "v0.1"

#include "engine_constants.h"



/*
  sprite_array[0] = player 0
  sprite_array[N-1] = player N-1
  sprite_array[N] to sprite_array[N+NB-1]
*/

Sprite sprite_array[NB_SPRITE_MAX] ;

int main(int argc, char* argv[])
{
    Level my_level("level.lvl") ;
    Player input(2, 2) ;

    sprite_array[0] = {};
    sprite_array[0].x = 2 ;
    sprite_array[0].y = 2 ;

    input.set_level(&my_level) ;
    Ray_engine inst_Ray_engine ;




    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;
        if (SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer) == 0) {
			       SDL_SetWindowTitle(window, __GAME_NAME__ " " __GAME_VERSION__);
            //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) ;
            SDL_bool done = SDL_FALSE;
            inst_Ray_engine.init(renderer, &my_level) ;
            while (input.get_leave()) {
              clock_t begin = clock();
              // Ctrl
              input.input_from_keyboard() ;
              std::cerr << "after input " << sprite_array[0].angle << std::endl;
              inst_Ray_engine.render(renderer) ;
              clock_t end = clock();
              //std::cerr << CLOCKS_PER_SEC/(double)(end - begin) << std::endl ;
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
