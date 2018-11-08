#include "define.hpp"
#include "Attack.h"
#include <iostream>
#include <memory>

using namespace engine;
using namespace std;
using namespace state;

int Attack::isLegit(state::GameState etat){
	shared_ptr<Characters> character = etat.current_player->get_current_character();
	Statistics& stats = character->get_statistics();
	
	if(character->get_attack_cost(attack_number) >= stats.get_attack_point()){
		return(-1);
	}
	return(0);
}

void Attack::execute(state::GameState& etat){
	
	cout << "executing attack" << endl;
	return;
}

