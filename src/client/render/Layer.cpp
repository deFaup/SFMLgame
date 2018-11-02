/* Includes */
//#include <iostream>
//#include "define.hpp"
#include "define.hpp"
#include "Layer.h"

using namespace std;
using namespace render;

Layer::Layer() {}

void Layer::setSurface(sf::RenderWindow& window)
{
	surface->draw(window);
}

void Layer::update()
{
	//future implementation: displays the state changes
}