#include "define.hpp"
#include "Attacks.h"

using namespace state;

Attacks::Attacks()
{
	int m = 15;
	int n = 15;
	attack_field_of_action = new unsigned int*[m];
	for (int i = 0; i < m; i++)
		attack_field_of_action[i] = new unsigned int[n];
}

Attacks::Attacks(unsigned int scope, unsigned int cost, unsigned int ** field)
{
	int m = 15;
	int n = 15;
	attack_field_of_action = new unsigned int*[m];
	for (int i = 0; i < m; i++)
		attack_field_of_action[i] = new unsigned int[n];
	attack_cost = cost;
	attack_scope = scope;
	attack_field_of_action = field;
}

Attacks::~Attacks()
{
	int m = 15;
	int n = 15;
	for (int i = 0; i < m; i++)
		delete[] attack_field_of_action[i];
	delete[] attack_field_of_action;
}

unsigned int Attacks::get_attack_cost()
{
	return attack_cost ;
}

unsigned int Attacks::get_attack_scope()
{
	return attack_scope ;
}

unsigned int ** Attacks::get_attack_field_of_action()
{
	return attack_field_of_action ;
}

