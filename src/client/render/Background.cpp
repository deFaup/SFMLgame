/* Includes */
#include <iostream>
#include "define.hpp"
#include "Background.h"

using namespace std;
using namespace render;

Background::Background(const state::GameState& state, const state::Map& map) : state(state), map(map)
{
	/* the background can be initialized straight into the constructor */
	
	/* instantiation of a unique work surface */
	surface = make_unique<Surface>();
	
	/* get map dimensions */
	int width(0), height(0);
	map.get_dimensions(width, height);

	/* creating an empty background */
	tileset.push_back(make_shared<TileSet>());
	sf::Image image = tileset[0]->getImageFile(0);
	image.create(width, height, sf::Color::Black);

	/* loading the images to make the background */
	tileset.push_back(make_shared<TileSet>("res/textureciel.png"));
	tileset.push_back(make_shared<TileSet>("res/textureherbe.png"));
	tileset.push_back(make_shared<TileSet>("res/texturesol.png"));

	/* update the background to represent the state's map */
//	update();

	/* get the mask of state's map */
	const vector<vector<int>>& mask(map.get_mask());

	/* update the background to represent the state's map */
	sf::Image& ciel = tileset[1]->getImageFile(0);
	sf::Image& herbe = tileset[2]->getImageFile(0);
	sf::Image& sol = tileset[3]->getImageFile(0);

	for (int y = 0; y < height; y++) { //y == height
		for (int x = 0; x < width; x++) { // x== width
			if (mask[y][x] == 1) {
				image.setPixel(x, y, sol.getPixel(x, y));
			}
			else if (mask[y][x] == 2) {
				image.setPixel(x, y, herbe.getPixel(x, y));
			}
			else {
				image.setPixel(x, y, ciel.getPixel(x, y));
			}
		}
	}

	/* create a texture from our backgroung image */
	surface->loadTexture(image);

	/* convert the texture into a sprite */
	surface->setSpriteTexture();
}

void Background::update()
{
	/* get background */
	//sf::Image& image = tileset[0]->getImageFile(0);

	/* get the mask of state's map */
	//const vector<vector<int>>& mask(map.get_mask()); //not allowed

	/* get map dimensions */
	//int width(0), height(0);
	//map.get_dimensions(width, height);	//not allowed

	/* update the background to represent the state's map */
/*	sf::Image& ciel = tileset[1]->getImageFile(0);
	sf::Image& herbe = tileset[2]->getImageFile(0);
	sf::Image& sol = tileset[3]->getImageFile(0);

	for (int y = 0; y < height; y++) { //y == height
		for (int x = 0; x < width; x++) { // x== width
			if (mask[y][x] == 1) {
				image.setPixel(x, y, sol.getPixel(x, y));
			}
			else if (mask[y][x] == 2) {
				image.setPixel(x, y, herbe.getPixel(x, y));
			}
			else {
				image.setPixel(x, y, ciel.getPixel(x, y));
			}
		}
	}
*/
	/* create a texture from our backgroung image */
	//surface->loadTexture(image);

	/* convert the texture into a sprite */
	//surface->setSpriteTexture();
}