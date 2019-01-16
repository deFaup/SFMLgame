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
HttpStatus TeamFormationService::get (const string& url, Json::Value& out) const
{
	out = players->JSONfile;
	return HttpStatus::OK;
}

// Insert new player in PlayerDB
HttpStatus TeamFormationService::post (const string& url, const Json::Value& in)
{
	if (url[pattern.size()] == '/')
	{
		//url = "/TeamFormationService/player";
		if (url.find("/player", pattern.size()) == pattern.size())
			players->addPlayer(in);
		
		//url = "/TeamFormationService/character"
		if (url.find("/character", pattern.size()) == pattern.size())
			players->addCharacter(in);
	}

	return HttpStatus::OK;
}
