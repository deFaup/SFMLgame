// Includes

#include <iostream>
#include "define.hpp"
#include "ChangeCharacter.h"

using namespace std;
using namespace state;
using namespace engine;

int ChangeCharacter::isLegit(state::GameState& etat)
{
	if (etat.ID == (not_started || team_selected) )
		return -1;

	else return 0;
}


void ChangeCharacter::execute(state::GameState& etat)
{
	/*unsigned int limit_player = etat.get_number_of_player();
	unsigned int limit_character = etat.current_player->get_number_of_characters();

	static unsigned int i(0), j(0);
	etat.current_player = etat.get_player(i);
	etat.current_player->current_character = etat.get_player(i)->get_character(j);

	j++; 
	i += j / limit_character;
	j %= limit_character;
	i %= limit_player;*/
	unsigned int i = 0;
	std::shared_ptr<Characters> t_character = etat.current_player->get_character(i);
	while(t_character != etat.current_player->current_character){
		i++;
		t_character = etat.current_player->get_character(i);
	}
	if(nextcharacter == true){
		if(i == (etat.current_player->get_number_of_characters()-1)){
			etat.current_player->current_character = etat.current_player->get_character(0);
		}
		else{
			etat.current_player->current_character = etat.current_player->get_character(i+1);
		}
	}
	if(nextcharacter == false){
		if(i == 0){
			etat.current_player->current_character = etat.current_player->get_character(etat.current_player->get_number_of_characters()-1);
		}
		else{
			etat.current_player->current_character = etat.current_player->get_character(i-1);
		}
	}
	return;
}
