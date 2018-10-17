/**
 * @file : Statistics.cpp
 * @brief : fichier contenant la definition des methodes de la classe Statistics
 */

#include "define.hpp"
#include "Statistics.h"

state::Statistics::Statistics (int vie, int attaque, int deplacement){
	if(0 <= vie <= MAX_LIFE_POINT){
		this->life_point = vie;
	}
	else if(vie > MAX_LIFE_POINT){
		this->life_point = MAX_LIFE_POINT;
	}
	else{
		this->life_point = 0;
	}
	if(0 <= attaque <= MAX_ATACK_POINT){
		this->attack_point = attaque;
	}
	else if(vie > MAX_LIFE_POINT){
		this->attack_point = MAX_ATTACK_POINT;
	}
	else{
		this->attack_point = 0;
	}
	if(0 <= deplacement <= MAX_MOVE_POINT){
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

unsigned int state::Statistics::Get_life_points (){
	return(this->life_point);
}

unsigned int state::Statistics::Get_attack_points (){
	return(this->attack_point);
}

unsigned int state::Statistics::Get_move_points (){
	return(this->move_point);
}

void state::Statistics::Set_statistiques (int vie, int attaque, int deplacement){
	if(0 <= vie <= MAX_LIFE_POINT){
		this->life_point = vie;
	}
	else if(vie > MAX_LIFE_POINT){
		this->life_point = MAX_LIFE_POINT;
	}
	else{
		this->life_point = 0;
	}
	if(0 <= attaque <= MAX_ATACK_POINT){
		this->attack_point = attaque;
	}
	else if(vie > MAX_LIFE_POINT){
		this->attack_point = MAX_ATTACK_POINT;
	}
	else{
		this->attack_point = 0;
	}
	if(0 <= deplacement <= MAX_MOVE_POINT){
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

