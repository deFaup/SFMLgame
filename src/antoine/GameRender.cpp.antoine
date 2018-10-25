#include <SFML/Graphics.hpp>
#include "define.hpp"
#include "state.h"
#include "GameRender.h"

using namespace render;

/*sf::RenderWindow fenetre(sf::VideoMode(500, 500), "test affichage");
sf::RenderWindow GameRender::fenetre = fenetre;*/

GameRender::GameRender (){

}

void GameRender::rend (){

	sf::RenderWindow renderWindow(sf::VideoMode(500, 500), "test affichage");

	sf::Event event;

	sf::Image image;
  	image.create(500, 500, sf::Color::Black);

	sf::Image ciel, herbe, sol;
	ciel.loadFromFile(grid.tileset.sky_file);
	herbe.loadFromFile(grid.tileset.grass_file);
	sol.loadFromFile(grid.tileset.ground_file);

	const std::vector<std::vector<int>>& mask = grid.etat->map.get_mask();

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

	//std::vector<sf::Sprite> sprites = character.draw();

	std::vector<sf::Sprite> sprites;
	for(int i = 0; i < character.characters.size(); i++){
		sf::Image person;
		person.loadFromFile(character.tilesets[i]->tileset_file);
		person.createMaskFromColor(person.getPixel(1,1));

		sf::Texture textureperso;
  		textureperso.loadFromImage(person);

		sf::Sprite spriteperso(textureperso,character.tilesets[i]->CharacterTile[0]);
		spriteperso.setPosition(sf::Vector2f(character.characters[i]->position.getPositionX(), character.characters[i]->position.getPositionY()));

		sprites.push_back(spriteperso);

	}

	while (renderWindow.isOpen()){
		while (renderWindow.pollEvent(event)){
			if (event.type == sf::Event::EventType::Closed)
			renderWindow.close();
		}

		renderWindow.clear();
		renderWindow.draw(sprite);
		for(int i=0; i<sprites.size(); i++){
			renderWindow.draw(sprites[i]);
		}
		renderWindow.display();
	}
}

