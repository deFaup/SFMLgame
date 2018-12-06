#include "define.hpp"
#include "HyAI.h"
#include <cmath>
#include <iostream>

using namespace ai;
using namespace engine;
using namespace render;

HyAI::HyAI(GameEngine& moteur) : moteur(moteur){}

void HyAI::play(){
	if(moteur.etat.current_player->name == "IA"){
		static std::shared_ptr<state::Characters> cible;
		static std::shared_ptr<state::Characters> attaquant;
		static unsigned int distancemin;
		static bool isCharacterChoose = false;
		// selection du joueur à prendre pour cible
		if(!isCharacterChoose){
			distancemin = 10000;
			for(unsigned int j = 0; j < moteur.etat.current_player->get_number_of_characters(); j++){
				std::shared_ptr<state::Characters>& test_char = moteur.etat.current_player->get_character(j);
				for(unsigned int i = 0; i < moteur.etat.characters.size(); i++){
					std::shared_ptr<state::Characters>& curr_char = moteur.etat.characters[i];
					if(curr_char->get_Player() != moteur.etat.current_player.get()){
						if(abs(curr_char->position.getPositionX() - test_char->position.getPositionX()) <= distancemin){
							cible = curr_char;
							attaquant = test_char;
							distancemin = sqrt((curr_char->position.getPositionY() - test_char->position.getPositionY())*(curr_char->position.getPositionY() - test_char->position.getPositionY()) + (curr_char->position.getPositionX() - test_char->position.getPositionX())*(curr_char->position.getPositionX() - test_char->position.getPositionX()));
						}
					}
				}
			}
			isCharacterChoose = true;
		}
		else{
			for(unsigned int i = 0; i < moteur.etat.characters.size(); i++){
				std::shared_ptr<state::Characters>& curr_char = moteur.etat.characters[i];
				if(curr_char->get_Player() != moteur.etat.current_player.get()){
					if(abs(curr_char->position.getPositionX() - attaquant->position.getPositionX()) <= distancemin){
						cible = curr_char;
						distancemin = sqrt((curr_char->position.getPositionY() - attaquant->position.getPositionY())*(curr_char->position.getPositionY() - attaquant->position.getPositionY()) + (curr_char->position.getPositionX() - attaquant->position.getPositionX())*(curr_char->position.getPositionX() - attaquant->position.getPositionX()));
						}
					}
				}
		}
		moteur.etat.current_player->current_character = attaquant;
		unsigned int attack_number = 0;
		bool isReachable = false;
		// vérifie si la cible est à portée d'attaque et si oui avec quelle attaque
		for(unsigned int i = 0; i < attaquant->get_number_of_attacks(); i++){
			if(attaquant->get_attack(i).get_attack_scope() >= distancemin){
				attack_number = i;
				isReachable = true;
			}
		}
		// en déduit la commande à lancer
		if(isReachable){
			if(attaquant->stats.get_attack_point() == 0){
				sfEvents events(enter);
				moteur.add_command(events);
				isCharacterChoose = false;
				return;
			}
			else{
				sfEvents events(left_click);
				events.mouse_position = cible->position;
				moteur.add_command(events);
				return;
			}
		}
		else{
			if(attaquant->stats.get_move_point() == 0){
				sfEvents events(enter);
				moteur.add_command(events);
				isCharacterChoose = true;
				return;
			}
			else if(cible->position.getPositionX() <= attaquant->position.getPositionX()){
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


