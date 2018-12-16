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

/*RandomAI::RandomAI(){
}*/

RandomAI::RandomAI(engine::GameEngine& moteur) : moteur(moteur){}

void RandomAI::play(){
	if(moteur.etat.current_player->name == "IA")
	{
		if (moteur.etat.ID == state::StateID::team_placement)
		{
			std::shared_ptr<state::Player> ia_player = moteur.etat.current_player;

			int width(0), height(0);
			moteur.etat.map.get_dimensions(width, height);
			srand(time(NULL));

			static bool aligned = false;
			if (!aligned)
			{
				for (unsigned i = 0; i < ia_player->get_number_of_characters(); i++) // set them randomly accross the top of the map
				{
					int nb_aleatoire = (100 + rand()) % width - 100;
					ia_player->get_character(i)->position.setPosition(nb_aleatoire, 0);
				} aligned = true;
			}
			// In this part we are going to move down each character one by one

			static state::Position previous_position; static unsigned int i = 0;
			previous_position = ia_player->get_current_character()->position;

			// Move down the current character of the AI
			//for (int i = 0; i != 5; i++)
			{
				render::sfEventsID arrow = arrow_down;
				engine::Move move_commande(arrow);
				move_commande.execute(moteur.etat);
			}

			if (previous_position.getPositionY() == ia_player->get_current_character()->position.getPositionY())
			{
				sfEvents change_character(arrow_up);
				moteur.add_command(change_character);
				i++;
			}
			if (i == ia_player->get_number_of_characters())
			{
				std::cout << "next player ia\n";
				sfEvents next_player(enter);
				moteur.add_command(next_player);
				//moteur.add_command(sfEvents(space)); // now the game has started!
			}
		}

		else if (moteur.etat.ID == state::StateID::started)
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
		}
	}
	return;
}
