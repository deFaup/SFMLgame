#include "GameStartedService.h"

using namespace server;
using namespace std;

GameStartedService::GameStartedService()
{
	commandes = std::make_unique<CommandDB>();
	server::AbstractService::pattern = "/version";
}

GameStartedService::~GameStartedService()
{
}

HttpStatus GameStartedService::get (const string& url, Json::Value& out) const
{
	out["welcome to the game"] = "not started";
	return HttpStatus::OK;
}

HttpStatus GameStartedService::post (const string& url, const Json::Value& in)
{
	return HttpStatus::OK;
}
