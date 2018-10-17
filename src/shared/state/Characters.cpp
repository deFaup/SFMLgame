
#include "define"
#include "Characters.h"

using namespace state;

state::Characters::Characters(){
	this->name = "Unparametred character";
	this->number_of_attacks = 0;
	return;
}

state::Characters::Characters(Statistics stats){
	this->name = "Unnamed character";
	this->number_of_attacks = 0;
	this->stats = stats;
	return;
}

state::Characters::Characters(std::string name){
	this->name = name;
	this->number_of_attacks = 0;
	if(name == "Vegeta"){				// Amené à évoluer
		this->stats.Set_statistiques(101,101,101);
	}
	return;
}

void state::Characters::addAttack (unsigned int attack_cost, unsigned int attack_damage, void * attack_field_of_action){
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
	this->attack_field_of_action[number_of_attacks] = attack_field_of_action;
}

unsigned int state::Characters::get_attack_cost (unsigned int numero_attack){
	return(this->attack_cost[numero_attack-1]);
	}

unsigned int state::Characters::get_attack_damage (unsigned int numero_attack){
	return(this->attack_damage[numero_attack-1]);
	}
void * state::Characters::get_attack_field_of_action (unsigned int numero_attack){
	return(this->attack_field_of_action[numero_attack-1]);
	}


