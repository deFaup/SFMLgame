// Unitary tests for state
#include "unit_test.hpp"

using namespace std;

/********************************/
//			Statistics			//
/********************************/

// MAX_LIFE_POINT		2000
// MAX_ATTACK_POINT		2000
// MAX_MOVE_POINT		2000


int result()
{
	Statistics stat_test_enum(4000, -100, 1.2); // should set stats to 2000, 0 and 1
	Statistics stat_test_id(goku);

	cout << "test get_life with 4000, expected 2000: " << test_get_life_point(stat_test_enum, 2000) << endl;
	cout << "test get_life with goku, expected 100: " << test_get_life_point(stat_test_id, 100) << endl;
	
	cout << "test_get_attack_point with -100, expected 0: " << test_get_attack_point(stat_test_enum, 0) << endl;
	cout << "test_get_attack_point with goku, expected 10: " << test_get_attack_point(stat_test_id, 10) << endl;
	
	cout << "test_get_move_point with 1.2, expected 1: " << test_get_move_point(stat_test_enum, 1) << endl;
	cout << "test_get_move_point with goku, expected 100: " << test_get_move_point(stat_test_id, 100) << endl;

	stat_test_id.set_statistics(Statistics(10, 10, 10));
	cout << "test_set_statistics on goku with 10,10,10: " << test_set_statistics(stat_test_id, 10,10,10) << endl;

	stat_test_id.reset_all_but_life(goku);
	cout << "test_reset_all_but_life on goku expected 100/10/100: " << test_reset_all_but_life(stat_test_id, goku) << endl;

	return 0;
}

bool test_get_life_point(Statistics m_stat, unsigned int life)
{
	unsigned int m_life = m_stat.get_life_point(); //get life
	m_life = -1;	// modify the value that we got
	m_life -= 1;	// modify the value that we got
	return (m_stat.get_life_point() == life); // check if the attribute has been changed
}

bool test_get_attack_point(Statistics m_stat, unsigned int attack)
{
	unsigned int m_attack = m_stat.get_attack_point(); //get life
	m_attack = -1;	// modify the value that we got
	m_attack -= 1;	// modify the value that we got
	return (m_stat.get_attack_point() == attack); // check if the attribute has been changed
}

bool test_get_move_point(Statistics m_stat, unsigned int move)
{
	unsigned int m_move = m_stat.get_move_point(); //get life
	m_move = -1;	// modify the value that we got
	m_move -= 1;	// modify the value that we got
	return (m_stat.get_move_point() == move); // check if the attribute has been changed
}

bool test_set_statistics(Statistics given_stats, unsigned life, unsigned attack, unsigned move)
{
	return (given_stats.get_life_point() == life && given_stats.get_attack_point() == attack && given_stats.get_move_point() == move);
}

bool test_reset_all_but_life(Statistics given_stats, CharactersID id)
{
	unsigned int attack_point, move_point;
	switch (id)
	{
	case goku:
		//life_point = 100;
		attack_point = 10;
		move_point = 100;
		break;

	case vegeta:
		//life_point = 100;
		attack_point = 10;
		move_point = 100;
		break;

	case miyo:
		//life_point = 100;
		attack_point = 10;
		move_point = 100;
		break;
	}

	return (given_stats.get_attack_point() == attack_point && given_stats.get_move_point() == move_point);
}

