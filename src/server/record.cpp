#include <iostream>
#include <string>	//string

#include "define.hpp"
#include "global_mutex.hpp"

#include "state.h"
#include "server.h"
#include "engine.h"
#include "ai.h"
#include <thread>

using namespace state;
using namespace engine;
using namespace ai;
using namespace server;


/* Init the game with two players: player can be AI or real */
static void init_game(state::GameState* etat, int& player_1_type, int& player_2_type)
{
	global::rng.seed(std::random_device()());

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

void record(int player_1_type, int player_2_type)
{
	GameState etat;
	GameEngine engine(&etat); std::thread thread_engine;
	std::shared_ptr<server::GameServer> m_server = std::make_shared<server::GameServer>(&etat, &engine);

	/* Linking the observer to each observable */
	//in Characters::stats & position + Player + GameState
	etat.registerObserver(m_server);
	std::cout << "main: observers listed\n\n";

	/* Init game */
	init_game(&etat, player_1_type, player_2_type);
	std::ofstream json_out_file;
	json_out_file.open(JSON_FILENAME);
	engine.JSONActive = true;

	/* config AI */
	std::shared_ptr<AI> ai_1(0); std::thread thread_ai_1;
	std::shared_ptr<AI> ai_2(0); std::thread thread_ai_2;
	switch (player_1_type)
	{
	case REAL://this player is not an AI
		break;
	case RANDOM_AI:
		ai_1 = std::make_shared<RandomAI>(engine);
		break;
	case HEURISTIC_AI:
		ai_1 = std::make_shared<HyAI>(engine);
		break;
	case DEEP_AI:
		ai_1 = std::make_shared<DeepAI>(engine);
		break;
	default:
		break;
	}
	switch (player_2_type)
	{
	case REAL://this player is not an AI
		break;
	case RANDOM_AI:
		ai_2 = std::make_shared<RandomAI>(engine);
		break;
	case HEURISTIC_AI:
		ai_2 = std::make_shared<HyAI>(engine);
		break;
	case DEEP_AI:
		ai_2 = std::make_shared<DeepAI>(engine);
		break;
	default:
		break;
	}
	if (ai_1) ai_1->name = AI_1;
	if (ai_2) ai_2->name = AI_2;
	std::cout << "main: IA created\n";

	thread_engine = std::thread(&engine::GameEngine::workLoop, &engine);
	if (ai_1)
		thread_ai_1 = std::thread(&ai::DeepAI::workloop, ai_1);
	if (ai_2)
		thread_ai_2 = std::thread(&ai::DeepAI::workloop, ai_2);

	thread_engine.join();
	std::cout << "engine thread closed\n";

	if (ai_1) thread_ai_1.join();
	if (ai_2) thread_ai_2.join();
	std::cout << "ai thread closed\n";

	/* Game is over */
	engine.JSONActive = false;
	json_out_file << global::json_commandes;
	json_out_file.close();
}