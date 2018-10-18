#include <string>

#include "define.hpp"
#include "Player.h"

using namespace state;
using namespace std;

Player::Player(): name("Un-named Player"), number_of_characters(0) {
	//this->number_of_characters = 0;
}

Player::Player(string name): name(name), number_of_characters(0){
	//this->number_of_characters = 0;
}

void Player::add_character(Characters new_character)
{
	if(this->number_of_characters < MAX_NB_CHARACTER){
		unique_ptr<Characters> ptr(&new_character);
		this->characters.push_back(move(ptr));
		(this->number_of_characters)++;
	}
	return;
}

void Player::select_character(Characters character){
	this->current_character = character;	
	return;
}

unsigned int Player::get_number_of_characters (){
	return(this->number_of_characters);
}

void Player::set_number_of_characters (unsigned int number){
	this->number_of_characters = number;
	return;
}

