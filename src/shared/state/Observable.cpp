#include "define.hpp"
#include <string>
#include <iostream>
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
//	cout << "entering notifyObserver function\n";
	observers[event_name]->setEventID(ID, true);
//	cout << "leaving notifyObserver function\n";

}

int Observable::observers_size() { return observers.size(); }