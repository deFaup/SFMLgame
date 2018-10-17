#include "Player.h"

namespace state {

state::Player::Player(){
	return;
}

void state::Player::add_character(Characters new_character){
	std::unique_ptr<Characters> ptr(&new_character);
	characters.push_back(std::move(ptr));
	return;
}

void state::Player::select_character(){
	this->current_character;	
	return;
}

};
