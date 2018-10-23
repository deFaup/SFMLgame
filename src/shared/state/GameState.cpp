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

void GameState::new_map(){

    map.get_screen_dimensions ();
    int width(500), height(500);
    map.set_dimensions(width, height);
    map.create_mask ();
}

void GameState::new_player(const std::string name){
	if(get_number_of_player() < MAX_NB_PLAYER)
  {
    shared_ptr<Player> ptr = make_shared<Player>(name);
		players.push_back(ptr);
	}
}

int GameState::get_number_of_player(){
	return(players.size());
}

shared_ptr<Player>& GameState::get_player (int i)
{
	if(i >= players.size() || i < 0)
		throw std::invalid_argument( "received negative value" );
	else 
		return players[i];
}
