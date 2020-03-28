
#include <fstream>
#include <string>

#include "misc.h"
#include "level.h"



Level::Level() {
  this->height = 0 ;
  this->width = 0 ;
  this->wall_array = NULL ;
}

// destructor
Level::~Level()
{
  for(int i=0;i<height;i++)
  {
	delete[] this->wall_array[i] ;
  }
  delete[] this->wall_array ;
}

Level::Level(std::string path)
{
  load(path) ;
}


// Accessors

unsigned int Level::get_height() {
  return this->height ;
}

unsigned int Level::get_width() {
  return this->width ;
}

unsigned int** Level::get_wall_array() {
  return this->wall_array ;
}

unsigned int** Level::get_floor_array() {
  return this->floor_array ;
}

// Methods
bool Level::load(std::string path) {
  std::ifstream file(path) ;
  std::string local ;
  bool ret = false ;
  if (file.is_open())
  {
	// Read Height/Width
	std::getline(file, local) ;
	this->height = std::stoi(misc::split(local,",", __MISC_LEFT__)) ;
	this->width = std::stoi(misc::split(local,",", __MISC_RIGHT__)) ;
	// wall_array Generation
	this->wall_array = new unsigned int*[height];
	this->floor_array = new unsigned int*[height];
	for(int i=0 ; i < this->height ; i++) { wall_array[i] = new unsigned int[width] ; floor_array[i] = new unsigned int[width] ; }
	
	int line = 0 ;
	while(std::getline(file, local) && line < this->height)
	{
	   for(int i=0;i<this->width;i++) {this->wall_array[line][i] = int(local.at(i << 1)-'0') ; }
	   line++ ;
	}
	line = 0 ; 
	std::getline(file, local); // separator between walls and floor
	while(std::getline(file, local) && line < this->height)
	{
	   for(int i=0;i<this->width;i++) {this->floor_array[line][i] = int(local.at(i << 1)-'0') ; }
	   line++ ;
	}
	file.close() ;
  }
  return ret ;
}
