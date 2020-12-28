#include "SDL2/SDL.h"
#include "player.h"
#include "const_trig.h"
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cmath>
#include "bitmap.h"
#include "sprite.h"
#include "engine_constants.h"

/*#define PI 3.14159
#define to_radian(angle) (angle/180.0)*PI*/


extern Sprite sprite_array[NB_SPRITE_MAX];


class Ray_engine {


  private:

    // Engine Parameters
    static unsigned int height ;
    static unsigned int half_height ;
    static unsigned int width  ;
    static unsigned int FOV ;
    static unsigned int HALF_FOV ;
	static unsigned int wall_max_height;
    static unsigned int angle_step ;
	static unsigned int fog;
	static unsigned int texture_height;
	static unsigned int texture_width;


    // Map and Sprite
    static unsigned int** wall_map ;
	static unsigned int** floor_map ;
    static int map_height ;
    static int map_width ;
	// static Sprite sprite_array[NB_SPRITE_MAX];

    // Display
  static SDL_Texture * wall_texture[NB_WALL_TEXTURE] ;
	static SDL_Texture * sprite_texture[NB_SPRITE_TEXTURE] ;
	static SDL_Texture * background_texture ;
  static unsigned char* floor_surface[NB_FLOOR_TEXTURE];
	static float z_buffer[640] ; // taille entrée en dur ici, je pense qu'il va falloir que je mette un certain nombre de paramètres (height, width, fov, etc) comme #define ... à voir

//   {


  public:
	   Ray_engine() ;
     ~Ray_engine() ;
	    static void init(SDL_Renderer* renderer, Level* level_arg);
      static void render(SDL_Renderer* renderer);

} ;
