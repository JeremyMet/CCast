#include "sprite.h"

Sprite::Sprite() {
	this->x = 0 ;
	this->y = 0 ;
	this->type = 0 ;
	this->active = false;
}

Sprite::Sprite(float x, float y, unsigned int type) {
	this->x = x ;
	this->y = y ;
	this->type = type ;
}

float Sprite::get_x() {
	return this->x;
}

float Sprite::get_y() {
	return this->y;
}

unsigned int Sprite::get_type() {
	return this->type;
}

void Sprite::set_x(float x) {
	this->x = x ;
}

void Sprite::set_y(float y) {
	this->y = y ;
}

void Sprite::set_type(unsigned int type) {
	this->type = type ;
}

void Sprite::switch_on() {
	this->active = true;
}

void Sprite::switch_off() {
	this->active = false;
}
