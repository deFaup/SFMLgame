/* Includes */
#include <iostream>
#include "define.hpp"
#include "Background.h"

using namespace std;
using namespace render;

Background::Background(const state::GameState& state, const state::Map& map) : state(state), map(map)
{
	surface = make_unique<Surface>();
	
	tileset.push_back(make_shared<TileSet>("res/textureciel.png"));
	tileset.push_back(make_shared<TileSet>("res/textureherbe.png"));
	tileset.push_back(make_shared<TileSet>("res/texturesol.png"));
	
	//sf::Image image;
	//image.create(500, 500, sf::Color::Black);
	
	/* using tileset getImageFile to get it 
	sf::Image ciel, herbe, sol;
	ciel.loadFromFile("res/textureciel.png");
	herbe.loadFromFile("res/textureherbe.png");
	sol.loadFromFile("res/texturesol.png");
	*/
}

