#include "define.hpp"
#include <string>
#include "Observable.h"

using namespace std;
using namespace state;

// init static member
std::map<std::string, shared_ptr<Event>> Observable::observers;

void Observable::registerObserver(string event_name, shared_ptr<Event> observer)
{
	observers[event_name] = observer;
}

void Observable::notifyObserver(string event_name, EventID ID)
{
	observers[event_name]->setEventID(ID, true);
}
