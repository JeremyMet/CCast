// Sprite.h

// Sprite Class

#ifndef __SPRITE__
#define __SPRITE__
/*
class Sprite {

	// Attributes
	private:
		float x ;
		float y ;
		unsigned int type ;
		bool active;

	public:
		Sprite() ;
		Sprite(float x, float y, unsigned int type);
		float get_x() ;
		float get_y() ;
		unsigned int get_type() ;
		void set_x(float x) ;
		void set_y(float y) ;
		void set_type(unsigned int type) ;
		void switch_on();
		void switch_off();

} ;
*/

typedef struct {
	float x ;
	float y ;
	float angle;
	float delta_x;
	float delta_y;
	unsigned int type ;
	bool active;
} Sprite ;
#endif
