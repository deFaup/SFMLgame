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
			/* Create a deep copy of the current GameState */
			GameState deep_gameState (moteur.etat);
			
			/* Create a GameEngine specific to the AI */
			GameEngine deep_engine(deep_gameState);

			// find the best character to make an action with min max
			int index (min_max(deep_engine.etat, 1, deep_engine.etat.current_player));

			// use this character to attack
			shared_ptr<state::Characters> attacker (moteur.etat.current_player->get_character(index));
			attack(moteur, moteur.etat, attacker);

			//attack(moteur.etat, moteur.etat.current_player->current_character);
			//sfEvents events(enter);
			//moteur.add_command(events);
		}
	}
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

int DeepAI::min_max(state::GameState& gameState, int depth, shared_ptr<state::Player> ai_player)
{
	static bool min_or_max = 0; min_or_max = !min_or_max; //1, then 0 on the next call to min_max, then 1 etc
	int weight(0);

	// compute the cost with evaluate function
	if (depth == 0)		
		return evaluation_function(ai_player);
	
	else
	{
		/// STEP 1 - Get all characters to try for min_max at current depth

		vector<shared_ptr<state::Characters>> char_to_try(gameState.get_characters());

		if (gameState.current_player->name != "IA")
			char_to_try = gameState.current_player->get_characters();

		for (unsigned int i(0); i < char_to_try.size(); i++)
		{	//delete dead characters and IA characters when current charcter is not IA
			if ((char_to_try[i]->stats.get_life_point() <= 0) ||
				(char_to_try[i]->get_Player()->name == "IA" && gameState.current_player->name != "IA"))
			{
				char_to_try[i].erase();
				i--;
			}
		}

		/// STEP 2 - go through the characters and apply min_max
		for (unsigned int i(0); i < char_to_try.size(); i++)
		{
			// char attack
			attack(deep_engine, gameState, char_to_try[i]);
			int val = min_max(gameState, depth - 1, ai_player);

			if (min_or_max == 1) //MAX
			{
				static int depth_max = val;
				weight = (val > depth_max) ? i : weight;
			}
			else
			{
				static int depth_min = val;
				weight = (val < depth_min) ? i : weight;
			}
		}
	}
	return weight;
}

int DeepAI::evaluation_function(shared_ptr<state::Player> ai_player)
{
	int sum = 0;
	for (unsigned int i = 0; i < ai_player->get_number_of_characters(); i++)
		sum += ai_player->get_character(i)->stats.get_life_point();

	return sum;
}

void DeepAI::attack(engine::GameEngine& gameEngine, state::GameState& gameState, std::shared_ptr<state::Characters> attacker)
{// rajouter si attacker dead

	std::shared_ptr<state::Characters> target;
	bool attack_finished = false;

	// make an action with attacker, modify the game state
	unsigned int distancemin;
	bool isCharacterChoose = false;

	while (!attack_finished)
	{
		// selection du joueur à prendre pour target
		if (!isCharacterChoose)
		{
			target = find_target(gameState, attacker, distancemin);
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
				isReachable = true; //break;
			}
		}

		// if reachable and you have attack point then you can send attack until target is dead
		if (isReachable) 
		{
			if (attacker->stats.get_attack_point() == 0) 
			{
				gameEngine.add_command(sfEvents(enter));
				isCharacterChoose = false; attack_finished = true;
			}
			else {
				sfEvents events(left_click);
				events.mouse_position = target->position;
				gameEngine.add_command(events);
				// sequential execution of the command because deep IA has its own engine/ in the same thread
				if (target->stats.get_life_point() == 0)
					isCharacterChoose = false;
			}
		}
	
		else {
			if (attacker->stats.get_move_point() == 0) {
				gameEngine.add_command(sfEvents(enter));
				isCharacterChoose = false; attack_finished = true;
			}
			else if (target->position.getPositionX() <= attacker->position.getPositionX()) {
				gameEngine.add_command(sfEvents (arrow_left));
			}
			else {
				gameEngine.add_command(sfEvents(arrow_right));
			}
		}
	}	
}

std::shared_ptr<state::Characters> DeepAI::find_target(
	const state::GameState& gameState, const std::shared_ptr<state::Characters> attacker, unsigned int& distance)
{
	std::shared_ptr<state::Characters> target;
	distance = 10000;
	for (unsigned int i = 0; i < gameState.characters.size(); i++)
	{
		const std::shared_ptr<state::Characters>& potential_target = gameState.characters[i];

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

GameState * DeepAI::StateCopy(const GameState etat) const
{
	// creation de la copie
	GameState * state_copy = (GameState *)malloc(sizeof(GameState));
	// copie des éléments de base du GameState
	for (unsigned int i = 0; i < etat.get_number_of_player(); i++)
	{
		// creation de la copie d'un joueur existant
		state_copy->new_player(etat.get_player(i)->name);
		std::shared_ptr<Player> original_player = etat.get_player(i);
		std::shared_ptr<Player> player_copy = state_copy->get_player(i);
		for (unsigned int j = 0; j < etat.get_player(i)->get_number_of_characters(); j++)
		{
			// creation de la copie d'un personnage existant
			state_copy->new_character(i, etat.get_player(i)->get_character(j)->get_id());
			std::shared_ptr<Characters> original_character = original_player->get_character(j);
			std::shared_ptr<Characters> character_copy = player_copy->get_character(j);
			character_copy->position.setPosition(character_copy->position.getPositionX(), character_copy->position.getPositionY());
			Statistics stats(original_character->stats.get_life_point(), original_character->stats.get_attack_point(), original_character->stats.get_move_point());
			character_copy->stats.set_statistics(stats);
			for (unsigned int k = 0; k < original_character->get_number_of_attacks(); k++)
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
