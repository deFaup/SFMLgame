#include <iostream>
#include <cstdlib>

#include "define.hpp"
#include "define.hpp"
#include "global_mutex.hpp"
#include "RandomAI.h"
#include "engine/Move.h"
#include "state/GameState.h"

using namespace ai;

RandomAI::RandomAI(engine::GameEngine& moteur) : AI(moteur){}
RandomAI::~RandomAI() {}

void RandomAI::play()
{
	if (moteur.etat->ID == state::StateID::team_placement)
		place_character(moteur);

	else if (moteur.etat->ID == state::StateID::started)
	{
		srand(global::server_seed);
		int nb_aleatoire = rand() % 2;
		if (nb_aleatoire == 0 || nb_aleatoire == 1) {
			int nb_aleatoire2 = rand() % 2;
			int nb_aleatoire3 = rand() % 3;
			for (int k = 0; k < nb_aleatoire3; k++) {
				state::sfEvents events(state::sfEventsID::arrow_up);
				moteur.add_command(events);
			}
			if (nb_aleatoire2 == 0) {
				for (int k = 0; k < (rand() % 100); k++) {
					state::sfEvents events(state::sfEventsID::arrow_left);
					moteur.add_command(events);
				}
			}
			else {
				for (int k = 0; k < (rand() % 100); k++) {
					state::sfEvents events(state::sfEventsID::arrow_right);
					moteur.add_command(events);
				}
			}
		}
		state::sfEvents events(state::sfEventsID::enter);
		moteur.add_command(events);
		moteur.executeCommandes();
	}
	return;
}
