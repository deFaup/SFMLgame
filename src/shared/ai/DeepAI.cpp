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
void update_char_to_try(const engine::GameEngine& gameEngine, std::deque<shared_ptr<state::Characters>>& char_to_try);
DeepAI::DeepAI(GameEngine& moteur) : moteur(moteur){}

void DeepAI::play()
{
	static bool min_max_done = false; // false when it's not the turn of AI
									  // during AI turn false when not done else true

	static shared_ptr<state::Characters> attacker;

	if (moteur.etat->current_player->name == "IA")
	{
		if (moteur.etat->ID == state::StateID::team_placement)
			place_character(moteur);

		else if (moteur.etat->ID == state::StateID::started)
		{
			///* we are going to use a boolean until we put the AI in a separate thread
			//it allows to send a command then go back to main.cpp and update the graphics */
			
			static int index(0);
			if (!min_max_done)
			{
				/* Create a deep copy of the current GameState */
				GameState deep_gameState(*(moteur.etat)); //OK

				/* Create a GameEngine specific to the AI */
				GameEngine deep_engine(&deep_gameState); //OK

				// find the best character to make an action with min max
				int* best = min_max(deep_engine, 2, 1);
				
				index = best[1];
				std::cout << "min_max = " << index << "\n";
				attacker = moteur.etat->current_player->get_character(index); //pb when index is last char and that he died; we are out of bounds
				min_max_done = true;
			}
			else
			{
				// the following test is to determine whether or not our attacker has died
				if (moteur.etat->current_player->get_character(index) == attacker)
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
			//state::GameState deep_gameState(gameEngine.etat);

			if (attack(gameEngine, char_to_try[i]) == -1)
			{
				res[0] = evaluation_function(gameEngine); //res[1] = i;
				return res; // we can return res as we don't need to explore the other possibilities
			}
			// on fait return pour donner une valeur au parent directement sans parcourir tous les fils
			std::cout << "attack with character " << i << " done\n";

			if (gameEngine.etat->current_player->name == "IA") //if player is IA we move to the next player
				gameEngine.add_command(sfEvents(enter));

			// same thread so we can't use next_player function

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
			update_char_to_try(gameEngine, char_to_try);

			gameEngine.add_command(sfEvents(arrow_up));
			gameEngine.executeCommandes();
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

	for (auto ai_player : gameEngine.etat->players)
	{
		if (ai_player->name == "IA")
			for (unsigned int i = 0; i < ai_player->get_number_of_characters(); i++)
			{
				std::cout << "evaluation_function: i= " << i << endl;
				std::cout << "evaluation_function: id character= "
					<< ai_player->get_character(i)->get_id() << endl;
				sum += ai_player->get_character(i)->stats.get_life_point();
				std::cout << "evaluation_function: sum= " << sum << endl;
			}
	}

	
	return sum;
}

int DeepAI::attack(engine::GameEngine& gameEngine, std::shared_ptr<state::Characters> attacker)
{// rajouter si attacker dead

	state::GameState* gameState = gameEngine.etat;
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
		//std::cout << "\nattacker ptr: " << attacker->get_Player() << "\n";

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
		//cout << "find attack OK\n";

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
		//cout << "send commands OK\n";
		gameEngine.executeCommandes();

		if (target->stats.get_life_point() == 0)
			isCharacterChoose = false;
		
		if (attacker->stats.get_life_point() == 0)
		{
			attack_finished = true;
			erno = -2;
		}
		//cout << "execute commands OK\n"; //qq fois passer le tour marche pas
	}	
	return erno;
}

// executed once with a while loop
std::shared_ptr<state::Characters> DeepAI::find_target(
	const state::GameState* gameState, const std::shared_ptr<state::Characters> attacker, unsigned int& distance)
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

// attack real time (executed many times when it's the turn of the AI once min max is done, to see the update in the render)
bool DeepAI::attack_RT(engine::GameEngine& gameEngine, std::shared_ptr<state::Characters> attacker)
{
	// All problems fixed (when the attacker dies we move on to the next player
	// When we need to change player (sfEnter) we set GameEngine::updating to true then wait for the engine to change it to false

	// Ex problème: si le character ou le perso meurt, les var statiques ne sont pas réinit dans ce cas.

	state::GameState* gameState = gameEngine.etat;
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

void DeepAI::update_char_to_try(const engine::GameEngine& gameEngine, std::deque<shared_ptr<state::Characters>>& char_to_try)
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