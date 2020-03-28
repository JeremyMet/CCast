#include "ray_engine.h"

#include<iostream>




unsigned int Ray_engine::height = 480 ;
unsigned int Ray_engine::half_height = 120 ;
unsigned int Ray_engine::width = 640 ;
unsigned int Ray_engine::FOV = 640 ;
unsigned int Ray_engine::HALF_FOV = 320 ;
unsigned int Ray_engine::cam_distance = 240;
unsigned int Ray_engine::angle_step = Ray_engine::FOV/Ray_engine::width ;
SDL_Texture* Ray_engine::wall_texture[NB_WALL_TEXTURE];
unsigned char* Ray_engine::floor_surface[NB_FLOOR_TEXTURE];
SDL_Texture* Ray_engine::background_texture = NULL ;

unsigned int** Ray_engine::wall_map = NULL ;
unsigned int** Ray_engine::floor_map = NULL ;
int Ray_engine::map_width = 0 ;
int Ray_engine::map_height = 0 ;
unsigned int Ray_engine::texture_height = 128;
unsigned int Ray_engine::texture_width = 128;
unsigned int Ray_engine::fog = 10;




void Ray_engine::init(SDL_Renderer* renderer, Level* level_arg)
{
	half_height = height >> 1 ;
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
}

void Ray_engine::render(SDL_Renderer* renderer, Player* my_player){
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
	float player_angle = my_player->get_angle() ;	
	float x = my_player->get_x() ;
	float y = my_player->get_y() ;
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
	float length_x_ray_vector = abs(1/x_ray_vector) ;
	float length_y_ray_vector = abs(1/y_ray_vector) ;
	
		
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
	ray_length *= fisheye_correction ;
		  

	int wall_height = (int) ((1.0*cam_distance*__RAY_ENGINE_WALL_HEIGHT__)/ray_length) ; //wall_height >>=1 ;
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

	for(unsigned int y_floor=y_wall_end;y_floor<height;y_floor++) {
	  float floor_dist = (1.0*height/((y_floor << 1)-height)); // mettre ces données dans une LUT ?
	  float floor_dist_corr = floor_dist/(fisheye_correction);
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
			SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
			SDL_RenderDrawPoint(renderer, w, y_floor) ;
		  }
	  }	  
	  
	  
	}


} // end while for ray casting.

SDL_RenderPresent(renderer) ;
}



