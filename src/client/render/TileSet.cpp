/* Includes */
#include <string>
#include <iostream>
#include "define.hpp"
#include "state.h"
#include "TileSet.h"

using namespace std;
using namespace render;

TileSet::TileSet() { std::cout << "TileSet created" << std::endl; }
TileSet::~TileSet() { std::cout << "TileSet deleted" << std::endl; }

/* Create a sf::Image of width x height dimensions and filled with sf::Color pixels*/
TileSet::TileSet(unsigned int width, unsigned int height, sf::Color color)
{
	std::cout << "TileSet created" << std::endl;

	sf::Image pic;
	pic.create(width, height, color);
	image = pic;
}

/* Load an image in the sf::Image attribute */
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

/* Return a reference to the sf::Image attribute (not a const method) */
sf::Image& TileSet::getImage(){
	return image;
}
