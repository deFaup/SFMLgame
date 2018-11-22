#include "define.hpp"
#include "Position.h"
#include <iostream>

using namespace state;

Position::Position() : x(0), y(0) {}

void Position::setPosition (const unsigned int x, const unsigned int y){
	this->x = x;
	this->y = y;
	state::EventCharacters event(Character_positionChanged);
	notifyObservers(event);
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

void Position::increaseX(int increment) 
{
	this->x += increment; 
	state::EventCharacters event(Character_positionChanged);
	notifyObservers(event);
	std::cout << "x_pos= " << x << "\t y_pos= " << y << std::endl;
}

void Position::increaseY(int increment) 
{	
	this->y += increment; 
	state::EventCharacters event(Character_positionChanged);
	notifyObservers(event);
	std::cout << "x_pos= " << x << "\t y_pos= " << y << std::endl;
}