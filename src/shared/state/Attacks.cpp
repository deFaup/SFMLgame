#include "define.hpp"
#include "Attacks.h"

using namespace state;

Attacks::Attacks()
{
	/*nbline = 15;
	nbcolumn = 15;
	attack_field_of_action = new unsigned int*[nbcolumn];
	for (unsigned int i = 0; i < nbcolumn; i++)
		attack_field_of_action[i] = new unsigned int[nbline];*/
}

Attacks::Attacks(unsigned int column, unsigned int line, unsigned int scope, unsigned int cost, unsigned int ** field)
{
	nbline = line;
	nbcolumn = column;
	/*attack_field_of_action = new unsigned int*[nbcolumn];
	for (unsigned int i = 0; i < nbcolumn; i++)
		attack_field_of_action[i] = new unsigned int[nbline];*/
	attack_cost = cost;
	attack_scope = scope;
	attack_field_of_action = field;
}

Attacks::~Attacks()
{
	for (unsigned int i = 0; i < nbcolumn; i++)
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

unsigned int Attacks::get_nbcolumn()
{
	return nbcolumn ;
}

unsigned int Attacks::get_nbline()
{
	return nbline ;
}

