#include <string>
#include <iostream>

#include "define.hpp"
#include "Player.h"

using namespace state;
using namespace std;

unsigned int Player::characters_range = MAX_NB_CHARACTER;

Player::Player() : name("Un-named Player") 
{
	//cout << "Player: " << name << " created" << " char max= " << characters_range << endl;
}

Player::Player (string name) : name(name) {	/*cout << "Player: " << name << " created" << endl;*/ }
Player::~Player() {	/* cout << "Player: " << name << " deleted" << endl; */}

void Player::add_character(shared_ptr<Characters>& character)
{
	if (characters.size() < characters_range) // You can't add more characters than characters_range
		characters.push_back(character);
}

void Player::select_character(unsigned int character_index)
{
	if (character_index < characters.size()) //you can only select a character among those that you have
		current_character = characters[character_index];
}

unsigned int Player::get_number_of_characters() {
	return(characters.size());
}

void Player::set_characters_range(unsigned int range)
{
	if (range < MAX_NB_CHARACTER)
		characters_range = range;
	else
		characters_range = MAX_NB_CHARACTER;

	// maybe we should remove MAX_NB_CHARACTER and put a static var MAX_NB_CHARACTER instead
	// this way we give the poissibility to the players to choose the max number of players
}

