#include "define.hpp"
#include "GameEngine.h"
#include "Move.h"
#include "ChangeCharacter.h"
#include "Attack.h"
#include <memory>
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace engine;
using namespace std;

GameEngine::GameEngine(state::GameState& etat) : etat(etat){}

void GameEngine::getUserInput(){

	commande.arrow_direction = arrow_none;
	commande.isClicked = 0;
	sf::Vector2i globalPosition = sf::Mouse::getPosition();
	commande.mouse_position.setPosition(globalPosition.x,globalPosition.y);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		commande.arrow_direction = arrow_left;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		commande.arrow_direction = arrow_right;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		commande.arrow_direction = arrow_up;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		commande.arrow_direction = arrow_down;
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		commande.isClicked = 1;
	}
	return;
}

void GameEngine::executeCommande(){
	if(commande.arrow_direction == arrow_left || commande.arrow_direction == arrow_right){
		Move move_command;
		if(commande.arrow_direction == arrow_left){
			move_command.move_type = MoveLeft;
		}
		else{
			move_command.move_type = MoveRight;
		}
		if(move_command.isLegit(etat) != -1){
			move_command.execute(etat);
		}
	}
	if(commande.isClicked == 1){
		Attack attack_command;
		attack_command.attack_position = commande.mouse_position;
		attack_command.attack_number = 1;
		if(attack_command.isLegit(etat) != -1){
			attack_command.execute(etat);
		}
	}
	
	return;
}
