#include "define.hpp"
#include "Position.h"

using namespace state;

void Position::setPosition (const unsigned int x, const unsigned int y){
	this->x = x;
	this->y = y;
	return;
}

unsigned int Position::getPositionX(){
	return(x);
}

unsigned int Position::getPositionY(){
	return(y);
}
