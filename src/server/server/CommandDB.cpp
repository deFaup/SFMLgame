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
	std::cout << "in CommandDB: " << JSONfile << std::endl;
	return JSONfile;
}

void CommandDB::addCommand(Json::Value& commande)
{
	static int i = 0;
	JSONfile["commandes"][i] = commande;
	i++;
	return;
}

void CommandDB::deleteCommand(Json::Value& commande)
{
	return;
}

