/**
 * @file : Statistics.cpp
 * @brief : fichier contenant la definition des methodes de la classe Statistics
 */

#include "define"
#include "Statistics.h"

state::Statistics::Statistics (unsigned int vie, unsigned int attaque, unsigned int deplacement){
	if(vie <= MAX_LIFE_POINT){
		this->life_point = vie;
	}
	else{
		this->life_point = MAX_LIFE_POINT;
	}
	if(attaque <= MAX_ATTACK_POINT){
		this->attack_point = attaque;
	}
	else{
		this->attack_point = MAX_ATTACK_POINT;
	}
	if(deplacement <= MAX_MOVE_POINT){
		this->move_point = deplacement;
	}
	else{
		this->move_point = MAX_MOVE_POINT;
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
	return(this->life_point);
}

void state::Statistics::Set_statistiques (unsigned int vie, unsigned int attaque, unsigned int deplacement){
	if(vie <= MAX_LIFE_POINT){
		this->life_point = vie;
	}
	else{
		this->life_point = MAX_LIFE_POINT;
	}
	if(attaque <= MAX_ATTACK_POINT){
		this->attack_point = attaque;
	}
	else{
		this->attack_point = MAX_ATTACK_POINT;
	}
	if(deplacement <= MAX_MOVE_POINT){
		this->move_point = deplacement;
	}
	else{
		this->move_point = MAX_MOVE_POINT;
	}
	return;
}

