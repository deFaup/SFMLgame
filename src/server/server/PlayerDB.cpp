#include "PlayerDB.h"
#include <iostream>

using namespace server;
//using namespace std;

// in PlayerDB we store in the JSON all the information related to the composition of a team
// players: name, number, number of charater, characters

/* the JSON look likes this */
//{
//	"first player name" : 0,
//	"second player name" : 1,
//	"third" : 2,
//	"team" :
//		[
//			{
//				"characters" :
//				[
//					103,
//					100,
//					301
//				],
//				"name" : "first player name"
//			},
//			{ and so on}
//
//		]
//}


PlayerDB::PlayerDB() {}

void PlayerDB::addPlayer(const Json::Value& player)
{
	/* player is json like this one*/
	//{
	//	"name" : "Joueur 2"
	//}

	JSONfile["players"] [player["name"].asString()] = JSONfile["players"].size();
	JSONfile["team"] [JSONfile["team"].size()]["name"] = player["name"].asString();
}

void PlayerDB::addCharacter(const Json::Value& character) 
{
	/* player is json like this one*/
	//{
	//	"name" : "Joueur 2"
	//	"character" : "103"
	//}

	int index_player = JSONfile["players"] [character["name"].asString()].asInt();
	JSONfile["team"][index_player]["characters"][JSONfile["team"][index_player]["characters"].size()]
		= character["character"].asInt();
};

// to be completed
void PlayerDB::deletePlayer(Json::Value& player) 
{
	Json::Value temp;
	for (unsigned int index(0), index_temp(0); index < JSONfile["team"].size(); ++index, ++index_temp)
	{
		if (JSONfile["team"][index]["name"] == (player["name"].asString()))
			index_temp--; 
		else
			temp["team"][index] = JSONfile["team"][index];
	}
	JSONfile = temp;
};

void PlayerDB::deleteCharacter(Json::Value& character) {};
void PlayerDB::getPlayer(Json::Value& player) {};
//void PlayerDB::getCharacter();
