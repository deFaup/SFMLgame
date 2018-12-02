/* Includes */
#include <iostream>
#include "define.hpp"
#include "sfEvents.h"

using namespace std;
using namespace render;

sfEvents::sfEvents(sfEventsID ID) : ID(ID) {}
sfEvents::sfEvents(sfEventsID ID, state::Position mouse_position) : ID(ID) , mouse_position(mouse_position) {}
