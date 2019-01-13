/* Includes */
#include <iostream>
#include "define.hpp"
#include "sfEvents.h"

state::sfEvents::sfEvents(state::sfEventsID ID) : ID(ID) {}
state::sfEvents::sfEvents(state::sfEventsID ID, state::Position mouse_position) : ID(ID) , mouse_position(mouse_position) {}
