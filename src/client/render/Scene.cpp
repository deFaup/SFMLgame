/* Includes */
#include <iostream>
#include "define.hpp"
#include "Scene.h"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace render;

/* Scene constructor share its references to the state and to the Map */
/* We share the map as doing state.get_map() wasn't working */
Scene::Scene(const state::GameState& state, const state::Map& map) : state(state), background(state, map), characters(state)
{
//	cout << "Scene created" << endl;
}

void Scene::updateScene(/* some event */)
{
	// the function looks at what have changed and only displays the changes
}

/* draw all layers (time-consuming) */
void Scene::draw()
{
	sf::RenderWindow renderWindow;
	sf::View window_view;
	sf::Vector2f position_in_map[2]; //[1] is the position with mouse coordinates
	init_window(window_view, renderWindow, position_in_map);
	const sf::Vector2f init_position = position_in_map[0];

	while (renderWindow.isOpen()) 
	{
		// Process events
		sf::Event event;

		while (renderWindow.pollEvent(event)) 
		{
			if (event.type == sf::Event::EventType::Closed)
				renderWindow.close();
		}
		renderWindow.clear();

		update_view(window_view, renderWindow, position_in_map, init_position);

		background.setSurface(renderWindow); // first the background
		characters.setSurface(renderWindow); // on top of the background the characters
		renderWindow.display();
	}
}

/* config the view position for the beginning */
void Scene::init_window(sf::View& window_view, sf::RenderWindow& renderWindow, sf::Vector2f (&position_in_map) [2])
{
	int width(0), height(0);
	state::Map::get_screen_dimensions(width, height);
	width -= 10;	height -= 150;
	renderWindow.create(sf::VideoMode(width, height), "SFML game", sf::Style::Titlebar | sf::Style::Close);

	window_view.setSize(width, height);
//	window_view.setSize(3000, 2000);

	// set the first position in the game: temporary
	position_in_map[0].x = position_in_map[1].x = width/2 ;
	position_in_map[0].y = position_in_map[0].y = height/2;
	sf::Mouse::setPosition((sf::Vector2i)position_in_map[0]);

	// set the view center so that the top left corner of the map is the top left corner of the window
	window_view.setCenter(position_in_map[0]);

	//position_in_map.x = 0; 	position_in_map.y = 0;

	renderWindow.setView(window_view);
	renderWindow.setPosition(sf::Vector2i(0, 0));
}

/* If we hit the border of the window then we move the view accordingly */
void Scene::update_view(sf::View& window_view, sf::RenderWindow& renderWindow, sf::Vector2f(&position_in_map)[2], sf::Vector2f init_position)
{
	unsigned int x = sf::Mouse::getPosition().x;
	unsigned int y = sf::Mouse::getPosition().y;
	position_in_map[1].x = position_in_map[0].x + x - init_position.x;
	position_in_map[1].y = position_in_map[0].y + y - init_position.y;

	//temporary
	int map_size[2] = { 0 };
	state.get_map().get_dimensions(map_size[0], map_size[1]);

	if (x >= renderWindow.getPosition().x + renderWindow.getSize().x)
		position_in_map[0].x += 10;

	if (x <= renderWindow.getPosition().x)
		position_in_map[0].x -= 10;

	if (y >= renderWindow.getPosition().y + renderWindow.getSize().y)
		position_in_map[0].y += 10;

	if (y <= renderWindow.getPosition().y)
		position_in_map[0].y -= 10;

	if (position_in_map[1].x >= map_size[0])
		position_in_map[0].x -= 10;
	else if (position_in_map[1].x <= 0)
		position_in_map[0].x += 10;

	if (position_in_map[1].y >= map_size[1])
		position_in_map[0].y -= 10;
	else if (position_in_map[1].y <= 0)
		position_in_map[0].y += 10;

	renderWindow.setView(window_view);
	window_view.setCenter(position_in_map[0]);
/*	cout << x << " " << y << endl;
	cout << position_in_map[1].x << " " << position_in_map[1].y << endl;
	cout << position_in_map[0].x << " " << position_in_map[0].y << endl << endl;
*/
}