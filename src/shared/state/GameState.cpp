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
  //number_of_player = 1;
}

void GameState::new_map(Map& carte){

    map.set_screen_dimensions ();
    int width,height;
 	map.get_screen_dimensions(&width, &height);
    cout << "ici" << endl;
    map.create_mask ();
}

void GameState::new_player(const std::string name){
	if(number_of_player < MAX_NB_PLAYER){
		players.push_back(unique_ptr<Player> (new Player(name)));
	}
}

unsigned int GameState::get_number_of_player(){
	return(this->number_of_player);
}


