#include "define.hpp"
#include "HyAI.h"
#include <iostream>
#include <cmath>

#include "engine/Move.h"
#include "state/GameState.h"

using namespace std;
using namespace ai;
using namespace engine;
using namespace render;

HyAI::HyAI(GameEngine& moteur) : AI(moteur){}
HyAI::~HyAI() {}


void HyAI::play()
{
	if(moteur.etat->current_player->name == "IA")
	{
		if (moteur.etat->ID == state::StateID::team_placement)
			place_character(moteur);

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


