#include "define.hpp"
#include "GameEngine.h"
#include "Move.h"
#include "ChangeCharacter.h"
#include "Attack.h"
#include "ChangePlayer.h"
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
	commande.isSpacePressed = 0;
	commande.isCTRLPressed = 0;
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
		commande.isSpacePressed = 1;
	}

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

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		commande.isClicked = 1;
	}
	return;
}

void GameEngine::executeCommande(){
	if(commande.isSpacePressed == 1)
	{
		ChangePlayer tour_commande;
		tour_commande.execute(etat);
	}

	Move move_command(commande.arrow_direction);

	if (move_command.isLegit(etat) != -1) {
		move_command.execute(etat);
	}

	if(commande.isClicked == 1)
	{
		Attack attack_command;
		attack_command.attack_position = commande.mouse_position;
		attack_command.attack_number = 1;
		if(attack_command.isLegit(etat) != -1){
			attack_command.execute(etat);
		}
	}
	return;
}
