#include "define.hpp"
#include <SFML/Graphics.hpp>
#include "state.h"
#include "Grid.h"

using namespace render;
using namespace state;

Grid::Grid(){}

Grid::Grid (GameState *state){
	etat = state;
}

sf::Sprite Grid::draw (){
	sf::Image image;
  	image.create(500, 500, sf::Color::Black);

	sf::Image ciel, herbe, sol;
	ciel.loadFromFile(tileset.sky_file);
	herbe.loadFromFile(tileset.grass_file);
	sol.loadFromFile(tileset.ground_file);

	const std::vector<std::vector<int>>& mask = etat->map.get_mask();

	//image.setPixel(1, 1, sol.getPixel(1,1));

	for (int y = 0; y < 500; y++){
		for (int x = 0; x < 500; x++){
			if (mask[y][x] == 1){
				//image.setPixel(x, y, sol.getPixel(x,y));
			}
			else if(mask[y][x] == 2){
				//image.setPixel(x, y, herbe.getPixel(x,y));
			}
			else {
				//image.setPixel(x, y, ciel.getPixel(x,y));
			}
		}
	}

	sf::Texture texture;
  	texture.loadFromImage(image);
  	sf::Sprite sprite(texture);

	return(sprite);

}
