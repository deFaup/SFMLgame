#include "TeamFormationService.h"
#include "define.hpp"
#include <iostream>
#include <thread>


using namespace server;
using namespace std;

// local functions

bool start(false);

TeamFormationService::TeamFormationService()
{
	players = std::make_unique<PlayerDB>();
	server::AbstractService::pattern = "/TeamFormationService";
}

TeamFormationService::~TeamFormationService(){}

// get Team Formation
HttpStatus TeamFormationService::get (const string& url, Json::Value& out)
{
	if (url.find("/start", pattern.size()) == pattern.size())
	{
		if (start)
			out["start"] = 1;
	}
	else out = players->JSONfile;

	return HttpStatus::OK;
}

HttpStatus TeamFormationService::post (const string& url, const Json::Value& in, Json::Value& out)
{
	if (url[pattern.size()] == '/')
	{
		std::string set_player("/set_player/");
		std::string set_character("/set_character/");

		if (url.find(set_player, pattern.size()) == pattern.size() && !start)
		{
			std::string id(url, pattern.size() + set_player.size());
			Json::Value conv_to_int; conv_to_int[set_player] = id;
			unsigned int int_id = conv_to_int[set_player].asUInt();

			if (int_id < MAX_NB_PLAYER)
			{
				players->nb_players = int_id;
				out = conv_to_int;
			}
			else
				out = "MAX_NB_PLAYER= " + to_string(MAX_NB_PLAYER);
		}
		else if (url.find(set_character, pattern.size()) == pattern.size() && !start)
		{
			std::string id(url, pattern.size() + set_character.size());
			Json::Value conv_to_int; conv_to_int[set_character] = id;
			unsigned int int_id = conv_to_int[set_character].asUInt();

			if (int_id < MAX_NB_CHARACTER)
			{
				players->nb_characters_by_player = int_id;
				out = conv_to_int;
			}
			else
				out = "MAX_NB_CHARACTER= " + to_string(MAX_NB_CHARACTER);
		}

		//url = "/TeamFormationService/player";
		else if (url.find("/player", pattern.size()) == pattern.size())
		{
			if (players->addPlayer(in, out) == 0)
			{
				service_gameStarted->commandes[out["id"].asString()] = make_shared<CommandDB>();
				service_gameStarted->players_id.push_back(in["name"].asString());
			}
			else
				return HttpStatus::BAD_REQUEST;
		}	
		//url = "/TeamFormationService/character"
		else if (url.find("/character", pattern.size()) == pattern.size())
		{
			if (players->addCharacter(in) == 0)
				try_to_start();
		}

		//url = "/TeamFormationService/delete_player";
		else if (url.find("/delete_player", pattern.size()) == pattern.size())
		{
			if (players->deletePlayer(in) == 0)
				service_gameStarted->commandes.erase(out["id"].asString());
		}
		
	}

	return HttpStatus::OK;
}

HttpStatus TeamFormationService::put(Json::Value& out, const Json::Value& in)
{
	return HttpStatus::OK;
}

int TeamFormationService::try_to_start()
{
	std::cout << "TRY TO START GAME" << std::endl;
	unsigned int count(0);

	if (players->JSONfile["players"].size() == players->nb_players)
	{
		for (auto& elem : players->JSONfile["team"])
		{
			if (elem["characters"].size() == players->nb_characters_by_player)
			{
				count++; std::cout << elem["name"].asString() << "is OK\n";
			}
		}
	}

	if (count == players->nb_players)
	{
		start = true;
		//std::thread(&server::GameServer::launch_game, gameServer, &(players->JSONfile));
		gameServer->launch_game(&(players->JSONfile));
	}
	return start;
}