
#include "define.hpp"
#include <iostream>
#include <string>
#include "Event.h"

using namespace std;
using namespace state;

//element() initialize the variable element to an empty value - behing it's a map element
Event::Event() : events() {}
Event::~Event() {};

bool Event::hasChanged(EventID ID)
{
	return this->events[ID];
}