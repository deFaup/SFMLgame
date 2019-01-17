#include "GameStartedService.h"

using namespace server;
using namespace std;

GameStartedService::GameStartedService()
{
	commandes = std::make_unique<CommandDB>();
	server::AbstractService::pattern = "/GameStartedService";
}

GameStartedService::~GameStartedService()
{
}

HttpStatus GameStartedService::get (const string& url, Json::Value& out) const
{
	//out["welcome to the game"] = "not started";
	if (url[pattern.size()] == '/')
	{
		if(url.find("/get_command",pattern.size()) == pattern.size())
		{
			out = commandes->getCommand();	
		}
	}
	return HttpStatus::OK;
}

HttpStatus GameStartedService::post (const string& url, const Json::Value& in)
{
	if (url[pattern.size()] == '/')
	{
		if(url.find("/add_command",pattern.size()) == pattern.size())
		{
			Json::Value json = in;
			commandes->addCommand(json);	
		}
	}
	return HttpStatus::OK;
}
