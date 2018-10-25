#include <string>
#include <iostream>

#include "define.hpp"
#include "Characters.h"

using namespace std;
using namespace state;

Characters::~Characters() {	/* cout << "Characters: "<< name << " deleted"<< endl; */ }

Characters::Characters(): name("Unparametered character"), number_of_attacks(0)
{
	// call Statistics constructor and set stats to 100/10/10
	// call Position constructor and do nothing

	//	cout << "Characters: " << name << " created" << endl;
}

Characters::Characters(Statistics stats): name("Unnamed character"), stats(stats), number_of_attacks(0)
{
	// call Position constructor and do nothing
}

Characters::Characters(string name): name(name), number_of_attacks(0)
{
	// call Statistics constructor and set stats to 100/10/10
	// call Position constructor and do nothing

	cout << "Characters: " << name << " created" << endl;

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


