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

Player::Player (string name) : name(name) 
{
	/*cout << "Player: " << name << " created" << endl;*/ 
}

Player::~Player() 
{
	/* cout << "Player: " << name << " deleted" << endl; */
}

void Player::new_character(const std::string name)
{
	if (get_number_of_characters() < characters_range)
	{
		shared_ptr<Characters> ptr = make_shared<Characters>(name);
		characters.push_back(ptr);
	}
}


void Player::add_character(shared_ptr<Characters>& character)
{
	if (characters.size() < characters_range) // You can't add more characters than characters_range
		characters.push_back(character);
}


std::shared_ptr<Characters>& Player::get_character(unsigned int i)
{
	if (i >= get_number_of_characters())
		throw std::invalid_argument( "you can't access to a non existent character" );
	else
		return characters[i];
}

void Player::select_character(unsigned int character_index)
{
	if (character_index < characters.size()) //you can only select a character among those that you have
		current_character = characters[character_index];
}

unsigned int Player::get_number_of_characters() const{
	return(characters.size());
}

void Player::set_characters_range(unsigned int range)
{
	if (range < MAX_NB_CHARACTER)
		characters_range = range;
	else
		characters_range = MAX_NB_CHARACTER;
}

ostream& operator<<(ostream& flux, Player const& player)
{
	flux << "player name: " << player.name << endl;
	flux << "player characters: " << player.get_number_of_characters() << endl;
	flux << "player's characters: " << endl;
	/*
	for (auto character : player.characters)
	{
		flux << "\t" << ;
	}
	*/
	return flux;
}