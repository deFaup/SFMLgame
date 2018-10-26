/* Includes */
#include <iostream>
#include "define.hpp"
#include "Background.h"

using namespace std;
using namespace render;

Background::Background(const state::GameState& state, const state::Map& map) : state(state), map(map)
{
	/* the background can be initialized straight into the constructor */
	
	/* one work surface */
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
	
	/* accès à la map */
	const vector<vector<int>>& mask(map.get_mask());

	/* update the tileset according to the map */
	//x_max is width, y_max is height
	sf::Image& ciel = tileset[1]->getImageFile(0);
	sf::Image& herbe= tileset[2]->getImageFile(0);
	sf::Image& sol =  tileset[3]->getImageFile(0);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
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

	/* créer une surface */
	surface->loadTexture(image);	// appel à load texture
	surface->setSpriteTexture();


}

void Background::update()
{
	//future implementation
	/* accès à la map */
	sf::Image& image = tileset[0]->getImageFile(0);

	const vector<vector<int>>& mask(map.get_mask());

	/* update the tileset according to the map */
	//x_max is width, y_max is height
	sf::Image& ciel = tileset[1]->getImageFile(0);
	sf::Image& herbe = tileset[2]->getImageFile(0);
	sf::Image& sol = tileset[3]->getImageFile(0);

	for (int y = 0; y < 500; y++) {
		for (int x = 0; x < 500; x++) {
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

	/* créer une surface */
	surface->loadTexture(image);	// appel à load texture
	surface->setSpriteTexture();

}