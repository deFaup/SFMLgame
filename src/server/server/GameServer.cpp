/* Includes */
#include "define.hpp"
#include "global_mutex.hpp"
#include "GameServer.h"

#include "state.h"
#include "server.h"
#include "engine.h"

#include <memory>
#include <iostream>
#include <string>	//string
#include <thread>
#include <json/json.h>

using namespace server;
void set_map_players_characters(state::GameState* gameState, const Json::Value& players);

GameServer::GameServer() {}
GameServer::~GameServer(){ std::cout << "Server deleted" << std::endl; }

/* Server constructor share its reference to the GameState */
GameServer::GameServer(state::GameState* etat, engine::GameEngine* moteur) : etat(etat),moteur(moteur)
{
	std::cout << "Server created" << std::endl;
}

void GameServer::launch_game(Json::Value* players)
{
	std::cout << "launch_game" << std::endl;
	set_map_players_characters(etat, *players);
}

void set_map_players_characters(state::GameState* gameState, const Json::Value& players)
{
	global::rng.seed(std::random_device()());

	/* Create players, characters and a map. Will be rewritten when menu is implemented */
	if (gameState->ID == state::StateID::not_started)
	{
		gameState->new_map(3000, 2000);

		int player_no(0);
		for (auto& elem : players["team"])
		{
			gameState->new_player(elem["name"].asString());
			for (auto& characters : elem["characters"])
			{
				gameState->new_character(player_no,
					static_cast<state::CharactersID>(characters.asInt()));
			}
			player_no++;
		}
		gameState->ID = state::StateID::team_selected;
	}
}

void GameServer::stateChanged(state::Event& event)
{	
	if (event.hasChanged(state::EventID::Character_isDead))
	{
		state::EventCharacters* event_character = (state::EventCharacters*) &event;
		state::Player* player = event_character->changed_character->get_Player();

		//std::cout << "Server::stateChanged Player: " << player->name << " character: ";
		//std::cout << event_character->changed_character->get_id() << " is dead !" << std::endl;

		unsigned int index(0);
		/* find the shared ptr to the dead character in the player characters vector */
		for (unsigned int i = 0; i < player->get_number_of_characters(); i++)
		{
			if ((*player).get_character(i).get() == event_character->changed_character)
			{
				index = i;
				break;
			}
		}
		// in this part we delete all shared ptr to the dead character
		//shared_ptr<state::Characters> temp = player->get_character(index);
		//std::cout << "references to this character= " << temp.use_count() << endl; //7 or 6 (5 if 1 in Attack)
		
		// delete in Player (current_character and vector characters)
		player->delete_character(index);
		//std::cout << "references to this character= " << temp.use_count() << endl; //5

		// delete in GameState (vector characters)
		etat->delete_character(event_character->changed_character);
		//std::cout << "references to this character= " << temp.use_count() << endl; //4

		//std::cout << "Server::stateChanged character: END\n";

	}//2 once we exit this namespace and 0 when we leave engine::Attack::execute (or only 1 in Attack)

	else if (event.hasChanged(state::EventID::Player_isDead))
	{
		state::EventPlayer* event_player = (state::EventPlayer*) &event;
		state::Player* player = event_player->changed_player;
		//std::cout << "Server::stateChanged  Player: " << player->name << " is dead !" << std::endl;

		//unsigned int index(0);
		//find the shared ptr to the dead character in the player characters vector
		//shared_ptr<state::Player> temp;
		//for (unsigned int i = 0; i < gameState.get_number_of_player(); i++)
		//{
		//	if (gameState.get_player(i).get() == player)
		//	{
		//		index = i;
		//		temp = gameState.get_player(index);
		//		break;
		//	}
		//}
		//std::cout << "references to this character= " << temp.use_count() << endl; //2 or 3

		// in this part we delete all shared ptr to the dead player
		etat->delete_player(player);
		//std::cout << "Server::stateChanged Player: END\n";
	}
}
