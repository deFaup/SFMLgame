#include "GameStartedService.h"
#include "GameServer.h"
#include "state.h"
#include "engine.h"
#include <iostream>
#include <iterator>

using namespace server;
using namespace std;

GameStartedService::GameStartedService()
{
	server::AbstractService::pattern = "/GameStartedService";
}

GameStartedService::~GameStartedService()
{
}

HttpStatus GameStartedService::get (const string& url, Json::Value& out)
{
	if (url[pattern.size()] == '/')
	{
		std::string get_command("/get_command/");
		if(url.find(get_command, pattern.size()) == pattern.size())
		{
			std::string id(url, pattern.size() + get_command.size());

			for (auto& players : players_id) //check if the players exist-- need a function for that as it should be done everywhere
			{
				if (players == id)
					out = commandes[id]->getCommand();
			}
		}
	}
	return HttpStatus::OK;
}

HttpStatus GameStartedService::post (const string& url, const Json::Value& in, Json::Value& out)
{
	if (url[pattern.size()] == '/')
	{
		std::string add_command("/add_command/");
		if (url.find(add_command, pattern.size()) == pattern.size())
		{
			state::sfEvents com;
			com.ID = static_cast<state::sfEventsID>(in["sfEventsID"].asInt());
			com.mouse_position.setPosition(in["x"].asInt(), in["y"].asInt());

			if ((com.ID == state::arrow_left) || (com.ID == state::arrow_right))
			{
				//cout << "4" << endl;
				engine::Move move_command(com.ID);
				//cout << "5" << endl;
				if (move_command.isLegit(*(gameServer->etat)) == -1) {
					return HttpStatus::OK;
				}
			}
			else if (gameServer->etat->ID != state::started || com.ID == state::num1 || com.ID == state::num2 || com.ID == state::num3 || com.ID == state::num4 || com.ID == state::num5)
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
				if (attack_command.isLegit(*(gameServer->etat)) == -1)
				{
					return HttpStatus::OK;
				}
			}

			gameServer->moteur->add_command(com);
			//server->moteur->executeCommandes(); // fait dans un thread !

			//std::cout << "server processing " << in["id"].asString() << std::endl;
			for (auto& players: players_id)
			{
				//std::cout << players << std::endl;
				if (players != in["id"].asString())
				{
					//std::cout << "ennemy: " << players << std::endl;
					//std::cout << "add cmd: " << in << std::endl;
					Json::Value json_cmd = in;
					//std::cout << "json cmd: " << json_cmd << std::endl;
					commandes[players]->addCommand(json_cmd);
					//std::cout << "new json cmd: " << commandes[players]->JSONfile << std::endl;
				}
			}
			//cout << "8" << endl;

			/*for (std::map<char,int>::const_iterator it=commandes.begin(); it == commandes.end(); ++it)
			{
			//std::cout << it->first << " => " << it->second << '\n';
			if(it->first != id)
			{
			Json::Value json = in;
			(it->second)->addCommand(json);
			}
			}*/
			return HttpStatus::OK;
		}

		else if (url.find("/stop", pattern.size()) == pattern.size())
		{
			gameServer->moteur->game_ended = true;
			gameServer->etat->ID = state::StateID::end;
		}

		else return HttpStatus::BAD_REQUEST;
	}
	else return HttpStatus::NOT_IMPLEMENTED;
	
	return HttpStatus::OK;
}
