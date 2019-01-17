#include "CommandDB.h"

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
CommandDB::CommandDB()
{
}

Json::Value& CommandDB::getCommand()
{

}

void CommandDB::addCommand(Json::Value& commande)
{

}

void CommandDB::deleteCommand(Json::Value& commande)
{

}
