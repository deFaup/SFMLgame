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
		etat->new_character(0, miyo);
		//etat->new_character(0, goku);
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
			etat->new_character(1, goku);
			etat->new_character(1, vegeta);
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
		}
		
		else if ((commandes.front().ID == arrow_left) || (commandes.front().ID == arrow_right))
		{
			Move move_command(commandes.front().ID);
			if (move_command.isLegit(*etat) != -1) {
				move_command.execute(*etat);
			}
		}

		else if ((commandes.front().ID == arrow_up) || (commandes.front().ID == arrow_down))
		{
			ChangeCharacter useless_var;
			useless_var.execute(*etat);
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
			if (commandes.front().ID == left_click)
			{
				//cout << "command is attack\n";
				Attack attack_command;
				attack_command.attack_position = commandes.front().mouse_position;
				attack_command.attack_number = 0;
				updating = true; // we forbid any call to scene.draw in main.cpp
				if (attack_command.isLegit(*etat) != -1)
					attack_command.execute(*etat);
				//updating = false;
			}
		}
	
		commandes.pop();
	}

	//render::sfEventsID arrow = arrow_down;
	//Move move_commande(arrow);
	//move_commande.execute(*etat);

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

void GameEngine::set_updating(bool true_false) { updating = true_false; }