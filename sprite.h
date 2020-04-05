// Sprite.h

// Sprite Class

class Sprite {
	
	// Attributes
	private:
		unsigned int x ; 
		unsigned int y ; 
		unsigned int type ; 
		
	public:
		Sprite() ; 		
		Sprite(unsigned int x, unsigned int y, unsigned int type);
		unsigned int get_x() ; 
		unsigned int get_y() ; 
		unsigned int get_type() ; 
		void set_x(unsigned int x) ; 
		void set_y(unsigned int y) ; 
		void set_type(unsigned int type) ; 
		
	
} ; 