#include "define.hpp"
#include <SFML/Graphics.hpp>
#include "state.h"
#include "Character.h"

using namespace render;
using namespace state;

Character::Character(){}

Character::Character(GameState * state){
	etat = state;
}

std::vector<sf::Sprite> Character::draw(){
	std::vector<sf::Sprite> sprites;
	for(int i = 0; i < characters.size(); i++){
		sf::Image person;
		person.loadFromFile(tilesets[i]->tileset_file);
		person.createMaskFromColor(person.getPixel(1,1));

		sf::Texture *textureperso;
  		(*textureperso).loadFromImage(person);

		sf::Sprite spriteperso(*textureperso,tilesets[i]->CharacterTile[0]);
		spriteperso.setPosition(sf::Vector2f(characters[i]->position.getPositionX(), characters[i]->position.getPositionY()));

		sprites.push_back(spriteperso);

	}
	return(sprites);
}
