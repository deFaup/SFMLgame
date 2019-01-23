#include "CommandDB.h"
#include <iostream>

using namespace server;
using namespace std;
/*
forme du JSON :

{
	"commandes" :
	[
		{
			"sfEventsID" : xxx,
			"x" : xxx,
			"y" : xxx
		},
		{
			Et ainsi de suite ...
		}
	]
}

*/

CommandDB::CommandDB() {}

Json::Value& CommandDB::getCommand()
{
	return JSONfile;
}

void CommandDB::addCommand(Json::Value& commande)
{
	std::cout << "in addCommand, cmd size: " << JSONfile["commandes"].size() << std::endl;
	JSONfile["commandes"][JSONfile["commandes"].size()] = commande;
	std::cout << "in addCommand, new cmd size: " << JSONfile["commandes"].size() << std::endl;

	return;
}

void CommandDB::deleteCommand(Json::Value& commande)
{
	return;
}

