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

	if(character->get_attack(attack_number).get_attack_cost() > stats.get_attack_point()){
		return(-1);
	}

	return(0);
}

void Attack::execute(state::GameState& etat)
{
	//std::cout << "\nAttack::execute begin" << std::endl;
	// reduction du nombre de point d'attaque pour ce tour
	unsigned int size_x, size_y;
	std::vector<std::vector<unsigned int>> matrix;
	{
		shared_ptr<Characters> character = etat.current_player->get_current_character();
		character->stats.increase_attack_point(0 - character->get_attack(attack_number).get_attack_cost() );

		// Pour faciliter les tests
		attack_position = character->position;
		attack_position.increaseY(270);

		// recuperation de la matrice de champ d'action de l'attaque
		size_x = character->get_attack(attack_number).get_nbcolumn();
		size_y = character->get_attack(attack_number).get_nbline();
		std::cout << size_x << ", " << size_y << std::endl;

		matrix = *(character->get_attack(attack_number).get_attack_field_of_action());
	}

	// parcours de tout les personnages du jeu pour savoir si il sont impacté par l'attaque
	// puisque l'on supprime des personnages et joueurs s'ils sont morts 
	// est ce qu'on aurait pas un problème avec les indices que l'on utilise !?
	// pas d'erreurs de segmentation normalement car la condition est tjs testé
	// on loupe juste des passages dans la boucle.
	// Solution: en cas de mort d'un perso (si stat ==0) alors on fait i--; idem pour la mort d'un joueur.
	// reactualiser le current player dans le cas ou le joueur courant lance l'attaque et meurt.
	//for(int k = 0; 
	//	k < (int)etat.players.size() && etat.get_number_of_player() > 1;
	//	k++)
	//{	
	if (etat.get_number_of_player() <= 1) return;

	for (auto cons_player : etat.players)
	{
		if (etat.get_number_of_player() <= 1) break;

		//shared_ptr<Player> cons_player = etat.get_player(k);
		//std::cout << cons_player->name << "\n";

		//for(int i = 0; i < (int)cons_player->get_number_of_characters(); i++)
		//int i(0);

		for (auto cons_char : cons_player->get_characters())
		{
			// impossible for a character to hurt himself
			// until we config the possibility to see the range of an action and launch it with mouse
			if (cons_char == etat.current_player->get_current_character())
				continue;

			//std::cout << "character n: " << i << "\n"; i++;
			//shared_ptr<Characters> cons_char = cons_player->get_character(i);
			unsigned int positionX = cons_char->position.getPositionX();
			unsigned int positionY = cons_char->position.getPositionY();

			if(positionX <= (attack_position.getPositionX() + size_x/2) &&
			   positionX >= (attack_position.getPositionX() - size_x/2) &&
			   positionY <= (attack_position.getPositionY() + size_y/2) &&
			   positionY >= (attack_position.getPositionY() - size_y/2))
			{	
				// diminution du nombre de point de vie du personnage si l'attaque l'a atteinte

				Statistics& statsa = cons_char->stats;
				Statistics statsn(statsa.get_life_point() - matrix[positionX - attack_position.getPositionX() + size_x/2]	[positionY - attack_position.getPositionY() + size_y/2],statsa.get_attack_point(),statsa.get_move_point());
				statsa.set_statistics(statsn);
				
				//if(statsa.get_life_point() == 0)
				//{
				//	//i--; // decrease i since we've just reduced the number of characters
				//	if (cons_player->get_number_of_characters() == 0)
				//	{
				//		//k--; // decrease k since we've just reduced the number of players
				//	}
				//}
			}
		}
	}


	// modification/destruction de la map sur les endroits impactés
	std::vector<std::vector<int>> mask = etat.map.get_mask();
	for(unsigned int i = 0; i < size_x; i++){
		for(unsigned int j = 0; j < size_y; j++){
			if(matrix[i][j] != 0 &&
			   attack_position.getPositionX() + i - size_x/2 >= 0 && 
			   attack_position.getPositionX() + i - size_x/2 < mask[0].size() &&
			   attack_position.getPositionY() + j - size_y/2 >= 0 &&
			   attack_position.getPositionY() + j - size_y/2 < mask.size()){
				mask[attack_position.getPositionY() + j - size_y/2][attack_position.getPositionX() + i - size_x/2] = 0;
				// me suis planté sur les indices pour l'instant ça fait n'importe quoi
			}
		}
	}
	etat.map.set_mask(mask);
	
	//std::cout << "Attack::execute end" << std::endl;
	return;
}
