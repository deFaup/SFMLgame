#include "define.hpp"
#include "ChangePlayer.h"
#include <iostream>

using namespace state;
using namespace engine;
using namespace std;

int ChangePlayer::isLegit(state::GameState etat)
{
	if (etat.get_ID() == not_started || etat.get_ID() == team_selected)
		return -1;

	else return 0;
}

void ChangePlayer::execute(state::GameState& etat)
{
	static unsigned int i = 0;
	if (etat.get_number_of_player() > 0)
	{
		etat.current_player = etat.players[i];
		i++;
		i %= etat.get_number_of_player();
	}
	cout << "executing current player change" << endl;
	return;
}

