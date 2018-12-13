#include "define.hpp"
#include "DeepAI.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace state;
using namespace ai;
using namespace engine;

DeepAI::DeepAI(GameEngine& moteur) : moteur(moteur){}


GameState * DeepAI::StateCopy (const GameState etat) const
{
	// creation de la copie
	GameState * state_copy = (GameState *)malloc(sizeof(GameState));
	// copie des éléments de base du GameState
	for(unsigned int i = 0; i < etat.get_number_of_player(); i++)
	{
		// creation de la copie d'un joueur existant
		state_copy->new_player(etat.get_player(i)->name);
		std::shared_ptr<Player> original_player = etat.get_player(i);
		std::shared_ptr<Player> player_copy = state_copy->get_player(i);
		for(unsigned int j = 0; j < etat.get_player(i)->get_number_of_characters(); j++)
		{
			// creation de la copie d'un personnage existant
			state_copy->new_character(i,etat.get_player(i)->get_character(j)->get_id());
			std::shared_ptr<Characters> original_character = original_player->get_character(j);
			std::shared_ptr<Characters> character_copy = player_copy->get_character(j);
			character_copy->position.setPosition(character_copy->position.getPositionX(),character_copy->position.getPositionY());
			Statistics stats(original_character->stats.get_life_point(),original_character->stats.get_attack_point(),original_character->stats.get_move_point());
			character_copy->stats.set_statistics(stats);
			for(unsigned int k = 0; k < original_character->get_number_of_attacks(); k++)
			{
				// ajout des attaques à la copie du personnage
				character_copy->addAttack(original_character->get_attack(k));
			}
		}
	}
	// creation d'une copie de la map
	state_copy->map.set_mask(etat.map.get_mask());
	return(state_copy);
	
}
