#include "define.hpp"
#include "Move.h"
#include <iostream>
#include <vector>

using namespace engine;
using namespace std;
using namespace state;
using namespace render;

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
	if (pos.getPositionX() < SPEED && move_type == MoveLeft)
		return -1;
	if (pos.getPositionX() > ((unsigned)right_limit - SPEED) && move_type == MoveRight)
		return -1;
	if (pos.getPositionY() < SPEED && move_type == MoveUp)
		return -1;
	if (pos.getPositionY() > ((unsigned)down_limit - SPEED) && move_type == MoveDown)
		return -1;
	Statistics stats = etat.current_player->get_current_character()->stats;
	if (stats.get_move_point() == 0)
		return -1;

	return(0);
}

void Move::execute(state::GameState& etat)
{
	state::Position& pos = etat.current_player->get_current_character()->position;
	//std::cout << "current player name: " << etat.current_player->name << "\n";
	//std::cout << "ptr: " << etat.current_player.get() << "\n";

	std::vector<std::vector<int>> mask = etat.map.get_mask();
	Statistics& stats = etat.current_player->get_current_character()->stats;
	
	switch (move_type)
	{
	case MoveLeft:
		if(mask[pos.getPositionY()][pos.getPositionX()-SPEED] == 0){
			pos.increaseX(-SPEED);
			stats.increase_move_point(-1);
		}
		else{
			unsigned int i = 0;
			while(mask[pos.getPositionY()-i][pos.getPositionX()-SPEED] != 0){
				i++;
			}
			pos.increaseX(-SPEED);
			pos.increaseY(-i);
			stats.increase_move_point(-1);
		}
		break;
	case MoveRight:
		if(mask[pos.getPositionY()][pos.getPositionX()+SPEED] == 0){
			// 
			pos.increaseX(SPEED);
			stats.increase_move_point(-1);
		}
		else{
			unsigned int i = 0;
			while(mask[pos.getPositionY()-i][pos.getPositionX()+SPEED] != 0){
				i++;
			}
			pos.increaseX(SPEED);
			pos.increaseY(-i);
			stats.increase_move_point(-1);
		}
		break;
	/*case MoveUp:
		pos.increaseY(-SPEED);
		break;*/
	case MoveDown:
		if(mask[pos.getPositionY()][pos.getPositionX()] == 0){
			pos.increaseY(SPEED);
		}
		break;
	default:
		break;
	}
	//std::cout << "current player char stats: LP/AP/MP: " << stats.get_life_point();
	//cout << " / " << stats.get_attack_point();
	//cout << " / " << stats.get_move_point() << "\n";
}

void Move::move_with_mouse(state::GameState& etat,state::Position pos)
{
	etat.current_player->get_current_character()->position.setPosition(pos.getPositionX(), pos.getPositionY());
}
