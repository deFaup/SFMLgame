/* Includes */
#include <iostream>
#include "define.hpp"
#include "Controller.h"
#include <string>

using namespace std;
//using namespace state;
using namespace render;
using namespace engine;

Controller::Controller(sf::RenderWindow& renderWindow, engine::GameEngine& engine) : renderWindow(renderWindow), engine(engine) {}

void Controller::handle_sfEvents(sf::Event& event)
{
	if (event.type == sf::Event::Closed)
		renderWindow.close();
	
	if (engine.etat.ID == state::StateID::started)
	{
		if (engine.etat.current_player->name == "IA")
		{
			return;
		}
	}
	
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Left)
			add_command(sfEvents(arrow_left));

		else if (event.key.code == sf::Keyboard::Right)
			add_command(sfEvents(arrow_right));

		else if (event.key.code == sf::Keyboard::Up)
			add_command(sfEvents(arrow_up));

		else if (event.key.code == sf::Keyboard::Down)
			add_command(sfEvents(arrow_down));
	}

	else if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::Space) {
			//cout << "engine events space released\n";
			add_command(sfEvents(space));
		}

		else if (event.key.code == sf::Keyboard::Return)
			add_command(sfEvents(enter));

		else if (event.key.code == sf::Keyboard::LControl)
			add_command(sfEvents(ctrl_l));

		else if (event.key.code == sf::Keyboard::RControl)
			add_command(sfEvents(ctrl_r));
	}

	else if (event.type == sf::Event::MouseButtonReleased)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			add_command(sfEvents(left_click));
			//cout << "engine events mouse left released\n";
		}
		else if (event.mouseButton.button == sf::Mouse::Right)
		{
			add_command(sfEvents(right_click));
			cout << "engine events mouse right released\n";
		}
	}

}

void Controller::add_command(sfEvents sf_event)
{
	engine.add_command(sf_event);
}
