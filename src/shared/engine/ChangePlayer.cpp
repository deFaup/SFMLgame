#include "define.hpp"
#include "ChangePlayer.h"
#include <iostream>
#include <memory>

using namespace state;
using namespace engine;
using namespace std;

int ChangePlayer::isLegit(state::GameState& etat)
{
	if (etat.ID == (not_started || team_selected) )
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
		etat.current_player->get_character(k)->reset_stats();
	}
	std::shared_ptr<Player> t_player = etat.get_player(i);
	unsigned int index = 0;
	cout << "\ncurrent player name " << etat.current_player->name << "\n";
	//cout << "current player ptr " << etat.current_player.get() << "\n";
	bool found = false;
	for (auto elem : etat.players)
	{
		//cout << index << endl;
		index++;
		
		//cout << "player " << index - 1 << " name: " << elem->name << "\n";
		//cout << "player " << index - 1 << " ptr: " << elem.get() << "\n";

		if (elem == etat.current_player && !found)
		{
			//cout << "nbr of players: " << etat.players.size() << "\n";
			//cout << "nbr of players: " << etat.get_number_of_player() << "\n";
			etat.current_player = etat.get_player((index == etat.players.size()) ? 0 : index);
			cout << "new player name " << etat.current_player->name << "\n";
			found = true;
		}
	}
	if (!found)
		throw std::runtime_error("ChangePlayer.cpp change player can't find current player in vector GameState::players");

	//while(t_player != etat.current_player){
	//	i++;  cout << i << endl;
	//	t_player = etat.get_player(i);
	//}
	//if(i == etat.get_number_of_player()-1){
	//	etat.current_player = etat.get_player(0);
	//}
	//else{
	//	etat.current_player = etat.get_player(i+1);
	//}
	cout << "change current player done\n" << endl;
	return;
}

