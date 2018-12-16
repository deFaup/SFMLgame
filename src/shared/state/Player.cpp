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
	cout << "Player: " << name << " created" << endl;
}

Player::~Player() 
{
	cout << "Player: " << name << " deleted" << endl;
}

/* create and add a character to the player */
shared_ptr<Characters> Player::new_character(const CharactersID id)
{
	shared_ptr<Characters> ptr = 0;
	if (get_number_of_characters() < characters_range)
	{
		ptr = make_shared<Characters>(this, id);
		for (auto obs : this->observers)
		{
			ptr->stats.registerObserver(obs);
			ptr->position.registerObserver(obs);
		}
		characters.push_back(ptr);
		current_character = *(characters.begin());
	}
	return ptr;
}

/* get a reference to your character number i */
std::shared_ptr<Characters>& Player::get_character(unsigned int i)
{
	if (i >= get_number_of_characters() && i!=0 )//only if there are characters and i is too high
		throw std::invalid_argument("Player.cpp in get_character,i bigger than number of characters");

	else if (characters.empty())
		throw std::invalid_argument("Player.cpp in get_character, this player has no characters");

	else
		return characters[i];
}

/* for GameEngine: tells which character you are using */
void Player::select_character(unsigned int character_index)
{
	if (character_index < characters.size()) //you can only select a character among those that you have
		current_character = characters[character_index];
}

unsigned int Player::get_number_of_characters() const{ return(characters.size()); }

void Player::set_characters_range(unsigned int range)
{
	if (range < MAX_NB_CHARACTER)
		characters_range = range;
	else
		characters_range = MAX_NB_CHARACTER;
}

std::shared_ptr<Characters> Player::get_current_character() 
{
	if (current_character)
		return(current_character); 
	else
		throw std::runtime_error("Player.cpp in get_current_character, this player has no character");
}

void Player::delete_character(unsigned int i)
{
	if (characters.size() > 1 && (i < characters.size()) ) //if the player has more than one character
	{
		characters.erase(characters.begin() + i);	
		current_character = characters[0];
	}
	else if (characters.size() == 1) // if he has only one player then the player has lost
	{
		characters.pop_back(); // delete the last character of this player
		current_character = NULL; 
		
		// this player has to be deleted now
		state::EventPlayer event(this, Player_isDead);
		notifyObservers(event);
	}
	else if (characters.empty())
		throw std::runtime_error("Player.cpp in delete_character, deleting a character but player has no character");

	else
		throw std::runtime_error("Player.cpp in delete_character, argument i is bigger than the number of characters");
}


