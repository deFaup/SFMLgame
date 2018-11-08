#include <string>
#include <iostream>

#include "define.hpp"
#include "Characters.h"

using namespace std;
using namespace state;

Characters::~Characters() {	/* cout << "Characters: "<< name << " deleted"<< endl; */ }

Characters::Characters(Player* player): owner(player), id(default_value), number_of_attacks(0)
{
	// call Statistics constructor and set stats to 100/10/10
	// call Position constructor and set position to (0,0)
	//	cout << "Characters: " << name << " created" << endl;
}

/* Create a character of id=id with a defined owner, stats depending on the ID and position to (0,0) */
Characters::Characters(Player* player, CharactersID id): owner(player), id(id), stats(id), number_of_attacks(0)
{
	cout << "Characters: " << id << " created" << endl;
	//Statistics my_stat(20, 21, 22);
	//get_statistics().set_statistics(my_stat);
	cout << "\tstats: life_point = " << get_statistics().get_attack_point() << endl;
	cout << "\tstats: attack_point = " << get_statistics().get_attack_point() << endl;
	cout << "\tstats: move_point = " << get_statistics().get_move_point() << endl;
}

CharactersID Characters::get_id() const{ return id; }

Position& Characters::get_position() { return position; }

Statistics& Characters::get_statistics() { return stats; }

unsigned int Characters::get_number_of_attacks(){
	return(this->number_of_attacks);
}

void Characters::addAttack (unsigned int attack_cost, unsigned int attack_damage, void* attack_field_of_action){
	if(this->number_of_attacks < MAX_NB_ATTACK)
	{
		if(attack_cost <= MAX_ATTACK_COST){	
			this->attack_cost[number_of_attacks] = attack_cost;
		}
		else{
			this->attack_cost[number_of_attacks] = MAX_ATTACK_COST;
		}
		if(attack_damage <= MAX_ATTACK_DAMAGE){	
			this->attack_damage[number_of_attacks] = attack_damage;
		}
		else{
			this->attack_damage[number_of_attacks] = MAX_ATTACK_DAMAGE;
		}
		//this->attack_field_of_action[number_of_attacks] = attack_field_of_action;
		(this->number_of_attacks)++;
	}
	return;
}

unsigned int Characters::get_attack_cost (unsigned int numero_attack){
	return(this->attack_cost[numero_attack-1]);
	}

unsigned int Characters::get_attack_damage (unsigned int numero_attack){
	return(this->attack_damage[numero_attack-1]);
	}
void* Characters::get_attack_field_of_action (unsigned int numero_attack){
	return(this->attack_field_of_action[numero_attack-1]);
	}


