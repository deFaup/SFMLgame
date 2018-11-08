// includes
#include "define.hpp"
#include "EventCharacters.h"

using namespace std;
using namespace state;

// set all atributes to false (parent's one included)
EventCharacters::EventCharacters()
{
	events[Character_positionChanged] = false;
	events[Character_statisticChanged] = false;
	events[SelectedCharacterChanged] = false;
}
