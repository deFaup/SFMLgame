/**
 * @file : Statistics.cpp
 * @brief : fichier contenant la definition des methodes de la classe Statistics
 */

#include "define.hpp"
#include "Statistics.h"
#include <iostream>

using namespace state;
using namespace std;

state::Statistics::Statistics (int vie, int attaque, int deplacement): owner(NULL)
{
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

Statistics::Statistics(Characters* owner, CharactersID id) : owner(owner)
{
	switch (id)
	{
	case goku:
		life_point = 100;
		attack_point = 10;
		move_point = MAX_MOVE_POINT;
		break;

	case vegeta:
		life_point = 100;
		attack_point = 10;
		move_point = MAX_MOVE_POINT;
		break;

	case miyo:
		life_point = 100;
		attack_point = 10;
		move_point = MAX_MOVE_POINT;
		break;
	}
}

unsigned int state::Statistics::get_life_point () {	return(this->life_point); }

unsigned int state::Statistics::get_attack_point () { return(this->attack_point); }

unsigned int state::Statistics::get_move_point () { return(this->move_point); }

void state::Statistics::set_statistics(Statistics given_stats)
{
	life_point = given_stats.life_point;
	attack_point = given_stats.attack_point;
	move_point = given_stats.move_point;

	if (life_point <= 0)
	{
		life_point = 0; // we can't kill the character from a function that only exist because the character exist
		state::EventCharacters event(owner, Character_isDead);
		notifyObservers(event);
	}
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

void Statistics::increase_life_point(int offset)
{
	life_point += offset;
	if (life_point <= 0)
	{
		life_point = 0;
		state::EventCharacters event(owner, Character_isDead);
		notifyObservers(event);	
	}
	life_point = (life_point > MAX_LIFE_POINT) ? MAX_LIFE_POINT : life_point; // set to MAX_LIFE_POINT if too high
}

void Statistics::increase_attack_point(int offset)
{
	attack_point += offset;
	attack_point = (attack_point < 0) ? 0 : attack_point; // set to 0 if negative
	attack_point = (attack_point > MAX_ATTACK_POINT) ? MAX_ATTACK_POINT : attack_point; // set to MAX_LIFE_POINT if too hgh
}

void Statistics::increase_move_point(int offset)
{
	move_point += offset;
	move_point = (move_point < 0) ? 0 : move_point; // set to 0 if negative
	move_point = (move_point > MAX_MOVE_POINT) ? MAX_MOVE_POINT : move_point; // set to MAX_LIFE_POINT if too hgh
}
