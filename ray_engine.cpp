#include "ray_engine.h"


#include <iomanip>      // std::setprecision

#define ABS(x) (((x) < 0) ? (-(x)):(x))

unsigned int Ray_engine::height = 480 ;
unsigned int Ray_engine::half_height = 120 ;
unsigned int Ray_engine::width = 640 ;
unsigned int Ray_engine::FOV = 640 ; // 64� angle
unsigned int Ray_engine::HALF_FOV = 320 ;
unsigned int Ray_engine::wall_max_height = 240;
unsigned int Ray_engine::angle_step = Ray_engine::FOV/Ray_engine::width ;
SDL_Texture* Ray_engine::wall_texture[NB_WALL_TEXTURE];
unsigned char* Ray_engine::floor_surface[NB_FLOOR_TEXTURE];
SDL_Texture * Ray_engine::sprite_texture[NB_SPRITE_TEXTURE] ;
//Sprite Ray_engine::sprite_array[NB_SPRITE_MAX];
float Ray_engine::z_buffer[640] ; // to DO, change 640


SDL_Texture* Ray_engine::background_texture = NULL ;


unsigned int** Ray_engine::wall_map = NULL ;
unsigned int** Ray_engine::floor_map = NULL ;
int Ray_engine::map_width = 0 ;
int Ray_engine::map_height = 0 ;
unsigned int Ray_engine::texture_height = 128;
unsigned int Ray_engine::texture_width = 128;
unsigned int Ray_engine::fog = 10;

// Sprite Initialization here, should be done from the init function through specific file loading ----

/*Sprite Ray_engine::sprite_array[NB_SPRITE_MAX];
(Ray_engine::sprite_array[0]).set_type(0);*/


typedef struct {
		unsigned int type;
		float dist_with_sprite;
		float tan_current_angle_with_sprite;
} zbuffer_sprite_t ;
zbuffer_sprite_t zbuffer_sprite_array[NB_SPRITE_MAX];
void sort(zbuffer_sprite_t* array, unsigned int len_array);

// ----------------------------------------------------------------------------------------------------


Ray_engine::Ray_engine() {
	sprite_array[2].type = 0;
	sprite_array[2].x = 1.4;
	sprite_array[2].y = 25.0;

	sprite_array[1].type = 0;
	sprite_array[1].x = 1.4;
	sprite_array[1].y = 13;

}

void Ray_engine::init(SDL_Renderer* renderer, Level* level_arg)
{
	half_height = height >> 1 ; // to be changed => half to static
	HALF_FOV = FOV >> 1 ;
	angle_step = FOV/width ;
	/* map part */
	wall_map = level_arg->get_wall_array() ;
	floor_map = level_arg->get_floor_array() ;
	map_width = level_arg->get_width() ;
	map_height = level_arg->get_height() ;
	/* Wall Textures Loading */
	for(int i=0;i<NB_WALL_TEXTURE;i++) {
		std::string path = "media/img/wall/wall_" ;
		std::string file_number = std::to_string(i);
		std::string extension = ".bmp" ;
		std::string full_path = path+file_number+extension;
		SDL_Surface* image = SDL_LoadBMP(full_path.c_str());
		wall_texture[i] = SDL_CreateTextureFromSurface(renderer, image);
	}
	SDL_Surface* image = SDL_LoadBMP("media/img/background.bmp");
	background_texture = SDL_CreateTextureFromSurface(renderer, image);
	for(int i=0;i<NB_FLOOR_TEXTURE;i++) {
		std::string path = "media/img/floor/floor_" ;
		std::string file_number = std::to_string(i);
		std::string extension = ".bmp" ;
		std::string full_path = path+file_number+extension;
		floor_surface[i] = readBMP(full_path.c_str());
	}
	for(int i=0;i<NB_SPRITE_TEXTURE;i++) {
		std::string path = "media/img/sprite/sprite_" ;
		std::string file_number = std::to_string(i);
		std::string extension = ".bmp" ;
		std::string full_path = path+file_number+extension;
		SDL_Surface* image = SDL_LoadBMP(full_path.c_str());
		SDL_SetColorKey(image, SDL_TRUE, 0);

	//	image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ARGB8888, 0);
		sprite_texture[i] = SDL_CreateTextureFromSurface(renderer, image);
		SDL_SetTextureBlendMode(sprite_texture[0] , SDL_BLENDMODE_BLEND);


	}
}

Ray_engine::~Ray_engine() {
	for(int i=0;i<NB_FLOOR_TEXTURE;i++) { delete[] floor_surface[i] ; }
}

void Ray_engine::render(SDL_Renderer* renderer){
	static unsigned int half_width = width >> 1 ;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer) ;
	////////////////////////
	// Background Display //
	////////////////////////
	SDL_Rect Source = {0, 0, int(width), int(height)} ;
	SDL_Rect Dest = {0, 0, int(width), int(height)} ;
	SDL_RenderCopy(renderer, background_texture, &Source, &Dest) ;
	/////////////////////
	//  Initialization //
	/////////////////////
	float player_angle = sprite_array[0].angle ;
	std::cerr << "versus " << player_angle << std::endl;
	float x = sprite_array[0].x ;
	float y = sprite_array[0].y ;
	int angle = (player_angle-HALF_FOV) ;
	if (angle > 3600) { angle -= 3600 ; }
	if (angle < 0) { angle += 3600 ; }
	// For each ray ... !
	for(int w = 0 ; w < width ; w++) {
		//////////////////////////////
		// Variable Initialization //
		/////////////////////////////
		float x_ray_length, y_ray_length ; // position of the current ray
		float x_ray_vector = const_trig::fast_cos(angle) ;
		float y_ray_vector = const_trig::fast_sin(angle) ;
		float length_x_ray_vector = fabs(1.0/x_ray_vector) ;
		float length_y_ray_vector = fabs(1.0/y_ray_vector) ;


		int x_map = (int) x ;
		int y_map = (int) y ;
		///////////////////////////////
		// Initialization before DDA //
		///////////////////////////////
		int delta_x, delta_y ;
		// Starting with x_ray
		if (x_ray_vector < 0) {
		  delta_x = -1 ;
		  x_ray_length = (x-int(x))*length_x_ray_vector ;
		}
		else {
		  delta_x = 1 ;
		  x_ray_length = (1+int(x)-x)*length_x_ray_vector ;
		}
		// Then with y_ray
		if (y_ray_vector < 0) {
		  delta_y = -1 ;
		  y_ray_length = (y-int(y))*length_y_ray_vector ;
		}
		else {
		  delta_y = 1 ;
		  y_ray_length = (1+int(y)-y)*length_y_ray_vector ;
		}
		//////////////////
		// Running DDA //
		/////////////////
		bool hit = false ;
		bool x_side = false ; // true if ray cross a x-axis in the end.
		int cpt = 0 ;
		unsigned int tile_type = 0 ;
		while(!hit) {
		  if (x_ray_length < y_ray_length) {
			x_map+= delta_x ;
			x_ray_length += length_x_ray_vector ;
			x_side = false ;
		  }
		  else {
			y_map+= delta_y ;
			y_ray_length += length_y_ray_vector ;
			x_side = true ;
		  }
		  if (x_map < 0 || x_map >= map_width) { hit = true ;  }
		  else if (y_map < 0 || y_map >= map_height)  { hit = true ; }
		  else if (wall_map[y_map][x_map]) { hit = true ;  tile_type = wall_map[y_map][x_map] ; }
		}


		/////////////////////////////
		// Distance Computing part //
		/////////////////////////////
		float x_edge, y_edge ;
		x_ray_length -= length_x_ray_vector ;
		y_ray_length -= length_y_ray_vector ;
		float ray_length ;
		float fisheye_correction = const_trig::fast_cos(abs(angle-player_angle)) ;
		if (x_side) { ray_length = y_ray_length ; } else { ray_length = x_ray_length ; }
		x_edge =  ((x+x_ray_vector*ray_length)) ;
		y_edge =  ((y+y_ray_vector*ray_length)) ;
		z_buffer[w] = ray_length ;
		ray_length *= fisheye_correction ;
		int wall_height = (int) ((512.0)/ray_length) ; //wall_height >>=1 ;
		int y_wall_start = half_height-(wall_height >>1) ;
		int y_wall_end = half_height+(wall_height>>1) ;

		///////////////////////
		// Jump to next ray  //
		///////////////////////
		angle += angle_step ;
		if (angle > 3600) { angle -= 3600 ; }

		/////////////////////
		// Rendering Part //
		/////////////////////

		///////////////
		// Wall Part //
		///////////////

		if (tile_type > 0) {
			int texture_x ;
			if (x_side) { texture_x = int((x_edge-int(x_edge))*texture_width) ; } else { texture_x = int((y_edge-int(y_edge))*texture_width) ;}
			Source = {texture_x, 0, 1, int(texture_height)} ;
			Dest = {w, y_wall_start, 1, wall_height} ;

			///////////////////////
			// Fog Configuration //
			///////////////////////

			int fog_coeff = (1-(ray_length)/fog)*256;
			SDL_SetTextureColorMod(wall_texture[tile_type-1], fog_coeff, fog_coeff, fog_coeff) ;
			SDL_RenderCopy(renderer, wall_texture[tile_type-1], &Source, &Dest) ;
		}

		////////////////
		// Floor Part //
		////////////////


		static float blink = 0 ;
		blink += 0.05 ;
		if (blink >= 3600) { blink = 0 ;}
		//std::cerr << "BLINK  " << std::setprecision(5) << blink << " " << const_trig::fast_cos(int(blink)) << std::endl;
		for(unsigned int y_floor=y_wall_end;y_floor<height;y_floor++) {
		  float floor_dist = (1.0*height/((y_floor << 1)-height)); // mettre ces données dans une LUT ?
		  float floor_dist_corr = floor_dist/fisheye_correction;
		  float tmp_x = floor_dist_corr*x_ray_vector+x ;
		  float tmp_y = floor_dist_corr*y_ray_vector+y ;
		  int floor_x = (tmp_x-int(tmp_x))*texture_width ;
		  int floor_y = (tmp_y-int(tmp_y))*texture_height ;

		  if  (tmp_x >= 0 && tmp_x < map_width) {
			  if (tmp_y >=0 && tmp_y < map_height) {
				int floor_type = floor_map[int(tmp_y)][int(tmp_x)];
				int indx = (floor_y*texture_width)+floor_x ;
				indx = (indx << 1) + indx ; // indx*=3
				unsigned char r = floor_surface[floor_type][indx] ;
				unsigned char g = floor_surface[floor_type][indx+1] ;
				unsigned char b = floor_surface[floor_type][indx+2] ;

				if (floor_type == 1) {
					float tmp_c = (const_trig::fast_cos(int(blink))+1)/2;
					r *= tmp_c;
					g *= tmp_c;
					b *= tmp_c;
				}

				SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
				SDL_RenderDrawPoint(renderer, w, y_floor) ;
			  }
		  }
		}
	} // end while for ray casting.


	/////////////////
	// Sprite Part //
	/////////////////
	unsigned int nb_of_displayed_sprites = 0 ;
	float HALF_FOV_ANGLE = const_trig::fast_cos(HALF_FOV) ;
	for(unsigned int i=0;i<NB_SPRITE_MAX;i++) {
		float sprite_vector_x = sprite_array[i].x - x ;
		float sprite_vector_y = sprite_array[i].y - y ;
		float plan_vector_x = const_trig::fast_cos(player_angle) ;
		float plan_vector_y = const_trig::fast_sin(player_angle) ;
		float cos_current_angle_with_sprite = (sprite_vector_x*plan_vector_x+sprite_vector_y*plan_vector_y);
		float sin_current_angle_with_sprite = (sprite_vector_y*plan_vector_x-sprite_vector_x*plan_vector_y);
		float tan_current_angle_with_sprite = (sin_current_angle_with_sprite/cos_current_angle_with_sprite+0.0000001);
			if (tan_current_angle_with_sprite > -0.4 && tan_current_angle_with_sprite < 0.6 && cos_current_angle_with_sprite > 0) {
			  // Calcul de la distance pour la gestion du z_buffer --------------------------------------------------------------
				float dist_with_sprite = sqrt(sprite_vector_x*sprite_vector_x+sprite_vector_y*sprite_vector_y)+0.0000001;
				zbuffer_sprite_t tmp_sprite = {sprite_array[i].type, dist_with_sprite, tan_current_angle_with_sprite};
				zbuffer_sprite_array[nb_of_displayed_sprites] = tmp_sprite;
				nb_of_displayed_sprites++;
		} // fin de if.
	} // fin de boucle for.
	sort(zbuffer_sprite_array, nb_of_displayed_sprites);
	/*for(unsigned int i=0;i<nb_of_displayed_sprites;i++) {
		std::cerr << zbuffer_sprite_array[i].dist_with_sprite_sq << std::endl;
	}*/

	for(unsigned int i=0;i<nb_of_displayed_sprites;i++) {
		float tan_current_angle_with_sprite = zbuffer_sprite_array[i].tan_current_angle_with_sprite;
		float square_tan = tan_current_angle_with_sprite*tan_current_angle_with_sprite;
		float cube_tan = square_tan*tan_current_angle_with_sprite ;
		float five_tan = square_tan*square_tan*tan_current_angle_with_sprite ;
		float approx_atan = tan_current_angle_with_sprite - cube_tan/3.0;
		int atan2_tmp = int(approx_atan*572.9); // conversion en degres (360/2*Pi)*10 (le x10 provient du fait que les angles de 0 a 359 sont mappés de 0 à 3600)
		if (atan2_tmp < -1800 ) { atan2_tmp += 3600 ;}
		if (atan2_tmp > 1800 ) { atan2_tmp = 3600-atan2_tmp ;}

		float sprite_size = 480.0/zbuffer_sprite_array[i].dist_with_sprite ;
		int x_sprite = int(atan2_tmp+320)-sprite_size/2 ;
		int proj = 0 ;
		for(int j=0;proj < 480 ;j++, x_sprite++) {
				proj = int((480.0/sprite_size)*j);
				if (x_sprite > 0 && x_sprite < 640) {
					if (zbuffer_sprite_array[i].dist_with_sprite < z_buffer[x_sprite]) {
						Source = {proj, 0, 1, 480} ;
						Dest = {x_sprite, int(240-sprite_size/2), 1, int(sprite_size)} ;
						SDL_RenderCopy(renderer, sprite_texture[0], &Source, &Dest) ;
				} // fin if
			} // fin if
		} // fin for
	} // fin boucle.*/

	SDL_RenderPresent(renderer) ;
}


// Du plus loin au plus proche
void sort(zbuffer_sprite_t* array, unsigned int len_array) {
	unsigned int i, j;
	float current_max_value;
	unsigned int current_max_index;
	for(i=0;i<len_array;i++) {
		current_max_index=i;
		current_max_value = array[i].dist_with_sprite;
		for(j=i+1;j<len_array;j++) {
			if (array[i].dist_with_sprite > current_max_value) {
				current_max_index = j;
				current_max_value = array[i].dist_with_sprite;
			} // fin if,
		} // fin j,
		zbuffer_sprite_t tmp ;
		tmp = array[i];
		array[i] = array[current_max_index];
		array[current_max_index] = tmp;
	} // fin i,
} // fin fonction.
