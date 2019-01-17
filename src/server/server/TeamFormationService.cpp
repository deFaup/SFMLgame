#include "TeamFormationService.h"
#include <iostream>

using namespace server;
using namespace std;

TeamFormationService::TeamFormationService()
{
	players = std::make_unique<PlayerDB>();
	server::AbstractService::pattern = "/TeamFormationService";
}

TeamFormationService::~TeamFormationService()
{
}

// get Team Formation
HttpStatus TeamFormationService::get (const string& url, Json::Value& out)
{
	out = players->JSONfile;
	return HttpStatus::OK;
}

// Insert new player in PlayerDB
HttpStatus TeamFormationService::post (const string& url, const Json::Value& in, Json::Value& out)
{
	if (url[pattern.size()] == '/')
	{
		//url = "/TeamFormationService/player";
		if (url.find("/player", pattern.size()) == pattern.size())
		{
			if (players->addPlayer(in, out) != 0)
				return HttpStatus::BAD_REQUEST;

			else
				service_gameStarted->commandes[out["id"].asString()] = make_shared<CommandDB>();
				//service_gameStarted->commandes.push_back(make_shared<CommandDB>());
		}
		
		//url = "/TeamFormationService/character"
		if (url.find("/character", pattern.size()) == pattern.size())
			players->addCharacter(in);

		//url = "/TeamFormationService/delete_player";
		if (url.find("/delete_player", pattern.size()) == pattern.size())
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