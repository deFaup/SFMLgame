#include "define.hpp"
#include "ChangePlayer.h"
#include <iostream>
#include <memory>

using namespace state;
using namespace engine;
using namespace std;

int ChangePlayer::isLegit(state::GameState& etat)
{
	if (etat.get_ID() == not_started || etat.get_ID() == team_selected)
		return -1;

	else return 0;
}

void ChangePlayer::execute(state::GameState& etat)
{
	/*static unsigned int i = 0;
	if (etat.get_number_of_player() > 0)
	{
		etat.current_player = etat.players[i];
		i++;
		i %= etat.get_number_of_player();
	}*/
	/*std::shared_ptr<Characters> curr = etat.current_player->current_character;*/
	unsigned int i = 0;
	for(unsigned int k = 0; k < etat.current_player->get_number_of_characters(); k++ ){
		etat.current_player->get_character(k)->get_statistics().reset_all_but_life(etat.current_player->get_character(k)->get_id());
	}
	std::shared_ptr<Player> t_player = etat.get_player(i);
	while(t_player != etat.current_player){
		i++;
		t_player = etat.get_player(i);
	}
	if(i == etat.get_number_of_player()-1){
		etat.current_player = etat.get_player(0);
	}
	else{
		etat.current_player = etat.get_player(i+1);
	}
	cout << "executing current player change" << endl;
	return;
}

