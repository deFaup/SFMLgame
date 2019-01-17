#include "TeamFormationService.h"
#include <iostream>

using namespace server;
using namespace std;

int nb_players(2), nb_characters_by_player(2);
// local functions


TeamFormationService::TeamFormationService()
{
	players = std::make_unique<PlayerDB>();
	server::AbstractService::pattern = "/TeamFormationService";
}

TeamFormationService::~TeamFormationService(){}

// get Team Formation
HttpStatus TeamFormationService::get (const string& url, Json::Value& out)
{
	out = players->JSONfile;
	return HttpStatus::OK;
}

HttpStatus TeamFormationService::post (const string& url, const Json::Value& in, Json::Value& out)
{
	if (url[pattern.size()] == '/')
	{
		std::string set_player("/set_player/");
		std::string set_character("/set_character/");

		if (url.find(set_player, pattern.size()) == pattern.size())
		{
			std::string id(url, pattern.size() + set_player.size());
			Json::Value conv_to_int; conv_to_int[set_player] = id;
			nb_players = conv_to_int[set_player].asInt();
			out = conv_to_int;
		}

		else if (url.find(set_character, pattern.size()) == pattern.size())
		{
			std::string id(url, pattern.size() + set_character.size());
			Json::Value conv_to_int; conv_to_int[set_character] = id;
			nb_characters_by_player = conv_to_int[set_character].asInt();
			out = conv_to_int;
		}

		//url = "/TeamFormationService/player";
		else if (url.find("/player", pattern.size()) == pattern.size())
		{
			if (players->addPlayer(in, out) == 0)
			{
				service_gameStarted->commandes[out["id"].asString()] = make_shared<CommandDB>();
				try_to_start();
			}
			else
				return HttpStatus::BAD_REQUEST;
		}
		
		//url = "/TeamFormationService/character"
		else if (url.find("/character", pattern.size()) == pattern.size())
			players->addCharacter(in);

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

void TeamFormationService::try_to_start()
{
	bool start(true);
	if (players->JSONfile["players"].size() == nb_players)
	{
		for (auto& elem : players->JSONfile["team"])
		{
			if (elem["characters"].size() != nb_characters_by_player)
				start = false;
		}
	}
	else start = false;
	if (start)
		server::AbstractService::gameServer->launch_game(players->JSONfile);
}