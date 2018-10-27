/* Includes */
#include <string>
#include "define.hpp"
#include "state.h"
#include "TileSet.h"

using namespace std;
using namespace render;

TileSet::TileSet() {}

TileSet::TileSet(unsigned int width, unsigned int height, sf::Color color)
{
	sf::Image pic;
	pic.create(width, height, color);
	image = pic;
}

TileSet::TileSet(string image)
{
	setImageFile(image);
}

void TileSet::setImageFile(std::string name){
	sf::Image pic;
	pic.loadFromFile(name);
	image = pic;
	return;
}

sf::Image& TileSet::getImage(){
	return image;
}
