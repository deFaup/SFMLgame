#ifndef UNIT_TEST
#define UNIT_TEST

#include <iostream>
#include <string>
#include "define.hpp"
#include "state.h"

using namespace state;

bool test_get_life_point(Statistics m_stat, unsigned int life);
bool test_get_attack_point(Statistics m_stat, unsigned int attack);
bool test_get_move_point(Statistics m_stat, unsigned int move);
bool test_set_statistics(Statistics given_stats, unsigned life, unsigned attack, unsigned move);
bool test_reset_all_but_life(Statistics given_stats, CharactersID id);

int result();

#endif // !UNIT_TEST
