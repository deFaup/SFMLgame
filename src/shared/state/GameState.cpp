#include <iostream>
#include <vector>

#include "define.hpp"
#include "GameState.h"

using namespace state;
using namespace std;

GameState::GameState(){

  state = not_started;
  new_map(map);
  new_player("player1");
}

void GameState::new_map(Map& carte){

    map.set_screen_dimensions ();
    int width,height;
 	map.get_screen_dimensions(&width, &height);
    map.create_mask ();
}

void GameState::new_player(const std::string name){

	players.push_back(unique_ptr<Player> (new Player(name)));

}


