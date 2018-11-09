#include "define.hpp"
#include "Move.h"
#include <iostream>
#include <vector>

using namespace engine;
using namespace std;
using namespace state;

int Move::isLegit(state::GameState etat){
	//if(etat.current_player->current_character->get_)
	return(0);
}

void Move::execute(state::GameState& etat){

	/* get current_player and current_character */
//	etat.current_player->get_current_character();
	cout << "executing movement" << endl;
	/*vector<std::vector<int>> mask = etat.map.get_mask();
	shared_ptr<Characters> character = etat.current_player->get_character(1);
	Position position = character->get_position();
	position.setPosition(position.getPositionX()+1,position.getPositionY());*/
	return;
}

