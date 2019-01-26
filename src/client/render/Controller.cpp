/* Includes */
#include <iostream>
#include "define.hpp"
#include "global_mutex.hpp"

#include "Controller.h"
#include <string>
#include <SFML/Network.hpp>

using namespace std;
//using namespace state;
using namespace render;
//using namespace engine;

extern Json::Value conv_event_to_json(state::sfEvents& to_export);
extern bool send_command(Json::Value& request_body);

Controller::Controller(sf::RenderWindow& renderWindow, engine::GameEngine& engine, state::GameState& gameState) 
	: renderWindow(renderWindow), engine(engine), gameState(gameState) {}

void Controller::handle_sfEvents(sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		renderWindow.close();
		engine.game_ended = true;
	}
	
	if (engine.etat->ID == state::StateID::started)
	{
		if (gameState.current_player->name == AI_1 ||
			gameState.current_player->name == AI_2 ||
			gameState.current_player->name == AI_3 ||
			gameState.current_player->name == AI_4 ||
			gameState.current_player->name == AI_5)
		{
			return;
		}
	}
	
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Left)
			add_command(state::sfEvents(state::sfEventsID::arrow_left));

		else if (event.key.code == sf::Keyboard::Right)
			add_command(state::sfEvents(state::sfEventsID::arrow_right));

		else if (event.key.code == sf::Keyboard::Up)
			add_command(state::sfEvents(state::sfEventsID::arrow_up));

		else if (event.key.code == sf::Keyboard::Down)
			add_command(state::sfEvents(state::sfEventsID::arrow_down));
	}

	else if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::Space)
			add_command(state::sfEvents(state::sfEventsID::space));

		else if (event.key.code == sf::Keyboard::Return)
			add_command(state::sfEvents(state::sfEventsID::enter));

		else if (event.key.code == sf::Keyboard::LControl)
			add_command(state::sfEvents(state::sfEventsID::ctrl_l));

		else if (event.key.code == sf::Keyboard::RControl)
			add_command(state::sfEvents(state::sfEventsID::ctrl_r));

		else if (event.key.code == sf::Keyboard::Num1 || event.key.code == sf::Keyboard::Numpad1)
			add_command(state::sfEvents(state::sfEventsID::num1));

		else if (event.key.code == sf::Keyboard::Num2 || event.key.code == sf::Keyboard::Numpad2)
			add_command(state::sfEvents(state::sfEventsID::num2));

		else if (event.key.code == sf::Keyboard::Num3 || event.key.code == sf::Keyboard::Numpad3)
			add_command(state::sfEvents(state::sfEventsID::num3));

		else if (event.key.code == sf::Keyboard::Num4 || event.key.code == sf::Keyboard::Numpad4)
			add_command(state::sfEvents(state::sfEventsID::num4));

		else if (event.key.code == sf::Keyboard::Num5 || event.key.code == sf::Keyboard::Numpad5)
			add_command(state::sfEvents(state::sfEventsID::num5));
	}

	else if (event.type == sf::Event::MouseButtonReleased)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			add_command(state::sfEvents(state::sfEventsID::left_click));
			//cout << "engine events mouse left released\n";
		}
		else if (event.mouseButton.button == sf::Mouse::Right)
		{
			add_command(state::sfEvents(state::sfEventsID::right_click));
			cout << "engine events mouse right released\n";
		}
	}

}

// no need to put a reference as I declare the sfEvents inside the call of the function (see above)
void Controller::add_command(state::sfEvents sf_event)
{
	if (engine.network_active)
		// convertir en json
		// envoyer au serveur
		// attendre la réponse
		// si réponse ajouter la commande
	{
		Json::Value cmd = conv_event_to_json(sf_event);
		bool response = send_command(cmd);
		if (!response)
			return;
	}
	
	engine.add_command(sf_event);
}
