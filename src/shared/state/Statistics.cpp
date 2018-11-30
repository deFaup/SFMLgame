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
		move_point = 100;
		break;

	case vegeta:
		life_point = 100;
		attack_point = 10;
		move_point = 100;
		break;

	case miyo:
		life_point = 100;
		attack_point = 10;
		move_point = 100;
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

void state::Statistics::reset_all_but_life(CharactersID id)
{
	switch (id)
	{
	case goku:
		//life_point = 100;
		attack_point = 10;
		move_point = 100;
		break;

	case vegeta:
		//life_point = 100;
		attack_point = 10;
		move_point = 100;
		break;

	case miyo:
		//life_point = 100;
		attack_point = 10;
		move_point = 100;
		break;
	}
}

void Statistics::set_life_point(int offset)
{
	life_point += offset;
	life_point %= MAX_LIFE_POINT;
	life_point = (life_point < 0) ? 0 : life_point; // set to 0 if negative
}

void Statistics::set_attack_point(int offset)
{
	attack_point += offset;
	attack_point %= MAX_ATTACK_POINT;
	attack_point = (attack_point < 0) ? 0 : attack_point; // set to 0 if negative
}

void Statistics::set_move_point(int offset)
{
	move_point += offset;
	move_point %= MAX_MOVE_POINT;
	move_point = (move_point < 0) ? 0 : move_point; // set to 0 if negative
}