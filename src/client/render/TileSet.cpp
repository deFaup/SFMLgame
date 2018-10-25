#include "define.hpp"
#include "state.h"
#include "TileSet.h"

using namespace render;

void TileSet::setImageFile(std::string name){
	sf::Image image;
	image.loadFromFile(name);
	images.push_back(image);
	return;
}

sf::Image& TileSet::getImageFile(unsigned int i){
	return(images[i]);
}
