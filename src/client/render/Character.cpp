/* Includes */
#include "define.hpp"
#include "state.h"
#include "Character.h"
#include "TileSet.h"
#include <memory>
#include <vector>
#include <iostream>

using namespace render;
using namespace std;

void Character::fill_characters_list(){
	for(unsigned int i = 0; i < state.get_number_of_player(); i++){
		for(unsigned int j = 0; j < state.players[i]->get_number_of_characters(); j++){
			characters.push_back(state.players[i]->get_character(i));
		}
	}

}

Character::Character(const state::GameState& state) : state(state)
{

	surface = std::make_unique<Surface>();

	//const vector<shared_ptr<state::Characters>>& state_characters = state.get_characters();

	fill_characters_list();

	for (int i = 0; i < characters.size(); i++)
	{
		const string name = /*state_*/characters[i]->get_name();
		if (name == "goku")
		{
			tileset.push_back(std::make_shared<TileSet>("res/DBZ_gokusheet2.gif"));
			sf::Image image = tileset[0]->getImageFile(0);
			image.createMaskFromColor(image.getPixel(1, 1));

			/* créer une surface */
			surface->loadTexture(image);	// appel à load texture
			Tile tile(134, 192, 80, 80);
			surface->setSpriteTexture(tile);
		}

	}

}
