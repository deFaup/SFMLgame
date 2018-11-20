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
	
	unsigned int ** matrix;

	matrix = new unsigned int*[3];
	for (int i = 0; i < 3; i++)
		matrix[i] = new unsigned int[3];

	Attacks attack(3,3,1,1,matrix);
	character->addAttack(attack);

	/*if(character->get_attack_cost(attack_number) >= stats.get_attack_point()){
		return(-1);
	}*/

	/*for (int i = 0; i < 3; i++)
		delete[] matrix[i];
	delete[] matrix;*/

	return(0);
}

void Attack::execute(state::GameState& etat){
	cout << "executing attack" << endl;
	return;
}

