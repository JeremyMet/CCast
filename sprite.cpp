#include "sprite.h"

Sprite::Sprite() {
	this->x = 0 ; 
	this->y = 0 ; 
	this->type = 0 ; 
}

Sprite::Sprite(unsigned int x, unsigned int y, unsigned int type) {
	this->x = x ; 
	this->y = y ; 
	this->type = type ; 
}

unsigned int Sprite::get_x() {
	return this->x; 
}

unsigned int Sprite::get_y() {
	return this->y;
}

unsigned int Sprite::get_type() {
	return this->type;
}

void Sprite::set_x(unsigned int x) {
	this->x = x ; 
}

void Sprite::set_y(unsigned int y) {
	this->y = y ; 
}

void Sprite::set_type(unsigned int type) {
	this->type = type ; 
}