/* Includes */
#include <iostream>
#include "define.hpp"
#include "Server.h"

#include "state.h"
#include "engine.h"
#include "ai.h"

using namespace std;
using namespace state;
using namespace server;
Server::Server() {}
Server::~Server(){ std::cout << "Server deleted" << std::endl; }

/* Server constructor share its reference to the GameState */
Server::Server(state::GameState* gameState) : gameState(gameState)
{
	std::cout << "Server created" << std::endl;
}

void Server::stateChanged(state::Event& event)
{	
	if (event.hasChanged(state::EventID::Character_isDead))
	{
		EventCharacters* event_character = (EventCharacters*) &event;
		Player* player = event_character->changed_character->get_Player();

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
		gameState->delete_character(event_character->changed_character);
		//std::cout << "references to this character= " << temp.use_count() << endl; //4

		//std::cout << "Server::stateChanged character: END\n";

	}//2 once we exit this namespace and 0 when we leave engine::Attack::execute (or only 1 in Attack)

	else if (event.hasChanged(state::EventID::Player_isDead))
	{
		EventPlayer* event_player = (EventPlayer*) &event;
		Player* player = event_player->changed_player;
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
		gameState->delete_player(player);
		//std::cout << "Server::stateChanged Player: END\n";
	}
}
