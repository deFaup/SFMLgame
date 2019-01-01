#include "define.hpp"
#include "HyAI.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "engine/Move.h"
#include "state/GameState.h"

using namespace std;
using namespace ai;
using namespace engine;
using namespace render;

HyAI::HyAI(GameEngine& moteur) : moteur(moteur){}

void HyAI::play()
{
	if(moteur.etat->current_player->name == "IA")
	{
		if (moteur.etat->ID == state::StateID::team_placement)
		{
			std::shared_ptr<state::Player> ia_player = moteur.etat->current_player;

			int width(0), height(0);
			moteur.etat->map.get_dimensions(width, height);
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
				move_commande.execute(*(moteur.etat));
			}

			if (previous_position.getPositionY() == ia_player->get_current_character()->position.getPositionY())
			{
				sfEvents change_character(arrow_up);
				moteur.add_command(change_character);
				i++;
			}
			if (i == ia_player->get_number_of_characters())
			{
				sfEvents next_player(enter);
				moteur.add_command(next_player);
				//moteur.add_command(sfEvents(space)); // now the game has started!
			}
		}

		else if (moteur.etat->ID == state::StateID::started)
		{
			static std::shared_ptr<state::Characters> target;
			static std::shared_ptr<state::Characters> attacker;
			static unsigned int distancemin;
			static bool isCharacterChoose = false;

			// selection du joueur à prendre pour target
			if (!isCharacterChoose) 
			{
				distancemin = 10000;
				for (unsigned int j = 0; j < moteur.etat->current_player->get_number_of_characters(); j++)
				{
					std::shared_ptr<state::Characters>& potential_attacker = moteur.etat->current_player->get_character(j);
					for (unsigned int i = 0; i < moteur.etat->characters.size(); i++)
					{
						std::shared_ptr<state::Characters>& potential_target = moteur.etat->characters[i];
						if (potential_target->get_Player() != moteur.etat->current_player.get())
						{
							int var = potential_target->position.getPositionX() - potential_attacker->position.getPositionX();
							var = (int)abs((double)var);
							if (var <= (int)distancemin) {
								target = potential_target;
								attacker = potential_attacker;
								distancemin = sqrt((potential_target->position.getPositionY() - potential_attacker->position.getPositionY())*(potential_target->position.getPositionY() - potential_attacker->position.getPositionY()) + (potential_target->position.getPositionX() - potential_attacker->position.getPositionX())*(potential_target->position.getPositionX() - potential_attacker->position.getPositionX()));
							}
						}
					}
				}
				isCharacterChoose = true;
			}
			else
			{
				distancemin = sqrt(
					(target->position.getPositionY() - attacker->position.getPositionY()) *
					(target->position.getPositionY() - attacker->position.getPositionY())
					+
					(target->position.getPositionX() - attacker->position.getPositionX()) *
					(target->position.getPositionX() - attacker->position.getPositionX())
				);
			}
			
			moteur.etat->current_player->current_character = attacker;
			unsigned int attack_number = 0;
			bool isReachable = false;
			// vérifie si la target est à portée d'attaque et si oui avec quelle attaque
			for (unsigned int i = 0; i < attacker->get_number_of_attacks(); i++) {
				if (attacker->get_attack(i).get_attack_scope() >= distancemin) {
					attack_number = i;
					isReachable = true;
				}
			}
			// en déduit la commande à lancer
			if (isReachable) {
				if (attacker->stats.get_attack_point() == 0) {
					sfEvents events(enter);
					moteur.add_command(events);
					isCharacterChoose = false;
					return;
				}
				else {
					sfEvents events(left_click);
					events.mouse_position = target->position;
					moteur.add_command(events);
					return;
				}
			}
			else {
				if (attacker->stats.get_move_point() == 0) {
					sfEvents events(enter);
					moteur.add_command(events);
					isCharacterChoose = true;
					return;
				}
				else if (target->position.getPositionX() <= attacker->position.getPositionX()) {
					sfEvents events(arrow_left);
					moteur.add_command(events);
					return;
				}
				else {
					sfEvents events(arrow_right);
					moteur.add_command(events);

				}
			}
		}
	
	}

	render::sfEventsID arrow = arrow_down;
	engine::Move move_commande(arrow);
	move_commande.execute(*(moteur.etat));
}


