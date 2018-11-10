#include "define.hpp"
#include "Position.h"

using namespace state;

Position::Position() : x(0), y(0) {}

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

const Position Position::getPosition() const
{
	return *this;
}

void Position::increaseX(int increment) {	this->x += increment; }

void Position::increaseY(int increment) {	this->y += increment; }