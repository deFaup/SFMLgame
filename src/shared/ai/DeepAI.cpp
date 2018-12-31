#include "define.hpp"
#include "DeepAI.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "engine/Move.h"
#include "state/GameState.h"
#include "global_mutex.hpp"

using namespace std;
using namespace state;
using namespace ai;
using namespace engine;
using namespace render;

DeepAI::DeepAI(GameEngine& moteur) : moteur(moteur){}

void DeepAI::play()
{
	static bool min_max_done = false; // false when it's not the turn of AI
									  // during AI turn false when not done else true

	static shared_ptr<state::Characters> attacker;

	if (moteur.etat.current_player->name == "IA")
	{
		if (moteur.etat.ID == state::StateID::team_placement)
			place_character(moteur);

		else if (moteur.etat.ID == state::StateID::started)
		{
			///* we are going to use a boolean until we put the AI in a separate thread
			//it allows to send a command then go back to main.cpp and update the graphics */
			
			static int index(0);
			if (!min_max_done)
			{
				/* Create a deep copy of the current GameState */
				GameState deep_gameState(moteur.etat); //OK

				/* Create a GameEngine specific to the AI */
				GameEngine deep_engine(deep_gameState); //OK

				// find the best character to make an action with min max
				index = min_max(deep_engine, 1, deep_engine.etat.current_player);
				std::cout << "min_max = " << index << "\n";
				attacker = moteur.etat.current_player->get_character(index); //pb when index is last char and that he died; we are out of bounds
				min_max_done = true;
			}
			else
			{
				// the following test is to determine whether or not our attacker has died
				if (moteur.etat.current_player->get_character(index) == attacker)
				{
					if (attack_RT(moteur, attacker))
						std::cout << "Deep AI is done\n";
				}
				else
				{
					next_player(moteur);
					//sfEvents events(enter);	moteur.add_command(events);
					//moteur.set_updating(true);	while (moteur.updating) {}
					//cout << "attacker is dead, next player\n";
				}
			}
		}
	}

	else
	{
		min_max_done = false;
		attacker = 0;
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
		cout << "ia placement finished begin\n";
		next_player(moteur);
		
		//moteur.set_updating(true);
		//event with the line above it seems that the engine can execute the exec function and have no commands in its list so it sets engine to false
		//while (moteur.updating) {}
		cout << "ia placement finished end\n";

		//moteur.add_command(sfEvents(space)); // now the game has started!
	}
}

// the engine is in the SAME thread
int DeepAI::min_max(engine::GameEngine& gameEngine, int depth, shared_ptr<state::Player> ai_player)
{//RE-INIT TOUS LES static quand tout est terminé!!
	static bool min_or_max = 0; //1 when AI , 0 else; set to 0 when we finish all the computation!!

	// 1 - local copy of the state
	//GameState this_layer_state(gameEngine.etat);

	// state that you can modify
	auto& gameState = gameEngine.etat;
	
	min_or_max = !min_or_max; //1, then 0 on the next call to min_max, then 1 etc
	int weight(0);

	// when the bottom of the tree is reached we compute the cost with evaluate function
	if (depth == 0) {
		cout << "bottom tree\n";
		return evaluation_function(ai_player);
	}
	
	else
	{
		cout << "\nbegin min_max with " << min_or_max << "(O is min)" << "\n";
		// 2 - Find all son of the father node 
		// father: this_layer_state; 
		// sons: father state modified by each char_to_try (valid AI_characters or AI_ennemies)

		std::deque<shared_ptr<state::Characters>> char_to_try(gameState.get_characters());

		if (gameState.current_player->name == "IA")
			char_to_try = gameState.current_player->get_characters();

		for (unsigned int i(0); i < char_to_try.size(); i++)
		{	//delete dead characters and IA characters when current charcter is not IA
			if ((char_to_try[i]->stats.get_life_point() <= 0) ||
				(char_to_try[i]->get_Player()->name == "IA" && gameState.current_player->name != "IA"))
			{
				char_to_try.erase(char_to_try.cbegin()+i);
				i--;
			}
		}
		// on considère que tous les adversaires forment un adversaire
		// il faut donc passer plusieurs fois le tour 
		// LE ENTER doit être fait dans la boucle for ci-dessous
		//if IA 1 enter; else nb_enter = nb_player - 1

		// 3 - Go through all sons and find their sons
		for (unsigned int i(0); i < char_to_try.size(); i++)
		{
			// char attack
			std::cout << "tree level: " << depth << ". attack with character " << i << "\n";

			// temp no rollback just gameState deep copy
			// so etat in GameEngine must be a pointer
			//state::GameState deep_gameState(gameEngine.etat);

			attack(gameEngine, char_to_try[i]);
			std::cout << "attack with character " << i << " done\n";

			// check if after the attack the AI or the ennemy died
			//for (unsigned int i(0); i < char_to_try.size(); i++)
			//{
			//	if ((char_to_try[i]->stats.get_life_point() <= 0))
			//	{
			//		char_to_try.erase(char_to_try.cbegin() + i);
			//		i--;
			//	}
			//}


			if (gameState.current_player->name == "IA") //if player is IA we move to the next player
			{
				gameEngine.add_command(sfEvents(enter));
				gameEngine.executeCommandes();
			}
			// same thread so we can't use next_player function

			else //if player is not the IA we skip turns so as IA is the next player
			{
				for (unsigned int i = 0; i < gameState.get_number_of_player() - 1; i++)
					gameEngine.add_command(sfEvents(enter));

				gameEngine.executeCommandes();
			}

			std::cout << "skip player OK\n";
			int val = min_max(gameEngine, depth - 1, ai_player);

			std::cout << "leave = " << val << "\n";

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
			
			// 4 - Restore the state to use for this layer
			//gameEngine.etat = &this_layer_state;

		}
	}
	
	// all static variables re-initialized
	min_or_max = 0;

	return weight;
}

int DeepAI:: evaluation_function(shared_ptr<state::Player> ai_player)
{
	int sum = 0;
	for (unsigned int i = 0; i < ai_player->get_number_of_characters(); i++)
	{
		std::cout << "evaluation_function: i= " << i << endl;
		std::cout << "evaluation_function: sum= " << sum << endl;
		std::cout << "evaluation_function: id character= "
			<< ai_player->get_character(i)->get_id() << endl;
		sum += ai_player->get_character(i)->stats.get_life_point();
	}
	return sum;
}

void DeepAI::attack(engine::GameEngine& gameEngine, std::shared_ptr<state::Characters> attacker)
{// rajouter si attacker dead

	auto& gameState = gameEngine.etat;
	std::shared_ptr<state::Characters> target;
	bool attack_finished = false;
	unsigned int distancemin;
	bool isCharacterChoose = false;
	cout << "begin while loop in DeepAI::attack\n";

	while (!attack_finished && (attacker->stats.get_life_point() > 0) )
	{
		// selection du joueur à prendre pour target
		if (!isCharacterChoose )
		{
			target = find_target(gameState, attacker, distancemin);
			if (!target) break;
			isCharacterChoose = true;
			cout << "find_target OK\n";
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
		cout << "find attack OK\n";

		// if reachable and you have attack point then you can send attack until target is dead
		if (isReachable) 
		{
			if (attacker->stats.get_attack_point() == 0) 
			{
				isCharacterChoose = false; attack_finished = true;
			}
			else {
				sfEvents events(left_click);
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
		cout << "send commands OK\n";
		gameEngine.executeCommandes();

		if (target->stats.get_life_point() == 0)
			isCharacterChoose = false;
		if (attacker->stats.get_life_point() == 0)
			attack_finished = true;
		cout << "execute commands OK\n"; //qq fois passer le tour marche pas
	}	
}

// executed once with a while loop
std::shared_ptr<state::Characters> DeepAI::find_target(
	const state::GameState& gameState, const std::shared_ptr<state::Characters> attacker, unsigned int& distance)
{
	std::shared_ptr<state::Characters> target(0);
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

// attack real time (executed many times when it's the turn of the AI once min max is done, to see the update in the render)
bool DeepAI::attack_RT(engine::GameEngine& gameEngine, std::shared_ptr<state::Characters> attacker)
{
	// All problems fixed (when the attacker dies we move on to the next player
	// When we need to change player (sfEnter) we set GameEngine::updating to true then wait for the engine to change it to false

	// Ex problème: si le character ou le perso meurt, les var statiques ne sont pas réinit dans ce cas.

	auto& gameState = gameEngine.etat;
	std::shared_ptr<state::Characters> target;
	bool attack_finished = false;
	unsigned int distancemin;
	bool isCharacterChoose = false;

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
			isCharacterChoose = false; attack_finished = true;
		}
		else {
			sfEvents events(left_click);
			events.mouse_position = target->position;
			gameEngine.add_command(events);
			//if (target->stats.get_life_point() == 0)
			//	isCharacterChoose = false;
		}
	}

	else {
		if (attacker->stats.get_move_point() == 0) {
			isCharacterChoose = false; attack_finished = true;
		}
		else if (target->position.getPositionX() <= attacker->position.getPositionX()) {
			gameEngine.add_command(sfEvents(arrow_left));
		}
		else {
			gameEngine.add_command(sfEvents(arrow_right));
		}
	}

	if (attack_finished)
	{
		sfEvents events(enter);
		gameEngine.add_command(events);
		std::unique_lock<std::mutex> unique_next_player(global::next_player);
		global::next_player_cv.wait(unique_next_player);

		//gameEngine.set_updating(true);
		//while (gameEngine.updating) {}
		cout << "attack finished\n";
	}

	cout << "send commands OK\n";

	return attack_finished;
}

// when the engine is executed in another thread !!
void DeepAI::next_player(engine::GameEngine& gameEngine)
{
	sfEvents next_player(enter);
	gameEngine.add_command(next_player);

	std::unique_lock<std::mutex> unique_next_player(global::next_player);
	global::next_player_cv.wait(unique_next_player);
}