// Includes
#include "global_mutex.hpp"
#include "define.hpp"
#include "engine.h"

//#include <SFML/Window/Event.hpp>
#include <iostream>

// threads, thread::sleep_for
#include <thread>
#include <chrono>

using namespace state;
using namespace engine;
using namespace std;

int speedp[10] = {0};
Json::Value conv_event_to_json(state::sfEvents& to_export);

GameEngine::GameEngine(state::GameState* etat) : etat(etat), updating(false), network_active(false){}

void GameEngine::check_stateID()
{
	if (etat->ID == started)	executeCommandes();

	/* place all characters in line at the top of the map */
	else if (etat->ID == team_selected)
	{
		unsigned int i = 10;

		for (auto characters_in_game : etat->get_characters())
		{
			characters_in_game->position.setPosition(i, 0);
			//int width(0), height(0);
			//etat->map.get_dimensions(width, height);
			//i += 200 % width;
		}

		etat->ID = team_placement;
	}

	/* Let user move their character with the mouse
		Valid position using space */
	else if (etat->ID == team_placement)
	{
		Move move_commande(state::sfEventsID::arrow_down);
		int four = 4;
		while (four != 0)
		{
			four--;
			move_commande.execute(*etat);
			//std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
		executeCommandes();
	}

	else if (etat->ID == state::StateID::end)
	{
		cout << "Vous avez remportez le jeu !! Bravo\n";
	}

	else std::this_thread::sleep_for(std::chrono::milliseconds(420));
}

void GameEngine::workLoop()
{
	while (!((etat->ID == state::StateID::end) || game_ended))
	{
		global::get_engine.lock();
		check_stateID();
		global::get_engine.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(80));
	}
}

void GameEngine::executeCommandes()
{
	while (!commandes.empty())
	{
		updating = true; // we execute each command one by one and others threads are paused
		//std::cout << "GameEngine::executeCommandes; commandes.size() = " << commandes.size() << endl;
		//std::cout << "commandes.ID = " << commandes.front().ID << endl;

		if (commandes.front().ID == enter)
		{
			ChangePlayer useless_var;
			useless_var.execute(*etat);
			global::next_player_cv.notify_all();
			//if (JSONActive)
			//	export_json(commandes.front());
			if (etat->ID == started && rollbackActive)
				executed.push_back(commandes.front());
		}

		else if ((commandes.front().ID == arrow_left) || (commandes.front().ID == arrow_right))
		{
			Move move_command(commandes.front().ID);
			if (move_command.isLegit(*etat) != -1) {
				move_command.execute(*etat);
				//if (JSONActive)
				//	export_json(commandes.front());
				if (etat->ID == started && rollbackActive)
					executed.push_back(commandes.front());
			}
		}

		else if ((commandes.front().ID == arrow_up) || (commandes.front().ID == arrow_down))
		{
			ChangeCharacter useless_var;
			useless_var.execute(*etat);
			//if (JSONActive)
			//	export_json(commandes.front());
			if (etat->ID == started && rollbackActive)
				executed.push_back(commandes.front());
		}

		else if (commandes.front().ID == space)
		{
			if (etat->ID == started)
			{
				//if (JSONActive)
				//	export_json(commandes.front());
				/*if(rollbackActive)
				{
				while(!executed.empty())
				{
				rollback();
				}
				rollbackActive = false;
				}
				else
				{
				rollbackActive = true;*/
				for (unsigned int i = 0; i < etat->characters.size(); i++)
				{
					if (etat->characters[i] == etat->current_player->current_character)
					{
						speedp[i] = -40;
					}
				}
				//}
			}
		}

		if (etat->ID == team_placement) //commands to place your characters then start the game
		{
			static unsigned int players_placed = 0;
			if (commandes.front().ID == enter)
				players_placed++;
			// when all players have pressed enter once then we can start the game.
			// if one player presses enter several time it will keep the others from placing correctly

			etat->ID = (players_placed == etat->get_number_of_player()) ? started : team_placement;
		}

		else if (etat->ID == started)
		{
			if (commandes.front().ID == num1 || commandes.front().ID == num2 || commandes.front().ID == num3 || commandes.front().ID == num4 || commandes.front().ID == num5)
			{
				//cout << "command is attack\n";
				Attack attack_command;
				attack_command.attack_position = commandes.front().mouse_position;
				if (commandes.front().ID == num1)
					attack_command.attack_number = 0;
				else if (commandes.front().ID == num2)
					attack_command.attack_number = 1;
				else if (commandes.front().ID == num3)
					attack_command.attack_number = 2;
				else if (commandes.front().ID == num4)
					attack_command.attack_number = 3;
				else if (commandes.front().ID == num5)
					attack_command.attack_number = 4;
				updating = true; // we forbid any call to scene.draw in main.cpp
				if (attack_command.isLegit(*etat) != -1)
				{
					//if (JSONActive)
					//	export_json(commandes.front());
					if (rollbackActive)
					{
						previous_mask.push_back(etat->map.get_mask());
						executed.push_back(commandes.front());
					}
					attack_command.execute(*etat);
					updating = false;
				}
			}
			//executed.push_back(commandes.front());		
		}

		commandes.pop();
	}

	gestion_gravite();
	updating = false;
	//cout << "GameEngine:exec end\n";

	return;
}

void GameEngine::gestion_gravite(void)
{
	if(etat->ID == team_placement)
		return;
	for(unsigned int i = 0; i < etat->characters.size(); i++)
	{
		std::shared_ptr<Characters> temp_character = etat->characters[i];
		state::Position& pos = temp_character->position;
		std::vector<std::vector<int>> mask = etat->map.get_mask();
		if(pos.getPositionY()+speedp[i] >= 1999){
			Statistics& statsa = temp_character->stats;
			Statistics statsn(0,statsa.get_attack_point(),statsa.get_move_point());
			statsa.set_statistics(statsn);
		}
		else if(mask[pos.getPositionY()+speedp[i]][pos.getPositionX()] == 0){
			speedp[i] = speedp[i]+1;
			pos.increaseY(speedp[i]);
		}
		else{
			speedp[i] = 0;
		}
	}
}

void GameEngine::place_characters_with_mouse()
{
	/*Move move_command(sfEventID event = arrow_right);
	// add a call to is legit
	move_command.move_with_mouse(etat, commandes[].mouse_position);*/
}

void GameEngine::add_command(state::sfEvents commande)
{
	if (JSONActive)
	{
		static int i = 0;
		global::json_commandes["commandes"][i] = conv_event_to_json(commande);
		i++;
	}

	commandes.push(commande);
}

void GameEngine::rollback(void){
	state::sfEvents last_command = executed[executed.size()-1];
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
			while(mask[pos.getPositionY()-t][pos.getPositionX()] != 0){
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

		if(mask[pos.getPositionY()][pos.getPositionX()-SPEED] == 0){
			pos.increaseX(-SPEED);
			stats.increase_move_point(1);
		}
		else{
			unsigned int i = 0;
			while(mask[pos.getPositionY()-i][pos.getPositionX()-SPEED] != 0){
				i++;
			}
			pos.increaseX(-SPEED);
			pos.increaseY(-i);
			stats.increase_move_point(1);
		}
	}
	else if(last_command.ID == arrow_left)
	{
		state::Position& pos = etat->current_player->get_current_character()->position;
		std::vector<std::vector<int>> mask = etat->map.get_mask();
		Statistics& stats = etat->current_player->get_current_character()->stats;

		if(mask[pos.getPositionY()][pos.getPositionX()+SPEED] == 0){
			pos.increaseX(SPEED);
			stats.increase_move_point(1);
		}
		else{
			unsigned int i = 0;
			while(mask[pos.getPositionY()-i][pos.getPositionX()+SPEED] != 0){
				i++;
			}
			pos.increaseX(SPEED);
			pos.increaseY(-i);
			stats.increase_move_point(1);
		}
	}
	else if (last_command.ID == enter)
	{
		ChangePlayer useless_var;
		for (unsigned int i = 0; i < etat->get_number_of_player() - 1; i++)
			useless_var.execute(*etat);
		//global::next_player_cv.notify_all();
	}
	executed.erase(executed.begin() + executed.size()-1);
}

void GameEngine::set_updating(bool true_false) { updating = true_false; }

//void GameEngine::export_json(state::sfEvents& to_export)
//{
//	static int i = 0;
//
//	Json::Value JsonCmd;
//	JsonCmd["id"] = global::player_id;
//	JsonCmd["sfEventsID"] = to_export.ID;
//	JsonCmd["x"] = to_export.mouse_position.getPositionX();
//	JsonCmd["y"] = to_export.mouse_position.getPositionY();
//
//	if (JSONActive)
//	{
//		global::json_commandes["commandes"][i] = JsonCmd;
//		i++;
//	}
//	else if (network_active)
//	{
//		//send_command(JsonCmd); // si reponse est BAD alors on ajoute rien sinon on ajoute à la queue
//	}
//}

Json::Value conv_event_to_json(state::sfEvents& to_export)
{
	Json::Value JsonCmd;
	JsonCmd["id"] = global::player_id;
	JsonCmd["sfEventsID"] = to_export.ID;
	JsonCmd["x"] = to_export.mouse_position.getPositionX();
	JsonCmd["y"] = to_export.mouse_position.getPositionY();

	return JsonCmd;
}

