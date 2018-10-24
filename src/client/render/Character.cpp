#include "define.hpp"
#include "state.h"
#include "Character.h"
#include <SFML/Graphics.hpp>

using namespace render;
using namespace state;

Character::Character(GameState * state){
	etat = state;
}

void Character::draw(){
	for(int i = 0; i < characters.size(); i++){
		sf::Image person;
		person.loadFromFile(tilesets[i]->tileset_file);
		person.createMaskFromColor(person.getPixel(1,1));

		sf::Texture textureperso;
  		textureperso.loadFromImage(person);

		sf::Sprite spriteperso(textureperso,tilesets[i]->CharacterTile[0]);
		spriteperso.setPosition(sf::Vector2f(characters[i]->position.getPositionX(), characters[i]->position.getPositionY()));

		fenetre.draw(spriteperso);
		fenetre.display();
	}
	return;
}
