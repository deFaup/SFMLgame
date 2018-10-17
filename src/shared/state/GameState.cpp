#include <iostream>
#include "GameState.h"

GameState::GameState(){

  state = 0;
  new_map(map);
  new_player("player1")
}

void GameState::new_map(){

    map.get_screen_dimensions ();
    map.create_mask (int width, int height);

}

void GameState::new_player(){

}


