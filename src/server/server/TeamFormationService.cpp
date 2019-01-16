#include "TeamFormationService.h"
#include <iostream>

using namespace server;
using namespace std;

TeamFormationService::TeamFormationService()
{
	server::AbstractService::pattern = "version";
}

TeamFormationService::~TeamFormationService()
{
}

HttpStatus TeamFormationService::get (Json::Value& out) const
{
	std::cout << "test " << out << "\n";

	out["major"] = 1;
	out["minor"] = 0;
	std::cout << "test "<< out << "\n";
	return HttpStatus::OK;
}

HttpStatus TeamFormationService::post (Json::Value& in)
{

	return HttpStatus::OK;
}
