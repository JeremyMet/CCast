#include "SDL2/SDL.h"
#include "player.h"
#include "const_trig.h"
#include <stdlib.h>
#include <iostream>
#include <string>
#include "bitmap.h"


/*#define PI 3.14159
#define to_radian(angle) (angle/180.0)*PI*/


// Engine Constants
#define __RAY_ENGINE_WALL_HEIGHT__ 2
#define NB_WALL_TEXTURE 2
#define NB_FLOOR_TEXTURE 2


class Ray_engine {


  private:

    // Engine Parameters
    static unsigned int height ;
    static unsigned int half_height ;
    static unsigned int width  ;
    static unsigned int FOV ;
    static unsigned int HALF_FOV ;
	static unsigned int cam_distance;
    static unsigned int angle_step ;
	static unsigned int fog;
	static unsigned int texture_height;
	static unsigned int texture_width;
	

    // Map

    static unsigned int** wall_map ;
	static unsigned int** floor_map ;
    static int map_height ;
    static int map_width ;

    // Display
 	
    static SDL_Texture * wall_texture[NB_FLOOR_TEXTURE] ;
	static SDL_Texture * background_texture ;
	

    static unsigned char* floor_surface[NB_FLOOR_TEXTURE];


  public:
	static void init(SDL_Renderer* renderer, Level* level_arg);
    static void render(SDL_Renderer* renderer, Player* my_player);

} ;





