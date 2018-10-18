#include <string>
#include "define.hpp"
#include "Characters.h"

using namespace std;
using namespace state;

Characters::Characters(): name("Unparametered character"), number_of_attacks(0)
{
	Statistics stats; 		// default stats is 10/10/10
	this->stats = stats;
//	Position pos;			// default pos is ?
//	this->position = pos;
}

Characters::Characters(Statistics stats): name("Unnamed character"), number_of_attacks(0), stats(stats)
{
//	Position pos;
//	this->position = pos;
}

Characters::Characters(string name): name(name), number_of_attacks(0)
{
//	Position pos;
//	this->position = pos;
	if(name == "Vegeta"){	// Amené à évoluer
		this->stats.Set_statistiques(101,101,101);
	}
	return;
}

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


