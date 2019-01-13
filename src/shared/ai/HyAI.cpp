#include "define.hpp"
#include "HyAI.h"
#include <iostream>
#include <cmath>

#include "engine/Move.h"
#include "state/GameState.h"

using namespace std;
using namespace ai;
using namespace engine;

HyAI::HyAI(GameEngine& moteur) : AI(moteur){}
HyAI::~HyAI() {}


void HyAI::play()
{
	if (moteur.etat->ID == state::StateID::team_placement)
		place_character(moteur);

	else if (moteur.etat->ID == state::StateID::started)
	{
		isCharacterChoose = false;

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

		// find the best attack among the possible attacks of the attacker
		state::sfEventsID attack_number;

		// is target reachable on one turn ? with wich attack?
		bool isReachable = false;
		for (unsigned int i = 0; i < attacker->get_number_of_attacks(); i++) 
		{
			if (attacker->get_attack(i).get_attack_scope() >= distancemin) {
				isReachable = true;
				attack_number = (i == 1) ? state::sfEventsID::num1 : attack_number;
				//attack_number = (i == 2) ? state::sfEventsID::num2 : attack_number;
				//attack_number = (i == 3) ? state::sfEventsID::num3 : attack_number;
				//attack_number = (i == 4) ? state::sfEventsID::num4 : attack_number;
				//attack_number = (i == 5) ? state::sfEventsID::num5 : attack_number;
			}
		}

		// en déduit la commande à lancer
		if (isReachable) {
			if (attacker->stats.get_attack_point() == 0) {
				state::sfEvents events(state::sfEventsID::enter);
				moteur.add_command(events);
				isCharacterChoose = false;
			}
			else {
				state::sfEvents events(attack_number);
				events.mouse_position = target->position;
				moteur.add_command(events);
			}
		}
		else {
			if (attacker->stats.get_move_point() == 0) {
				state::sfEvents events(state::sfEventsID::enter);
				moteur.add_command(events);
				isCharacterChoose = true;
			}
			else if (target->position.getPositionX() <= attacker->position.getPositionX()) {
				state::sfEvents events(state::sfEventsID::arrow_left);
				moteur.add_command(events);
			}
			else {
				state::sfEvents events(state::sfEventsID::arrow_right);
				moteur.add_command(events);

			}
		}
	}
	
	moteur.executeCommandes();

	state::sfEventsID arrow = state::sfEventsID::arrow_down;
	engine::Move move_commande(arrow);
	move_commande.execute(*(moteur.etat));
}


