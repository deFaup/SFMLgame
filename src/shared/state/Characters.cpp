#include <string>
#include <iostream>

#include "define.hpp"
#include "Characters.h"

using namespace std;
using namespace state;

Characters::~Characters()
{
	cout << "Characters: " << id << " deleted" << endl;

	/*for (unsigned int i = 0; i < number_of_attacks; i++){
		delete &(attack[i]);
	}*/
}

/* Create a character of id=id with a defined owner, stats depending on the ID and position to (0,0) */
Characters::Characters(Player* player, CharactersID id): owner(player), id(id), stats(id)
{
	cout << "Characters: " << id << " created" << endl;
	//Statistics my_stat(20, 21, 22);
	//get_statistics().set_statistics(my_stat);
	//cout << "\tstats: life_point = " << get_statistics().get_attack_point() << endl;
	//cout << "\tstats: attack_point = " << get_statistics().get_attack_point() << endl;
	//cout << "\tstats: move_point = " << get_statistics().get_move_point() << endl;
}

CharactersID Characters::get_id() const{ return id; }

Position& Characters::get_position() { return position; }

Statistics& Characters::get_statistics() { return stats; }

Attacks& Characters::get_attack(unsigned int i) { return attacks[i]; }

unsigned int Characters::get_number_of_attacks(){
	return(attacks.size());
}

void Characters::addAttack (Attacks& attaque){
	if(attacks.size() < MAX_NB_ATTACK)
	{
		attacks.push_back(attaque);
	}
	return;
}

void Characters::reset_stats()
{
	stats.reset_all_but_life(id);
}


