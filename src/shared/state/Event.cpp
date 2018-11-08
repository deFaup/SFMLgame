
#include "define.hpp"
#include <iostream>
#include <string>
#include "Event.h"

using namespace std;
using namespace state;

//element() initialize the variable element to an empty value - behing it's a map element
Event::Event() : events() {}
Event::~Event() {};

void Event::setEventID(EventID ID, bool true_false) { events[ID] = true_false; }

const bool Event::getEventID(EventID ID) { return events[ID]; }

bool Event::hasChanged(EventID ID)
{
	return events[ID];
}