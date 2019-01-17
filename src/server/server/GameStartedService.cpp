#include "GameStartedService.h"
#include "GameServer.h"
#include "state.h"
#include "engine.h"

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
			state::sfEvents com;

			com.ID = static_cast<state::sfEventsID>(in["sfEvents"].asInt());
			com.mouse_position.setPosition(in["x"].asInt(),in["y"].asInt());

			if ((com.ID == state::arrow_left) || (com.ID == state::arrow_right))
			{
				engine::Move move_command(com.ID);
				if (move_command.isLegit(*(server->etat)) == -1) {
					return HttpStatus::OK;
				}
			}

			else if (server->etat->ID != state::started || com.ID == state::num1 || com.ID == state::num2 || com.ID == state::num3 || com.ID == state::num4 || com.ID == state::num5)
			{
				return HttpStatus::OK;
			}

			else if (com.ID == state::num1 || com.ID == state::num2 || com.ID == state::num3 || com.ID == state::num4 || com.ID == state::num5)
			{
				engine::Attack attack_command;
				attack_command.attack_position = com.mouse_position;
				if (com.ID == state::num1)
					attack_command.attack_number = 0;
				else if (com.ID == state::num2)
					attack_command.attack_number = 1;
				else if (com.ID == state::num3)
					attack_command.attack_number = 2;
				else if (com.ID == state::num4)
					attack_command.attack_number = 3;
				else if (com.ID == state::num5)
					attack_command.attack_number = 4;
				if (attack_command.isLegit(*(server->etat)) == -1)
				{
					return HttpStatus::OK;
				}
			}

			server->moteur->add_command(com);
			server->moteur->executeCommandes();

			Json::Value json = in;
			commandes->addCommand(json);	
		}
	}
	return HttpStatus::OK;
}
