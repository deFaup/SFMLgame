#include "define.hpp"
#include "Character.h"

using namespace render;

Character::Character(const state::GameState& state){
	this->state = state;
	return;
}

