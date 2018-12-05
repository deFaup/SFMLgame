// includes
#include "define.hpp"
#include "EventCharacters.h"
#include "EventPlayer.h"

using namespace std;
using namespace state;

// set all atributes to false (parent's one included)
EventCharacters::EventCharacters() : changed_character(NULL)
{
	events[Character_positionChanged] = false;
	events[Character_statisticChanged] = false;
	events[SelectedCharacterChanged] = false;
}

EventCharacters::EventCharacters(Characters* changed_character, EventID ID): changed_character(changed_character)
{
	events[ID] = true;
	//	events[xxx] = false;
}

EventCharacters::~EventCharacters() {};

EventPlayer::EventPlayer(Player* changed_player, EventID ID) : changed_player(changed_player)
{
	events[ID] = true;
}