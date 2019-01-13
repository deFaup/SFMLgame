#ifndef UNIT_TEST
#define UNIT_TEST

#include <iostream>
#include <string>
#include "define.hpp"
#include "state.h"

using namespace state;

int result_statistics();
bool test_get_life_point(Statistics m_stat, unsigned int life);
bool test_get_attack_point(Statistics m_stat, unsigned int attack);
bool test_get_move_point(Statistics m_stat, unsigned int move);
bool test_set_statistics(Statistics given_stats, unsigned life, unsigned attack, unsigned move);
bool test_reset_all_but_life(Statistics given_stats, CharactersID id);

int result_position();
bool test_getPositionX(Position pos, const unsigned int x);
bool test_getPositionY(Position pos, const unsigned int y);
bool test_setPosition(Position& pos, const unsigned int x, const unsigned int y);
bool test_increaseX(Position& pos, int increment);
bool test_increaseY(Position& pos, int increment);
#endif // !UNIT_TEST
