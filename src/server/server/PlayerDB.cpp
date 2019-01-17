#include "PlayerDB.h"
#include "define.hpp"
#include <iostream>
#include <cstdlib>

using namespace server;
//using namespace std;

// in PlayerDB we store in the JSON all the information related to the composition of a team
// players: name, number, number of charater, characters

/* the JSON look likes this */
	//{
	//	"players" : {
	//		"Joueur 1" : 0,
	//			"Joueur 2" : 1
	//	},
	//		"team" : [
	//		{
	//			"characters" : [100, 100, 100],
	//				"name" : "Joueur 1"
	//		},
	//		{
	//			"characters" : [200, 200, 200],
	//			"name" : "Joueur 2"
	//		}
	//		]
	//}

PlayerDB::PlayerDB() {}

// return -1 if player is already in, -2 if there are too many players and 0 if OK
// return an id in HTML body when 0
int PlayerDB::addPlayer(const Json::Value& player, Json::Value& out)
{
	/* player is json like this one*/
	//{
	//	"name" : "Joueur 2"
	//}

	if (JSONfile["players"].size() < MAX_NB_PLAYER)
	{
		for (unsigned int i = 0; i < JSONfile["players"].size(); i++)
		{
			if (JSONfile["players"][i].asString() == player["name"].asString())
				return -1;
		}
		JSONfile["players"][JSONfile["players"].size()] = player["name"].asString();
		JSONfile["team"][JSONfile["team"].size()]["name"] = player["name"].asString();

		static unsigned short int i(0);
		out["id"] = "player"+ std::to_string(i++);
		return 0;
	}
	else return -2;
}

void PlayerDB::addCharacter(const Json::Value& character) 
{
	/* player is json like this one*/
	//{
	//	"name" : "Joueur 2"
	//	"character" : "103"
	//}

	unsigned int index_player;
	for (index_player = 0; index_player < JSONfile["players"].size(); index_player++)
	{
		if (JSONfile["players"][index_player].asString() == character["name"].asString())
			break;
	}
	JSONfile["team"][index_player]["characters"][JSONfile["team"][index_player]["characters"].size()]
		= character["character"].asInt();
};

int PlayerDB::deletePlayer(const Json::Value& player) 
{
	int erno(-1);

	Json::Value temp;
	unsigned int index_player(0), i(0);
	for (i = 0; i < JSONfile["players"].size(); i++)
	{
		if (JSONfile["players"][i].asString() == player["name"].asString())
		{
			index_player = i;
			erno = 1;
			continue;
		}
		temp["players"][i] = JSONfile["players"][i].asString();
	}

	for (i=0; i < JSONfile["team"].size(); i++)
	{
		if (i == index_player) {
			erno--;
			continue;
		}
		temp["team"][i] = JSONfile["team"][i];
	}
	JSONfile = temp;

	return erno;
};

void PlayerDB::deleteCharacter(const Json::Value& character) {};
void PlayerDB::getPlayer(Json::Value& player) {};
//void PlayerDB::getCharacter();
