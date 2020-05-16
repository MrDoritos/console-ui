#include "box.h"

box::box(int offsetx, int offsety, int sizex, int sizey) {
	this->sizex = sizex;
	this->sizey = sizey;
	this->offsetx = offsetx;
	this->offsety = offsety;
}

box::box() {
	this->sizex = 0;
	this->sizey = 0;
	this->offsetx = 0;
	this->offsety = 0;
}