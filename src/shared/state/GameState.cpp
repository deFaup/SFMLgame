#include <iostream>
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

    map.get_screen_dimensions ();
    map.create_mask ();

}

void GameState::new_player(const std::string name){

}


