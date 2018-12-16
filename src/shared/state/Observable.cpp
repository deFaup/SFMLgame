#include "define.hpp"
#include <string>
#include <iostream>
#include "Observable.h"

using namespace std;
using namespace state;

void Observable::registerObserver(shared_ptr<Observer> observer)
{
	observers.push_back(observer);
}

void Observable::notifyObservers(Event& event)
{
	for (auto obs : observers)
	{
		//cout << "entering notifyObserver function\n";
		obs->stateChanged(event);
		//cout << "leaving notifyObserver function\n";
	}
}

int Observable::observers_size() { return observers.size(); }
