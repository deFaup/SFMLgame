// Includes
#include "define.hpp"
#include "EventMap.h"
#include <iostream>

using namespace state;
using namespace std;

// set all atributes to false (parent's one included)
EventMap::EventMap() : Event()
{
	events[Map_maskChanged] = false;
}

/*
void EventMap::setEventID(EventID ID, bool true_false) 
{
	if (ID == Map_maskChanged)
		events[ID] = true_false;
}*/


