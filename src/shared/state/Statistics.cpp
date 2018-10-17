#include "state/Statistics.h"

namespace state {

state::Statistics::Statistics (int vie, int attaque, int deplacement){
	this->life_point = vie;
	this->attack_point = attaque;
	this->move_point = deplacement;
	return;
}

int state::Statistics::Get_life_points (){
	return(this->life_point);
}

int state::Statistics::Get_attack_points (){
	return(this->attack_point);
}

int state::Statistics::Get_move_points (){
	return(this->life_point);
}

void state::Statistics::Set_statistiques (int life_point, int attack_point, int move_point){
	this->life_point = life_point;
	this->attack_point = attack_point;
	this->move_point = move_point;
	return;
}

};
