#include "define.hpp"
#include "Move.h"
#include <iostream>
#include <vector>

using namespace engine;
using namespace std;
using namespace state;
using namespace render;

// speed at xich we move sprtite/characters
unsigned int speed = 8;

// move method used when the game has started
Move::Move(render::sfEventsID dir)
{
	switch (dir)
	{
	case arrow_left:
		move_type = MoveLeft;
		break;
	case arrow_right:
		move_type = MoveRight;
		break;
	case arrow_up:
		move_type = MoveUp;
		break;
	case arrow_down:
		move_type = MoveDown;
		break;
	default:
		move_type = MoveNone;
		break;
	}
}

int Move::isLegit(state::GameState& etat)
{
	if (move_type == MoveNone)
		return -1;

	int right_limit(0), down_limit(0);
	etat.map.get_dimensions(right_limit, down_limit);

	state::Position pos = etat.current_player->get_current_character()->position;
	if (pos.getPositionX() < speed && move_type == MoveLeft)
		return -1;
	if (pos.getPositionX() > (right_limit - speed) && move_type == MoveRight)
		return -1;
	if (pos.getPositionY() < speed && move_type == MoveUp)
		return -1;
	if (pos.getPositionY() > (down_limit - speed) && move_type == MoveDown)
		return -1;
	Statistics stats = etat.current_player->get_current_character()->stats;
	if (stats.get_move_point() == 0)
		return -1;

	return(0);
}

void Move::execute(state::GameState& etat)
{
	state::Position& pos = etat.current_player->get_current_character()->position;
	std::vector<std::vector<int>> mask = etat.map.get_mask();
	Statistics& stats = etat.current_player->get_current_character()->stats;
	Statistics statsp(stats.get_life_point(),stats.get_attack_point(),stats.get_move_point()-1);

	switch (move_type)
	{
	case MoveLeft:
		if(mask[pos.getPositionY()+270][pos.getPositionX()-speed] == 0){
			pos.increaseX(-speed);
			stats.set_statistics(statsp);
		}
		else{
			unsigned int i = 0;
			while(mask[pos.getPositionY()+270-i][pos.getPositionX()-speed] != 0){
				i++;
			}
			pos.increaseX(-speed);
			pos.increaseY(-i);
			stats.set_statistics(statsp);
		}
		break;
	case MoveRight:
		if(mask[pos.getPositionY()+270][pos.getPositionX()+speed] == 0){
			pos.increaseX(speed);
			stats.set_statistics(statsp);
		}
		else{
			unsigned int i = 0;
			while(mask[pos.getPositionY()+270-i][pos.getPositionX()-speed] != 0){
				i++;
			}
			pos.increaseX(speed);
			pos.increaseY(-i);
			stats.set_statistics(statsp);
		}
		break;
	case MoveUp:
		pos.increaseY(-speed);
		break;
	case MoveDown:
		if(mask[pos.getPositionY()+270][pos.getPositionX()] == 0){
			pos.increaseY(speed);
		}
		break;
	default:
		break;
	}
	// call to observers is automatic
	// state::EventCharacters event(Character_positionChanged);
	// etat.current_player->get_current_character()->notifyObservers(event);

	//std::cout << "executing movement" << endl;
}

void Move::move_with_mouse(state::GameState& etat,state::Position pos)
{
	etat.current_player->get_current_character()->position.setPosition(pos.getPositionX(), pos.getPositionY());
}
