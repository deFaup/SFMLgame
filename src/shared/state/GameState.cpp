#include <iostream>
#include <stdexcept>

#include "define.hpp"
#include "GameState.h"

using namespace state;
using namespace std;

GameState::GameState()
{
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
}

/* create a new instance of Player */
void GameState::new_player(const std::string name)
{
	if(get_number_of_player() < MAX_NB_PLAYER)
	{
		shared_ptr<Player> ptr = make_shared<Player>(name);
		players.push_back(ptr);
	}
}

/* return the number of Player's instances */
unsigned int GameState::get_number_of_player() const{
	return(players.size());
}

/* return a reference (not const) to a player */
shared_ptr<Player>& GameState::get_player (unsigned int i)
{
	if (i >= get_number_of_player()) {
		cout << "ID que vous appellez " << i << endl;

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

/* return a const reference of the state's map*/
const Map& GameState::get_map() const { return map; }

/* to limit the possible number of players */
/* the range is also limited by MAX_NB_PLAYER in define.hpp */
void GameState::set_characters_range(unsigned int range)
{
	Player::set_characters_range(range);
}
