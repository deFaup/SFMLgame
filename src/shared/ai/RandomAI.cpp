#include "define.hpp"
#include "RandomAI.h"
#include "define.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "engine/Move.h"
#include "state/GameState.h"
using namespace ai;
using namespace render;

RandomAI::RandomAI(engine::GameEngine& moteur) : AI(moteur){}
RandomAI::~RandomAI() {}

void RandomAI::play()
{
	if (moteur.etat->ID == state::StateID::team_placement)
		place_character(moteur);

	else if (moteur.etat->ID == state::StateID::started)
	{
		srand(time(NULL));
		int nb_aleatoire = rand() % 2;
		if (nb_aleatoire == 0 || nb_aleatoire == 1) {
			int nb_aleatoire2 = rand() % 2;
			int nb_aleatoire3 = rand() % 3;
			for (int k = 0; k < nb_aleatoire3; k++) {
				sfEvents events(arrow_up);
				moteur.add_command(events);
			}
			if (nb_aleatoire2 == 0) {
				for (int k = 0; k < (rand() % 100); k++) {
					sfEvents events(arrow_left);
					moteur.add_command(events);
				}
			}
			else {
				for (int k = 0; k < (rand() % 100); k++) {
					sfEvents events(arrow_right);
					moteur.add_command(events);
				}
			}
		}
		sfEvents events(enter);
		moteur.add_command(events);
		moteur.executeCommandes();
	}
	return;
}
