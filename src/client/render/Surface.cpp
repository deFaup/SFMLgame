#include "define.hpp"
#include "state.h"
#include "Tile.h"
#include "Surface.h"

using namespace render;

void Surface::loadTexture(sf::Image& image){
	texture.loadFromImage(image);
	return;
}

void Surface::setSpriteTexture(){
	sprite.setTexture(texture,false);
	return;
}

void Surface::setSpriteTexture(const Tile& rect){
	sprite.setTexture(texture,true);
	sf::IntRect intrect(rect.x,rect.y,rect.width,rect.height);
	sprite.setTextureRect(intrect);
	return;
}


void Surface::setSpriteLocation(state::Position position){
	sprite.setPosition(sf::Vector2f(position.getPositionX(),position.getPositionY()));
	return;
}

void Surface::draw(sf::RenderWindow& window){
	window.draw(sprite);
	return;
}
