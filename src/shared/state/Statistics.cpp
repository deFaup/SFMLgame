/**
 * @file : Statistics.cpp
 * @brief : fichier contenant la definition des methodes de la classe Statistics
 */

#include "define.hpp"
#include "Statistics.h"
using namespace state;

state::Statistics::Statistics (int vie, int attaque, int deplacement){
	if(0 <= vie && vie <= MAX_LIFE_POINT){
		this->life_point = vie;
	}
	else if(vie > MAX_LIFE_POINT){
		this->life_point = MAX_LIFE_POINT;
	}
	else{
		this->life_point = 0;
	}
	if(0 <= attaque && attaque <= MAX_ATTACK_POINT){
		this->attack_point = attaque;
	}
	else if(attaque > MAX_ATTACK_POINT){
		this->attack_point = MAX_ATTACK_POINT;
	}
	else{
		this->attack_point = 0;
	}
	if(0 <= deplacement && deplacement <= MAX_MOVE_POINT){
		this->move_point = deplacement;
	}
	else if(deplacement > MAX_MOVE_POINT){
		this->move_point = MAX_MOVE_POINT;
	}
	else{
		this->move_point = 0;
	}
	return;
}

Statistics::Statistics(CharactersID id)
{
	switch (id)
	{
	case goku:
		life_point = 100;
		attack_point = 10;
		move_point = 10;
		break;

	case vegeta:
		life_point = 100;
		attack_point = 10;
		move_point = 10;
		break;

	case miyo:
		life_point = 100;
		attack_point = 10;
		move_point = 10;
		break;
	}
}

unsigned int state::Statistics::get_life_point (){
	return(this->life_point);
}

unsigned int state::Statistics::get_attack_point (){
	return(this->attack_point);
}

unsigned int state::Statistics::get_move_point (){
	return(this->move_point);
}

void state::Statistics::set_statistics(Statistics given_stats)
{
	life_point = given_stats.life_point;
	attack_point = given_stats.attack_point;
	move_point = given_stats.move_point;
}

/*
void state::Statistics::Set_statistics (int vie, int attaque, int deplacement){
	if(0 <= vie && vie <= MAX_LIFE_POINT){
		this->life_point = vie;
	}
	else if(vie > MAX_LIFE_POINT){
		this->life_point = MAX_LIFE_POINT;
	}
	else{
		this->life_point = 0;
	}
	if(0 <= attaque && attaque <= MAX_ATTACK_POINT){
		this->attack_point = attaque;
	}
	else if(attaque > MAX_ATTACK_POINT){
		this->attack_point = MAX_ATTACK_POINT;
	}
	else{
		this->attack_point = 0;
	}
	if(0 <= deplacement && deplacement <= MAX_MOVE_POINT){
		this->move_point = deplacement;
	}
	else if(deplacement > MAX_MOVE_POINT){
		this->move_point = MAX_MOVE_POINT;
	}
	else{
		this->move_point = 0;
	}
	return;
}
*/
