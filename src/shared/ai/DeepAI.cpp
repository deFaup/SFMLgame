#include "define.hpp"
#include "DeepAI.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "engine/Move.h"
#include "state/GameState.h"

using namespace std;
using namespace state;
using namespace ai;
using namespace engine;
using namespace render;

DeepAI::DeepAI(GameEngine& moteur) : moteur(moteur){}

void DeepAI::play()
{
	if (moteur.etat.current_player->name == "IA")
	{
		if (moteur.etat.ID == state::StateID::team_placement)
			place_character(moteur);

		else if (moteur.etat.ID == state::StateID::started)
		{
			// find the best character to make an action with min max
			// shared_ptr<state::Characters> attacker = min_max();

			// use this character to attack
			// attacker..

			//unique_ptr<MinMaxGenerator> generator(new MinMaxGenerator);
			//shared_ptr<Engine> engine = make_shared<Engine>(e);
			//State s = engine->getState();
			//BestAction action = generator->compute(s, 3, player, enemy);

			//BestAction MinMaxGenerator::compute(State s, uint epoch, uint playerId, uint enemyId) 
			//{
			//	BestAction action = this->tour(s, MAX, epoch, playerId, enemyId, HEAL);
			//	cerr << "cost end compute :" << action.getCost() << endl;
			//	return action;
			//}

			attack(moteur.etat, moteur.etat.current_player->current_character);
			//sfEvents events(enter);
			//moteur.add_command(events);
		}
	}
}
//int DeepAI::min_max(
//	state::GameState& state_deepCopy, const int depth)
//{
//	vector<shared_ptr<state::Characters>> player_characters;
//
//	player_characters = state_deepCopy.get_characters();
//	
//	// if we are not working the AI characters then we consider all others characters are its ennemies.
//	if (state_deepCopy.current_player->name != "IA")
//	{
//		for (unsigned int i(0); i < player_characters.size(); i++)
//			if (player_characters[i]->get_Player()->name == "IA")
//			{
//				player_characters[i].erase();
//				i--;
//			}
//	}
//	
//	for (unsigned int i(0); i < player_characters.size(); i++)
//	{
//		// at each level of the tree we take all the characters and make them attack one by one and get the Min or Max value
//		//player_characters[i] is going to attack
//		//get min
//	}
//	//return;
//}

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

void DeepAI::place_character(GameEngine& moteur)
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
		sfEvents next_player(enter);
		moteur.add_command(next_player);
		//moteur.add_command(sfEvents(space)); // now the game has started!
	}
}

int DeepAI::evaluation_function(shared_ptr<state::Player> ai_player)
{
	int sum = 0;
	for (unsigned int i = 0; i < ai_player->get_number_of_characters(); i++)
		sum += ai_player->get_character(i)->stats.get_life_point();

	return sum;
}

void DeepAI::attack(state::GameState& state_deepCopy, std::shared_ptr<state::Characters> attacker)
{
	static std::shared_ptr<state::Characters> target;
	static bool attack_finished = false;
	//temp
	auto& deep_engine = moteur;
	// make an action with attacker, modify the game state
	//while (!attack_finished)
	//{
		unsigned int distancemin;
		bool isCharacterChoose = false;

		// selection du joueur à prendre pour target
		if (!isCharacterChoose)
		{
			target = find_target(state_deepCopy, attacker, distancemin);
			isCharacterChoose = true;
		}

		// if already selected then we update the distance between attacker and target
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

		// future implementation: find the best attack among the possible attacks of the attacker
		//unsigned int attack_number = 0;

		// is target reachable on one turn ? with wich attack?
		bool isReachable = false;
		for (unsigned int i = 0; i < attacker->get_number_of_attacks(); i++) 
		{
			if (attacker->get_attack(i).get_attack_scope() >= distancemin) {
				//attack_number = i;
				isReachable = true; break;
			}
		}
		
		// if reachable and you have attack point then you can send attack until target is dead
		if (isReachable) 
		{
			if (attacker->stats.get_attack_point() == 0) 
			{
				sfEvents events(enter);
				deep_engine.add_command(events);
				isCharacterChoose = false; attack_finished = true;
			}
			else {
				sfEvents events(left_click);
				events.mouse_position = target->position;
				deep_engine.add_command(events);

				while (deep_engine.updating) {/* wait for commands to be executed */}
				if (target->stats.get_life_point() == 0)
					isCharacterChoose = false;
				return;
			}
		}
	
		else {
			if (attacker->stats.get_move_point() == 0) {
				sfEvents events(enter);
				deep_engine.add_command(events);
				isCharacterChoose = false; attack_finished = true;
				return;
			}
			else if (target->position.getPositionX() <= attacker->position.getPositionX()) {
				sfEvents events(arrow_left);
				deep_engine.add_command(events);
				return;
			}
			else {
				sfEvents events(arrow_right);
				deep_engine.add_command(events);

			}
		}

		//while (deep_engine.updating) {/* wait for commands to be executed */ }
	//}	

}

std::shared_ptr<state::Characters> DeepAI::find_target(
	const state::GameState& state_deepCopy, const std::shared_ptr<state::Characters> attacker, unsigned int& distance)
{
	std::shared_ptr<state::Characters> target;
	distance = 10000;
	for (unsigned int i = 0; i < state_deepCopy.characters.size(); i++)
	{
		const std::shared_ptr<state::Characters>& potential_target = state_deepCopy.characters[i];

		//targets can be dead as we don't remove them from the state
		if (potential_target->get_Player() != attacker->get_Player() &&
			potential_target->stats.get_life_point() > 0)
		{
			int var = potential_target->position.getPositionX() - attacker->position.getPositionX();
			var = (int)abs((double)var);
			if (var <= (int)distance) {
				target = potential_target;
				distance = sqrt(
					(target->position.getPositionY() - attacker->position.getPositionY()) *
					(target->position.getPositionY() - attacker->position.getPositionY())
					+
					(target->position.getPositionX() - attacker->position.getPositionX()) *
					(target->position.getPositionX() - attacker->position.getPositionX())
				);
			}
		}
	}
	return target;
}