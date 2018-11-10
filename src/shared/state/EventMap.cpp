// Includes
#include "define.hpp"
#include "EventMap.h"
#include <iostream>

using namespace state;
using namespace std;

// set all atributes to false (parent's one included)
EventMap::EventMap()
{
	events[Map_maskChanged] = false;
}

EventMap::~EventMap() {};

EventMap::EventMap(EventID ID)
{
	events[ID] = true;
//	events[xxx] = false;
}