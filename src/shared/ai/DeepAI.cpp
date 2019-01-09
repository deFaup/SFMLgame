#include "define.hpp"
#include "DeepAI.h"
#include <iostream>
#include <cmath>

#include "engine.h"

using namespace std;
using namespace state;
using namespace ai;
using namespace engine;
using namespace render;

DeepAI::DeepAI(GameEngine& moteur) : AI(moteur){}
DeepAI::~DeepAI() {}

void DeepAI::play()
{
	/*static*/ bool min_max_done = false; // false when it's not the turn of AI
									  // during AI turn false when not done else true

	/*static*/ shared_ptr<state::Characters> attacker;

	if (moteur.etat->ID == state::StateID::team_placement)
		place_character(moteur);

	else if (moteur.etat->ID == state::StateID::started)
	{
		///* we are going to use a boolean until we put the AI in a separate thread
		//it allows to send a command then go back to main.cpp and update the graphics */

		static int index(0);
		if (!min_max_done)
		{
			/* Create a GameEngine specific to the AI */
			GameEngine deep_engine(moteur.etat); //OK

			// find the best character to make an action with min max 
			// parameters: depth=2, 1 is mandatory for AI
			int* best = min_max(deep_engine, 2, 1);

			index = best[1];
			std::cout << "min_max = " << index << "\n";

			// we need to set char n°index as the current char
			attacker = moteur.etat->current_player->get_character(index); 
			moteur.etat->current_player->current_character = attacker;
			min_max_done = true;
		}

		/*else*/ if (min_max_done)
		{
			// version avec attack
			// if the attacker dies it's not a problem. we wait attack to be done then call next_player
			attack(moteur, attacker);
			std::cout << "min_max done so next_player\n";
			next_player(moteur);

			min_max_done = false;
		}
	}
}

// the engine is in the SAME thread
// return the min or max value and the index of the value
int res[2] = { 0,0 };
int* DeepAI::min_max(engine::GameEngine& gameEngine, int depth, bool min_or_max)
{
	// return value : result, index
	res[0] = 0; res[1] = 0;

	// when the bottom of the tree is reached we compute the cost with evaluate function
	if (depth == 0) {
		res[0] = evaluation_function(gameEngine);
		cout << "bottom tree = " << res[0] << "\n";
		return res;
	}
	
	else
	{
		// 1 - local copy of the state
		state::GameState this_layer_state(*(gameEngine.etat));
		std::cout << "copy layer state: " << depth << "\n";

		// sons'weight for one layer
		std::vector<int> sons_weight;

		cout << "\nbegin min_max with " << min_or_max << "(O is min)" << "\n";

		// 2 - Find all sons of the father node 
		// father: this_layer_state; 
		// sons: father state modified by each char_to_try (valid AI_characters or AI_ennemies)
		std::deque<shared_ptr<state::Characters>> char_to_try;// (gameState->get_characters());
		update_char_to_try(gameEngine, char_to_try);

		int size = char_to_try.size();
		
		// 3 - Check if the player has lost
		if (char_to_try.empty())
		{
			std::cout << "player lost\n";
			res[0] = evaluation_function(gameEngine);
			return res;
		}

		// 4 - Go through all living sons and find their sons
		for (int i(0); i < size; i++)
		{
			// char attack
			std::cout << "tree level: " << depth << ". attack with character " << i << "\n";

			// temp no rollback just gameState deep copy
			// so etat in GameEngine must be a pointer

			// x - copy of the state for this character, deleted at each incrementation
			state::GameState loop_state(this_layer_state);
			gameEngine.etat = &loop_state;
			std::cout << "loop state: " << i << "\n";
//std::cout << "current player charc: " << gameEngine.etat->current_player->current_character->id << "\n";
//std::cout << "current char ptr: " << gameEngine.etat->current_player->current_character.get() << "\n";

			update_char_to_try(gameEngine, char_to_try);

			if (attack(gameEngine, char_to_try[i]) == -1)
			{
				res[0] = evaluation_function(gameEngine); //res[1] = i;
				return res; // we can return res as we don't need to explore the other possibilities
			}
			// on fait return pour donner une valeur au parent directement sans parcourir tous les fils
			std::cout << "attack with character " << i << " done\n";

			if (gameEngine.etat->current_player->name == "IA") //if player is IA we move to the next player
				gameEngine.add_command(sfEvents(enter));

			else //if player is not the IA we skip turns so as IA is the next player
			{
				for (unsigned int i = 0; i < gameEngine.etat->get_number_of_player() - 1; i++)
					gameEngine.add_command(sfEvents(enter));
			}

			gameEngine.executeCommandes();
			std::cout << "skip player OK\n";
			
			int* val = min_max(gameEngine, depth - 1, !min_or_max);
			sons_weight.push_back(val[0]);
			std::cout << "weight = " << val[0] << "\n";
			
			// 4 - Restore the state to use for this layer
			std::cout << "restoring layer state: " << depth << "\n\n";
			gameEngine.etat = &this_layer_state;

		}

		// 5 - find min or max value
		res[0] = sons_weight[0];

		if (min_or_max == 1) //MAX
		{
			for (unsigned int i = 0; i < sons_weight.size(); i++)
			{
				if (sons_weight[i] > res[0])
				{
					res[0] = sons_weight[i];
					res[1] = i;
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < sons_weight.size(); i++)
			{
				if (sons_weight[i] < res[0])
				{
					res[0] = sons_weight[i];
					res[1] = i;
				}
			}
		}
		return res;
	}
}

int DeepAI:: evaluation_function(engine::GameEngine& gameEngine)
{
	int sum = 0;
	int coeff = 0;

	for (auto ai_player : gameEngine.etat->players)
	{
		if (ai_player->name == "IA")
			coeff = +1;
		else
			coeff = -1;

		for (unsigned int i = 0; i < ai_player->get_number_of_characters(); i++)
		{
			//std::cout << "evaluation_function: i= " << i << endl;
			//std::cout << "evaluation_function: id character= "
				//<< ai_player->get_character(i)->get_id() << endl;
			sum += coeff * (ai_player->get_character(i)->stats.get_life_point());
			//std::cout << "evaluation_function: sum= " << sum << endl;
		}
	}

	
	return sum;
}

int DeepAI::attack(engine::GameEngine& gameEngine, std::shared_ptr<state::Characters> attacker)
{// rajouter si attacker dead

	state::GameState* gameState = gameEngine.etat;
	gameState->current_player->current_character = attacker;
	std::cout << "character ptr: " << attacker.get() /*get_Player()*/ << "\n";
	std::cout << "engine player-character ptr: ";
	std::cout << gameEngine.etat->current_player->current_character.get() << "\n";

	std::shared_ptr<state::Characters> target;
	bool attack_finished = false;
	unsigned int distancemin;
	bool isCharacterChoose = false;
	cout << "begin while loop in DeepAI::attack\n";

	int erno = 0;

	while (!attack_finished && (attacker->stats.get_life_point() > 0) )
	{
		//cout << "attacker name|LP/AP/MP: " << attacker->get_id();
		//cout << " | " << attacker->stats.get_life_point();
		//cout << " / " << attacker->stats.get_attack_point();
		//cout << " / " << attacker->stats.get_move_point() << "\n";
		//std::cout << "\ncharacter ptr: " << attacker.get() /*get_Player()*/ << "\n";
		//std::cout << "engine player-character ptr: ";
		//std::cout << gameEngine.etat->current_player->current_character.get() << "\n";

		// selection du joueur à prendre pour target
		// if no target then this player has won
		if (!isCharacterChoose )
		{
			target = find_target(gameState, attacker, distancemin);
			if (!target) {	erno = -1; cout << "no target\n"; break;	}
			isCharacterChoose = true;
			cout << "find_target OK";
			std::cout << ": " << target->get_Player() <<  "\n";
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
		//cout << "target   name|LP/AP/MP: " << target->get_id();
		//cout << " | " << target->stats.get_life_point();
		//cout << " / " << target->stats.get_attack_point();
		//cout << " / " << target->stats.get_move_point() << "\n";
		
		// find the best attack among the possible attacks of the attacker
		render::sfEventsID attack_number;

		// is target reachable on one turn ? with wich attack?
		bool isReachable = false;
		for (unsigned int i = 0; i < attacker->get_number_of_attacks(); i++) 
		{
			if (attacker->get_attack(i).get_attack_scope() >= distancemin) {
				isReachable = true;
				attack_number = (i == 1) ? render::sfEventsID::num1 : attack_number;
				//attack_number = (i == 2) ? render::sfEventsID::num2 : attack_number;
				//attack_number = (i == 3) ? render::sfEventsID::num3 : attack_number;
				//attack_number = (i == 4) ? render::sfEventsID::num4 : attack_number;
				//attack_number = (i == 5) ? render::sfEventsID::num5 : attack_number;
				//std::cout << "target reachable OK\n";
				break;
			}
		}

		// if reachable and you have attack point then you can send attack until target is dead
		if (isReachable) 
		{
			if (attacker->stats.get_attack_point() == 0) 
			{
				isCharacterChoose = false; attack_finished = true;
			}
			else {
				sfEvents events(attack_number);
				events.mouse_position = target->position;
				gameEngine.add_command(events);
			}
		}
	
		else {
			if (attacker->stats.get_move_point() == 0) {
				isCharacterChoose = false; attack_finished = true;
			}
			else if (target->position.getPositionX() <= attacker->position.getPositionX()) {
				gameEngine.add_command(sfEvents (arrow_left));
			}
			else {
				gameEngine.add_command(sfEvents(arrow_right));
			}
		}
		//cout << "send commands OK\n";
		gameEngine.executeCommandes();

		if (target->stats.get_life_point() == 0)
			isCharacterChoose = false;
		
		if (attacker->stats.get_life_point() == 0)
		{
			attack_finished = true;
			erno = -2;
		}
		//cout << "execute commands OK\n";
	}	
	return erno;
}

// executed once with a while loop
std::shared_ptr<state::Characters> DeepAI::find_target(const state::GameState* gameState, 
	const std::shared_ptr<state::Characters> attacker, unsigned int& distance)
{
	std::shared_ptr<state::Characters> target(0);
	distance = 10000;
	for (unsigned int i = 0; i < gameState->characters.size(); i++)
	{
		const std::shared_ptr<state::Characters>& potential_target = gameState->characters[i];

		//targets can be dead as we don't remove them from the state
		if (potential_target->get_Player() != attacker->get_Player() &&
			potential_target->stats.get_life_point() > 0)
		{
			//std::cout << "target: " << potential_target->get_Player();
			////std::cout << "\nattacker: " << attacker->get_Player() << "\n";

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

void DeepAI::update_char_to_try(const engine::GameEngine& gameEngine, 
	std::deque<shared_ptr<state::Characters>>& char_to_try)
{
	if (gameEngine.etat->current_player->name == "IA")
		char_to_try = gameEngine.etat->current_player->get_characters();
	else
		char_to_try = gameEngine.etat->get_characters();

	for (unsigned int i(0); i < char_to_try.size(); i++)
	{	//delete dead characters and IA characters when current charcter is not IA
		if ((char_to_try[i]->stats.get_life_point() <= 0) ||
			(char_to_try[i]->get_Player()->name == "IA" && gameEngine.etat->current_player->name != "IA"))
		{
			char_to_try.erase(char_to_try.cbegin() + i);
			i--;
		}
	}
}