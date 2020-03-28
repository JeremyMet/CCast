#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <string>


class Level {

  private:
    unsigned int height ;
    unsigned int width ;
    unsigned int** wall_array ;
	unsigned int** floor_array ;

  public:

    // constructors 

    Level() ;
	Level(std::string path);
    ~Level();

    
    // Accessors
    unsigned int get_height();
    unsigned int get_width();
    unsigned int** get_wall_array();
	unsigned int** get_floor_array();



    // Methods
    bool load(std::string path);

};

#endif