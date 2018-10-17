
#include "define.hpp"
#include "Player.h"

state::Player::Player(){
	this->name = "Un-named Player";
	return;
}

void state::Player::add_character(Characters new_character){
	if(this->number_of_characters < MAX_NB_CHARACTER){
		std::unique_ptr<Characters> ptr(&new_character);
		this->characters.push_back(std::move(ptr));
		(this->number_of_characters)++;
	}
	return;
}

void state::Player::select_character(Characters character){
	this->current_character = character;	
	return;
}

unsigned int state::Player::get_number_of_characters (){
	return(this->number_of_characters);
}

void state::Player::set_number_of_characters (unsigned int number){
	this->number_of_characters = number;
	return;
}

