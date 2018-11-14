// Includes
#include "define.hpp"
#include "GameEngine.h"
#include "Move.h"
#include "ChangeCharacter.h"
#include "Attack.h"
#include "ChangePlayer.h"

#include <SFML/Window/Event.hpp>
#include <iostream>

using namespace state;
using namespace engine;
using namespace std;

GameEngine::GameEngine(state::GameState& etat, sf::RenderWindow& window) : etat(etat), renderWindow(window) 
{
	commande.SpaceWasPressed = false;
	commande.EnterWasPressed = false;
	commande.CtrlWasPressed = false;
	commande.isRightClicked = false;
	commande.isLeftClicked = false;

	// init the game
	check_stateID();
}

void GameEngine::check_stateID()
{
	/* Create players, characters and a map. Will be rewritten when menu is implemented */
	if (etat.get_ID() == not_started)
	{
		etat.new_map(3000, 2000);

		for (int i = 0; i < 3; i++)
		{
			etat.new_player("Joueur " + to_string(i));
			etat.new_character(i, vegeta);
			etat.new_character(i, goku);
		}

		state::StateID& etat_id = etat.get_ID();
		etat_id = team_selected;
	}

	/* place all characters in line at the top of the map */
	else if (etat.get_ID() == team_selected)
	{
		unsigned int i = 10;

		for (auto characters_in_game : const_cast<vector<shared_ptr<Characters>>&> (etat.get_characters()) )
		{
			characters_in_game->get_position().setPosition(i, 0);
			int width(0), height(0);
			etat.get_map().get_dimensions(width, height);
			i += 100 % width;
		}

		state::StateID& etat_id = etat.get_ID();
		etat_id = team_placement;
	}

	/* Let user move their character with the mouse
		Valid position using space */
	else if (etat.get_ID() == team_placement)
	{
		static int counter = 0;
		if (counter < etat.get_characters().size()+1)
		{
			getUserInput();
			place_characters_with_mouse();
			if (commande.isRightClicked == 1)
				counter++;
			executeCommande();
		}
		else
		{
			state::StateID& etat_id = etat.get_ID();
			etat_id = started;
		}
	}

	else if (etat.get_ID() == started){
		getUserInput();
		executeCommande();
	}

	// reset event commands here as main first while loop functions are called more than those in the second one (pollEvents)
	commande.SpaceWasPressed = false;
	commande.EnterWasPressed = false;
	commande.CtrlWasPressed = false;
	commande.isLeftClicked = false;
	commande.isRightClicked = false;
}

void GameEngine::getUserInput(){

	commande.arrow_direction = arrow_none;
	sf::Vector2i globalPosition = sf::Mouse::getPosition(renderWindow);
	commande.mouse_position.setPosition(globalPosition.x,globalPosition.y);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		commande.arrow_direction = arrow_left;

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		commande.arrow_direction = arrow_right;

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		commande.arrow_direction = arrow_up;

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		commande.arrow_direction = arrow_down;

	return;
}

void GameEngine::executeCommande()
{
	if (commande.arrow_direction == arrow_up || commande.arrow_direction == arrow_down)
	{
		ChangeCharacter useless_var;
		useless_var.execute(etat);
	}
/*	if (commande.EnterWasPressed == 1)
	{
		ChangePlayer tour_commande;
		tour_commande.execute(etat);
	}
*/

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
		/*
		Attack attack_command;
		attack_command.attack_position = commande.mouse_position;
		attack_command.attack_number = 1;
		if(attack_command.isLegit(etat) != -1){
			attack_command.execute(etat);
		}
		*/
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
}

void GameEngine::look_sfEvents(sf::Event& event)
{
	if (event.type == sf::Event::Closed)
		renderWindow.close();

	else if(event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::Space) {
			//cout << "engine events space released\n";
			commande.SpaceWasPressed = true;
		}

		else if (event.key.code == sf::Keyboard::Return)
			commande.EnterWasPressed = true;
		
		else if (event.key.code == sf::Keyboard::LControl || sf::Keyboard::RControl)
			commande.CtrlWasPressed = true;
	}

	else if (event.type == sf::Event::MouseButtonReleased)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			commande.isLeftClicked = 1;
			//cout << "engine events mouse left released\n";
		}
		if (event.mouseButton.button == sf::Mouse::Right)
		{
			commande.isRightClicked = 1;
			cout << "engine events mouse right released\n";
		}
	}
		
}

void GameEngine::place_characters_with_mouse()
{
	Move move_command(arrow_none);
	// add a call to is legit
	move_command.move_with_mouse(etat, commande.mouse_position);
}
