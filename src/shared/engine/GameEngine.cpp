// Includes
#include "define.hpp"
#include "GameEngine.h"
#include "Move.h"
#include "ChangeCharacter.h"
#include "Attack.h"
#include "ChangePlayer.h"

#include <SFML/Window/Event.hpp>
#include <iostream>
#include "global_mutex.hpp"

using namespace state;
using namespace engine;
using namespace std;
using namespace render;

vector<sfEvents> executed;
vector<vector<vector<int>>> previous_mask;

GameEngine::GameEngine(state::GameState* etat) : etat(etat), updating(false)
{
	// init the game
	//check_stateID();
}
/* Configure the game map and 2 players */
//mode : 0 - 2 real players
//mode : !0 - you vs ia

void GameEngine::init_game(int mode)
{
	/* Create players, characters and a map. Will be rewritten when menu is implemented */
	if (etat->ID == not_started)
	{
		etat->new_map(3000, 2000);

		etat->new_player("Joueur 1");
		etat->new_character(0, vegeta);
		//etat->new_character(0, vegeta);
		//etat->new_character(0, vegeta);

		if (mode == 0)
		{
			etat->new_player("Joueur 2");
			etat->new_character(1, goku);
			etat->new_character(1, goku);
			etat->new_character(1, goku);
		}
		else
		{
			etat->new_player("IA");
			etat->new_character(1, miyo);
			etat->new_character(1, miyo);
			//etat->new_character(1, miyo);
		}
		
		auto& etat_id = etat->ID;
		etat_id = team_selected;
	}
}

void GameEngine::check_stateID()
{
	/* place all characters in line at the top of the map */
	if (etat->ID == team_selected)
	{
		unsigned int i = 10;

		for (auto characters_in_game : etat->get_characters())
		{
			characters_in_game->position.setPosition(i, 0);
			int width(0), height(0);
			etat->map.get_dimensions(width, height);
			i += 200 % width;
		}

		auto& etat_id = etat->ID;
		etat_id = team_placement;
	}

	/* Let user move their character with the mouse
		Valid position using space */
	else if (etat->ID == team_placement)
	{
		render::sfEventsID arrow = arrow_down;
		Move move_commande(arrow);
		move_commande.execute(*etat);

		executeCommandes();
	}

	else if (etat->ID == started) {
		executeCommandes();
		//cout << "check_stateID: started end\n" << endl;
	}

	else if (etat->ID == state::StateID::end)
	{
		cout << " vous avez remportez le jeu !! Bravo\n";
	}
}

void GameEngine::workLoop()
{
	while (!((etat->ID == state::StateID::end) || game_ended))
	{
		check_stateID();
	}
}

/*void GameEngine::executeCommande()
{
	if (commande.arrow_direction == arrow_up || commande.arrow_direction == arrow_down)
	{
		ChangeCharacter useless_var;
		useless_var.execute(etat);
	}
	if (commande.EnterWasPressed == 1)
	{
		ChangePlayer tour_commande;
		tour_commande.execute(etat);
	}

	// only execute left and right
	if (commande.arrow_direction == arrow_left || commande.arrow_direction == arrow_right) {
		Move move_command(commande.arrow_direction);
		if (move_command.isLegit(etat) != -1) {
			move_command.execute(etat);
		}
	}

	// switch between characters of a player. then goes to next player
	if (commande.isRightClicked == 1)
	{
		ChangeCharacter useless_var;
		useless_var.execute(etat);
		cout << "execute event click" << endl;
	}
		
		Attack attack_command;
		attack_command.attack_position = commande.mouse_position;
		attack_command.attack_number = 1;
		if(attack_command.isLegit(etat) != -1){
			attack_command.execute(etat);
		}
		
	if (etat.get_ID() == started){
		for(int k = 0; k < etat.characters.size(); k++){
			ChangeCharacter useless_var;
			useless_var.execute(etat);	
			ArrowDirection arrow = arrow_down;
			Move move_commande(arrow);
			move_commande.execute(etat);
		}
	}
	return;
}*/

void GameEngine::executeCommandes()
{
	while (!commandes.empty())
	{
		updating = true; // we execute each command one by one and others threads are paused
		//cout << "GameEngine::executeCommandes; commandes.size() = " << commandes.size() << endl;
		//cout << "commandes.ID = " << commandes.front().ID << endl;

		if (commandes.front().ID == enter)
		{
			ChangePlayer useless_var;
			useless_var.execute(*etat);
			global::next_player_cv.notify_all();
			//if(etat->ID == started)
			//	executed.push_back(commandes.front());
		}
		
		else if ((commandes.front().ID == arrow_left) || (commandes.front().ID == arrow_right))
		{
			Move move_command(commandes.front().ID);
			if (move_command.isLegit(*etat) != -1) {
				move_command.execute(*etat);
				//if(etat->ID == started)
				//	executed.push_back(commandes.front());
			}
		}

		else if ((commandes.front().ID == arrow_up) || (commandes.front().ID == arrow_down))
		{
			ChangeCharacter useless_var;
			useless_var.execute(*etat);
			//if(etat->ID == started)
			//	executed.push_back(commandes.front());
		}

		else if (commandes.front().ID == space)
		{
			while(!executed.empty())
			{
				rollback();
			}
		}

		if (etat->ID == team_placement) //commands to place your characters then start the game
		{
			static unsigned int players_placed = 0;
			if (commandes.front().ID == enter)
				players_placed++;
			// when all players have pressed enter once then we can start the game.
			// if one player presses enter several time it will keep the others from placing correctly

			etat->ID = (players_placed == etat->get_number_of_player()) ?  started:team_placement;
		}
		
		else if (etat->ID == started)
		{
			if (commandes.front().ID == num1 || commandes.front().ID == num2 || commandes.front().ID == num3 || commandes.front().ID == num4 || commandes.front().ID == num5)
			{	
				//cout << "command is attack\n";
				Attack attack_command;
				attack_command.attack_position = commandes.front().mouse_position;
				if(commandes.front().ID == num1)
					attack_command.attack_number = 0;
				else if(commandes.front().ID == num2)
					attack_command.attack_number = 1;
				else if(commandes.front().ID == num3)
					attack_command.attack_number = 2;
				else if(commandes.front().ID == num4)
					attack_command.attack_number = 3;
				else if(commandes.front().ID == num5)
					attack_command.attack_number = 4;
				updating = true; // we forbid any call to scene.draw in main.cpp
				if (attack_command.isLegit(*etat) != -1)
				{
					//previous_mask.push_back(etat->map.get_mask());
					attack_command.execute(*etat);
					//updating = false;
					//executed.push_back(commandes.front());
				}
			}

			//executed.push_back(commandes.front());		
		}
		
		commandes.pop();
	}

	render::sfEventsID arrow = arrow_down;
	Move move_commande(arrow);
	move_commande.execute(*etat);

	updating = false;
	//cout << "GameEngine:exec end\n";

	return;
}

void GameEngine::place_characters_with_mouse()
{
	/*Move move_command(sfEventID event = arrow_right);
	// add a call to is legit
	move_command.move_with_mouse(etat, commandes[].mouse_position);*/
}

void GameEngine::add_command(render::sfEvents commande){
	commandes.push(commande);
}

void GameEngine::rollback(void){
	//cout << "seg fault 1" << endl;
	sfEvents last_command = executed[executed.size()-1];
	if(last_command.ID == num1 || last_command.ID == num2 || last_command.ID == num3 || last_command.ID == num4 || last_command.ID == num5)
	{
		unsigned int attack_number;
		if(last_command.ID == num1)
			attack_number = 0;
		if(last_command.ID == num2)
			attack_number = 1;
		if(last_command.ID == num3)
			attack_number = 2;
		if(last_command.ID == num4)
			attack_number = 3;
		if(last_command.ID == num5)
			attack_number = 4;

		unsigned int size_x, size_y;
		size_x = etat->current_player->current_character->get_attack(attack_number).get_nbcolumn();
		size_y = etat->current_player->current_character->get_attack(attack_number).get_nbline();

		vector<vector<int>> matrix = previous_mask[previous_mask.size()-1];
		etat->map.set_mask(matrix);

		std::deque<std::shared_ptr<Characters>> characters = etat->get_characters();

		Statistics& stats = etat->current_player->get_current_character()->stats;
		stats.increase_attack_point(etat->current_player->current_character->get_attack(attack_number).get_attack_cost());

		for(unsigned int i = 0; i < etat->characters.size(); i++)
		{
			std::shared_ptr<Characters> temp_character = characters[i];
			state::Position& pos = etat->current_player->get_current_character()->position;
			std::vector<std::vector<int>> mask = etat->map.get_mask();
			unsigned int t = 0;
			while(mask[pos.getPositionY()+270-t][pos.getPositionX()] != 0){
				t++;
			}
			pos.increaseY(-t);
		}
		for(unsigned int i = 0; i < etat->characters.size(); i++)
		{
			vector<vector<unsigned int>> matrix2 = *(etat->current_player->current_character->get_attack(attack_number).get_attack_field_of_action());
			Position attack_position = etat->current_player->current_character->position;
			std::shared_ptr<Characters> temp_character = characters[i];
			unsigned int positionX = temp_character->position.getPositionX();
			unsigned int positionY = temp_character->position.getPositionY();

			if(positionX <= (attack_position.getPositionX() + size_x/2) &&
			   positionX >= (attack_position.getPositionX() - size_x/2) &&
			   positionY <= (attack_position.getPositionY() + size_y/2) &&
			   positionY >= (attack_position.getPositionY() - size_y/2))
			{	
				// diminution du nombre de point de vie du personnage si l'attaque l'a atteinte
				Statistics& statsa = temp_character->stats;
				Statistics statsn(statsa.get_life_point() + matrix2[positionX - attack_position.getPositionX() + size_x/2]	[positionY - attack_position.getPositionY() + size_y/2],statsa.get_attack_point(),statsa.get_move_point());
				statsa.set_statistics(statsn);
			}
		}
		previous_mask.erase(previous_mask.begin() + previous_mask.size()-1);
	}
	else if((last_command.ID == arrow_up) || (last_command.ID == arrow_down))
	{
		ChangeCharacter useless_var;
		useless_var.execute(*etat);
		useless_var.execute(*etat);
	}
	else if(last_command.ID == arrow_right)
	{
		state::Position& pos = etat->current_player->get_current_character()->position;
		std::vector<std::vector<int>> mask = etat->map.get_mask();
		Statistics& stats = etat->current_player->get_current_character()->stats;
		unsigned int speed = 8;

		if(mask[pos.getPositionY()+270][pos.getPositionX()-speed] == 0){
			pos.increaseX(-speed);
			stats.increase_move_point(1);
		}
		else{
			unsigned int i = 0;
			while(mask[pos.getPositionY()+270-i][pos.getPositionX()-speed] != 0){
				i++;
			}
			pos.increaseX(-speed);
			pos.increaseY(-i);
			stats.increase_move_point(1);
		}
	}
	else if(last_command.ID == arrow_left)
	{
		state::Position& pos = etat->current_player->get_current_character()->position;
		std::vector<std::vector<int>> mask = etat->map.get_mask();
		Statistics& stats = etat->current_player->get_current_character()->stats;
		unsigned int speed = 8;

		if(mask[pos.getPositionY()+270][pos.getPositionX()+speed] == 0){
			pos.increaseX(speed);
			stats.increase_move_point(1);
		}
		else{
			unsigned int i = 0;
			while(mask[pos.getPositionY()+270-i][pos.getPositionX()+speed] != 0){
				i++;
			}
			pos.increaseX(speed);
			pos.increaseY(-i);
			stats.increase_move_point(1);
		}
	}
	else if (last_command.ID == enter)
	{
		ChangePlayer useless_var;
		useless_var.execute(*etat);
		global::next_player_cv.notify_all();
	}
	executed.erase(executed.begin() + executed.size()-1);
}

void GameEngine::set_updating(bool true_false) { updating = true_false; }
