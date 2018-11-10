#include <iostream>
#include <stdexcept>

#include "define.hpp"
#include "GameState.h"

using namespace state;
using namespace std;

GameState::GameState()
{
//	cout << "GameState created" << endl;
	state = not_started;
}

/* Create a mask in the map object */
void GameState::new_map(unsigned const int width, unsigned const int height)
{
	if (width == 0 && height == 0) // default parameters of the function
		map.set_screen_dimensions();
    
	else
		map.set_dimensions(width, height);

    map.create_mask ();
	// notify map's observer EventMap that the mask has changed
	//notifyObservers(EventMap(Map_maskChanged));
}

/* create a new instance of Player */
void GameState::new_player(const std::string name)
{
	if(get_number_of_player() < MAX_NB_PLAYER)
	{
		shared_ptr<Player> ptr = make_shared<Player>(name);
		players.push_back(ptr);
		current_player = ptr;
	}
}

/* create a new character for player i */
void GameState::new_character(const unsigned int player_id, const CharactersID character_id)
{
	shared_ptr<Characters> ptr(0);
	ptr = get_player(player_id)->new_character(character_id);
	characters.push_back(ptr);
}

/* return the number of Player's instances */
unsigned int GameState::get_number_of_player() const{ return(players.size()); }

/* return a reference (not const) to a player */
shared_ptr<Player>& GameState::get_player (unsigned int i)
{
	if (i >= get_number_of_player()) {
		throw std::invalid_argument("you can't access to a non existent player");
	}
	else 
		return players[i];
}

/* return a const reference to a player */
const shared_ptr<Player>& GameState::get_player(unsigned int i) const
{
	if (i >= get_number_of_player())
		throw std::invalid_argument("you can't access to a non existent player");
	else
		return players[i];
}

/* return a reference to the characters list */
const vector<shared_ptr<Characters>>& GameState::get_characters() const {	return characters;	}

/* return a const reference of the state's map*/
const Map& GameState::get_map() const { return map; }

/* to limit the possible number of players */
/* the range is also limited by MAX_NB_PLAYER in define.hpp */
void GameState::set_characters_range(unsigned int range)
{
	Player::set_characters_range(range);
}
