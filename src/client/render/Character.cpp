#include "define.hpp"
#include "state.h"
#include "Character.h"
#include "TileSet.h"

using namespace render;

Character::Character(state::GameState& state) : state(state){
	this->state = state;
	surface = std::make_unique<Surface>();
	tileset.push_back(std::make_shared<TileSet>("res/DBZ_gokusheet2.gif"));
	return;
}

void Character::fill_characters_list(){
	for(unsigned int i = 0; i < state.get_number_of_player(); i++){
		for(unsigned int j = 0; j < state.players[i]->get_number_of_characters(); j++){
			characters.push_back(state.players[i]->get_character(i));
		}
	}

}
