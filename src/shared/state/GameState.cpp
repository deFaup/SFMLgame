#include <iostream>
#include <stdexcept>

#include "define.hpp"
#include "GameState.h"

using namespace state;
using namespace std;

GameState::GameState(){

  state = not_started;
  //new_map();
  //new_player("player1");
  //number_of_player = 1;
}

void GameState::new_map(unsigned const int width, unsigned const int height)
{
	if (width == 0 && height == 0) // default parameters of the function
		map.set_screen_dimensions();
    
	else
		map.set_dimensions(width, height);

    map.create_mask ();
}

void GameState::new_player(const std::string name)
{
	if(get_number_of_player() < MAX_NB_PLAYER)
	{
		shared_ptr<Player> ptr = make_shared<Player>(name);
		players.push_back(ptr);
	}
}

unsigned int GameState::get_number_of_player() const{
	return(players.size());
}

shared_ptr<Player>& GameState::get_player (unsigned int i)
{
	if( i >= get_number_of_player() )
		throw std::invalid_argument( "you can't access to a non existent player" );
	else 
		return players[i];
}

const Map& GameState::get_map() const { return map; }


void GameState::set_characters_range(unsigned int range)
{
	Player::set_characters_range(range);
}

const vector<shared_ptr<Characters>>& GameState::get_characters() const { return characters; }


