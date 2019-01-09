/* Includes */
#include <iostream>
#include "define.hpp"
#include "Background.h"

using namespace std;
using namespace render;

Background::~Background() { cout << "background deleted" << endl; }

Background::Background(const state::GameState& state) : state(state), map(state.map) {}

void Background::new_background_layer()
{
	/* the background is initialized straight into the constructor */
	// peut être mettre une boucle while !surface.empty() surface.pop_back()
	// cad qu'il faut vider les vecteurs et tout supprimé si on fait une nouvelle partie sans relancer le jeu.
	// même chose pour character layer
	/* instantiation of a unique work surface */
	surface.push_back( make_unique<Surface>() );
	
	/* get map dimensions */
	int width(0), height(0);
	map.get_dimensions(width, height);

	/* creating an empty background whose size == map's size*/
	tileset.push_back( make_shared<TileSet>(width, height, sf::Color::Black) );

	/* Linking our Surface texure to the background */
	surface[0]->loadTexture(tileset[0]->getImage());

	/* Linking our Surface sprite to the Surface texture */
	surface[0]->setSpriteTexture();

	/* loading the images to make the background */
	tileset.push_back(make_shared<TileSet>("res/BlueSkyWithClouds.png"));
	tileset.push_back(make_shared<TileSet>("res/GreenGrass.png"));
	tileset.push_back(make_shared<TileSet>("res/texturesol.png"));

	/* update the background to represent the state's map */
	update();
	cout << "New background created\n" << endl;
}

/* Update the background if the mask has changed */
void Background::update()
{
	/* get background */
	sf::Image& image = tileset[0]->getImage();

	/* get the mask of state's map */
	const vector<vector<int>> mask (map.get_mask()); 
	
	/* get map dimensions */
	int width(0), height(0);
	map.get_dimensions(width, height);

	/* update the background to represent the state's map */
	sf::Image& ciel = tileset[1]->getImage();
	sf::Image& herbe = tileset[2]->getImage();
	sf::Image& sol = tileset[3]->getImage();

	sf::Vector2u size_ciel(ciel.getSize()), size_herbe(herbe.getSize()), size_sol(sol.getSize());
	for (int y = 0; y < height; y++) //y == height
	{
		for (int x = 0; x < width; x++) // x== width
		{ 
			if (mask[y][x] == 1) 
			{
				image.setPixel(x, y, sol.getPixel(x % (size_sol.x), y % (size_sol.y)));
				//image.setPixel(x, y, sol.getPixel(x, y));
			}
			else if (mask[y][x] == 2) {
				image.setPixel(x, y, herbe.getPixel(x % (size_herbe.x), y % (size_herbe.y)));
			}
			else {
				image.setPixel(x, y, ciel.getPixel(x % (size_ciel.x), y % (size_ciel.y)));
			}
		}
	}

	/* update the texture and thus the sprite as they are linked */
	surface[0]->update(image);

}

void Background::setSurface(sf::RenderWindow& window)
{
	surface[0]->draw(window);
}
