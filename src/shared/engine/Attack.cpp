#include "define.hpp"
#include "Attack.h"
#include "state/Player.h"
#include "state/Observable.h"
#include <iostream>
#include <memory>

using namespace engine;
using namespace std;
using namespace state;

int Attack::isLegit(state::GameState& etat)
{
	shared_ptr<Characters> character = etat.current_player->get_current_character();
	Statistics& stats = character->stats;
	
	std::vector<std::vector<unsigned int>> matrix;
	for(unsigned int i = 0; i < 501; i++)
		matrix.push_back(vector<unsigned int>(501,5));

	//matrix = new unsigned int*[501];
	//for (int i = 0; i < 501; i++)
	//	matrix[i] = new unsigned int[501];

	/*for(unsigned int i = 0; i < 501; i++){
		for(unsigned int j = 0; j < 501; j++){
			//if(i <= 120 && i >= 360 && j <= 120 && j >= 360)
				matrix[i][j] = 5;
			//else
			//	matrix[i][j] = 0;
		}*/
	//}

	Attacks attack(501,501,1,1,matrix);
	character->addAttack(attack);

	if(character->get_attack(attack_number).get_attack_cost() > stats.get_attack_point()){
		return(-1);
	}

	return(0);
}

void Attack::execute(state::GameState& etat){

	// reduction du nombre de point d'attaque pour ce tour
	shared_ptr<Characters> character = etat.current_player->get_current_character();
	Statistics& stats = character->stats;
	Statistics statsf(stats.get_life_point(),stats.get_attack_point()-character->get_attack(attack_number).get_attack_cost(),stats.get_move_point());
	stats.set_statistics(statsf);

	// recuperation de la matrice de champ d'action de l'attaque
	unsigned int size_x = character->get_attack(attack_number).get_nbcolumn();
	unsigned int size_y = character->get_attack(attack_number).get_nbline();
	cout << size_x << ", " << size_y << endl;
	/*unsigned int **matrix = character->get_attack(attack_number).get_attack_field_of_action();
	matrix = new unsigned int*[size_x];
	for (unsigned int i = 0; i < size_x; i++)
		matrix[i] = new unsigned int[size_y];*/
	std::vector<std::vector<unsigned int>> matrix = *(character->get_attack(attack_number).get_attack_field_of_action());

	// parcours de tout les personnages du jeu pour savoir si il sont impacté par l'attaque
	for(unsigned int k = 0; k < etat.players.size(); k++){
		shared_ptr<Player> cons_player = etat.players[k];
		for(unsigned int i = 0; i < cons_player->get_number_of_characters(); i++){
			shared_ptr<Characters> cons_char = cons_player->get_character(i);
			unsigned int positionX = cons_char->position.getPositionX();
			unsigned int positionY = cons_char->position.getPositionY();

			// Pour facilité les tests
			attack_position = character->position;

			if(positionX <= (attack_position.getPositionX() + size_x/2) &&
			   positionX >= (attack_position.getPositionX() - size_x/2) &&
			   positionY <= (attack_position.getPositionY() + size_y/2) &&
			   positionY >= (attack_position.getPositionY() - size_y/2)){
				// diminution du nombre de point de vie du personnage si l'attaque l'a atteinte
				Statistics& statsa = cons_char->stats;
				Statistics statsn(statsa.get_life_point() - matrix[positionX - attack_position.getPositionX() + size_x/2]	[positionY - attack_position.getPositionY() + size_y/2],statsa.get_attack_point(),statsa.get_move_point());
				statsa.set_statistics(statsn);
				if(statsa.get_life_point() == 0){
					// suppression du joueur si sa vie atteint 0
					cons_player->delete_character(i);
					// le joueur est supprimer du jeu par contre il reste affiché
				}
			}
		}
	}


	// modification/destruction de la map sur les endroits impactés
	std::vector<std::vector<int>> mask = etat.map.get_mask();
	for(unsigned int i = 0; i < size_x; i++){
		for(unsigned int j = 0; j < size_y; j++){
			if(matrix[i][j] != 0 &&
			   attack_position.getPositionX() + i - size_x/2 >= 0 && 
			   attack_position.getPositionX() + i - size_x/2 < mask.size() &&
			   attack_position.getPositionY() + j - size_y/2 >= 0 &&
			   attack_position.getPositionY() + j - size_y/2 < mask[0].size()){
				mask[attack_position.getPositionX() + i - size_x/2][attack_position.getPositionY() + j - size_y/2] = 0;
				// me suis planté sur les indices pour l'instant ça fait n'importe quoi
			}
		}
	}
	etat.map.set_mask(mask);
	
	// destruction de la matrice stockant temporairement le champ d'action de l'attaque
	/*for (unsigned int i = 0; i < size_x; i++)
		delete[] matrix[i];
	delete[] matrix;*/
	
	cout << "executing attack" << endl;
	return;
}
