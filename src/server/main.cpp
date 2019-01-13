#include <iostream>
#include <string>	//string
#include <cstring>	//strcmp

#include "define.hpp"
#include "global_mutex.hpp"

#include "state.h"
#include "server.h"
#include "engine.h"
#include "ai.h"
#include <thread>

using namespace std;
using namespace state;
using namespace engine;
using namespace ai;

void init_game(state::GameState* etat, int& player_1_type, int& player_2_type);

void enginet(int player_1_type, int player_2_type)
{

	GameState etat;
	GameEngine engine(&etat); std::thread thread_engine;
	shared_ptr<server::Server> m_server = make_shared<server::Server>(&etat);

	/* Linking the observer to each observable */
	//in Characters::stats & position + Player + Map + GameState
	etat.registerObserver(m_server);
	etat.map.registerObserver(m_server);
	cout << "main: observers listed\n" << endl;
	
	/* Init game */
	init_game(&etat, player_1_type, player_2_type);
	global::json_file.open(JSON_FILENAME);
	engine.JSONActive = true;
	
	/* config AI */
	shared_ptr<AI> ai_1(0); std::thread thread_ai_1;
	shared_ptr<AI> ai_2(0); std::thread thread_ai_2;
	switch (player_1_type)
	{
	case REAL://this player is not an AI
		break;
	case RANDOM_AI:
		ai_1 = make_shared<RandomAI>(engine);
		break;
	case HEURISTIC_AI:
		ai_1 = make_shared<HyAI>(engine);
		break;
	case DEEP_AI:
		ai_1 = make_shared<DeepAI>(engine);
		break;
	default:
		break;
	}
	switch (player_2_type)
	{
	case REAL://this player is not an AI
		break;
	case RANDOM_AI:
		ai_2 = make_shared<RandomAI>(engine);
		break;
	case HEURISTIC_AI:
		ai_2 = make_shared<HyAI>(engine);
		break;
	case DEEP_AI:
		ai_2 = make_shared<DeepAI>(engine);
		break;
	default:
		break;
	}
	if (ai_1) ai_1->name = AI_1;
	if (ai_2) ai_2->name = AI_2;
	cout << "main: IA created" << endl;

	thread_engine = thread(&engine::GameEngine::workLoop, &engine);
	if (ai_1)
		thread_ai_1 = thread(&ai::DeepAI::workloop, ai_1);	
	if (ai_2)
		thread_ai_2 = thread(&ai::DeepAI::workloop, ai_2);

	//while (etat.ID != state::StateID::end)
	//{
	//	// AI are working in separate thread
	//}

	thread_engine.join();
	cout << "engine thread closed\n";

	if (ai_1) thread_ai_1.join();
	if (ai_2) thread_ai_2.join();
	cout << "ai thread closed\n";

	/* Game is over */
	engine.JSONActive = false;
	global::json_file << global::json_commandes;
	global::json_file.close();
}

int main(int argc, char* argv[])
{

	if (argc == 2)
	{
		if (strcmp(argv[1], "record") == 0) enginet(HEURISTIC_AI, HEURISTIC_AI);
	}
	return 0;
}

/* Init the game with two players: player can be AI or real */
void init_game(state::GameState* etat, int& player_1_type, int& player_2_type)
{
	/* Create players, characters and a map. Will be rewritten when menu is implemented */
	if (etat->ID == not_started)
	{
		etat->new_map(3000, 2000);

		if (player_1_type == REAL) etat->new_player("Joueur 1");
		else etat->new_player(AI_1);
		
		if (player_2_type == REAL) etat->new_player("Joueur 2");
		else etat->new_player(AI_2);

		etat->new_character(0, vegeta);
		etat->new_character(0, vegeta);
		etat->new_character(0, vegeta);

		etat->new_character(1, goku);
		etat->new_character(1, goku);
		etat->new_character(1, goku);
		
		etat->ID = team_selected;
	}
}

