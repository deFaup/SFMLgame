/* Includes */
#include <iostream>
#include "define.hpp"
#include "sfEvents.h"

state::sfEvents::sfEvents() : ID(state::sfEventsID::right_click), mouse_position(Position()) {}
state::sfEvents::sfEvents(state::sfEventsID ID) : ID(ID) {}
state::sfEvents::sfEvents(state::sfEventsID ID, state::Position mouse_position) : ID(ID) , mouse_position(mouse_position) {}
