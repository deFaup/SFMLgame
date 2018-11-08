
#include "define.hpp"
#include <iostream>
#include <string>
#include "Event.h"

using namespace std;
using namespace state;

Event::Event() : changed(false) {}

void Event::setEventID(EventID ID, bool true_false) { events[ID] = true_false; }

const bool Event::getEventID(EventID ID) { return events[ID]; }

bool Event::hasChanged(EventID ID)
{
	return events[ID];
}