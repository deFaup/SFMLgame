// Includes

#include <iostream>
#include "define.hpp"
#include "ChangeCharacter.h"

using namespace std;
using namespace state;
using namespace engine;

int ChangeCharacter::isLegit(state::GameState etat)
{
	if (etat.get_ID() == not_started || etat.get_ID() == team_selected)
		return -1;

	else return 0;
}


void ChangeCharacter::execute(state::GameState& etat)
{
	static unsigned int i = 0;
	unsigned int number_of_characters = etat.current_player->get_number_of_characters();

	if (etat.get_number_of_player() > 0) // not so usefull here
	{
		etat.current_player->current_character = etat.current_player->get_character(i);
		i++;
	}

	if (i >= number_of_characters)
	{
		i %= number_of_characters;
		// move to next player with static function
	}
	cout << "executing current character change" << endl;
	return;
}