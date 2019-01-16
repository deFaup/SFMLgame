#include "PlayerDB.h"

using namespace server;
//using namespace std;

// in PlayerDB we store in the JSON all the information related to the composition of a team
// players: name, number, number of charater, characters

/* the JSON look likes this */
//[
//	{
//		"characters" :
//		[
//			103,
//			100,
//			301
//		],
//		"name" : "Joueur 1"
//	},
// each block between {} is for one player
//]


PlayerDB::PlayerDB() {}

void PlayerDB::addPlayer(Json::Value& player)
{
	/* player is json like this one*/
	//{
	//	"name" : "Joueur 2"
	//}

	JSONfile[JSONfile.size()]["name"] = player["name"].asString();
}

void PlayerDB::addCharacter(int index_player, Json::Value& character) 
{
	/* player is json like this one*/
	//{
	//	"character" : "103"
	//}

	JSONfile[index_player]["characters"][JSONfile[index_player]["characters"].size()] 
		= character["character"].asInt();
};

void PlayerDB::deletePlayer(Json::Value& player) 
{
	Json::Value temp;
	for (unsigned int index(0), index_temp(0); index < JSONfile.size(); ++index, ++index_temp)
	{
		if (JSONfile[index]["name"] == (player["name"].asString()))
			index_temp--; 
		else
			temp[index] = JSONfile[index];
	}
	JSONfile = temp;
};

void PlayerDB::deleteCharacter(Json::Value& character) {};
void PlayerDB::getPlayer(Json::Value& player) {};
//void PlayerDB::getCharacter();
