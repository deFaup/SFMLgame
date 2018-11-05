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
	cout << "Scene created" << endl;
}

void Scene::updateScene(/* some event */)
{
	// the function looks at what have changed and only displays the changes
}

/* draw all layers (time-consuming) */
void Scene::draw()
{
	//int width(0), height(0);
	//state.get_map().get_dimensions(width, height);
	sf::RenderWindow renderWindow;
	sf::View window_view;
	sf::Vector2f position_in_map;

	init_window(window_view, renderWindow, position_in_map);

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

		update_view(window_view, renderWindow, position_in_map);

		background.setSurface(renderWindow); // first the background
		characters.setSurface(renderWindow); // on top of the background the characters
		renderWindow.display();
	}
}

/* config the view position for the beginning */
void Scene::init_window(sf::View& window_view, sf::RenderWindow& renderWindow, sf::Vector2f& position_in_map)
{
	int width(0), height(0);
	state::Map::get_screen_dimensions(width, height);
	width -= 10;	height -= 150;
	renderWindow.create(sf::VideoMode(width, height), "SFML game", sf::Style::Titlebar | sf::Style::Close);

	window_view.setSize(width, height);

	// set the first position in the game: temporary
	position_in_map.x = width/2 ; 	position_in_map.y = height/2;
	sf::Mouse::setPosition((sf::Vector2i)position_in_map);

	// set the view center so that the top left corner of the map is the top left corner of the window
	window_view.setCenter(position_in_map);

	position_in_map.x = 0; 	position_in_map.y = 0;

	renderWindow.setView(window_view);
	renderWindow.setPosition(sf::Vector2i(0, 0));
}

/* If we hit the border of the window then we move the view accordingly */
void Scene::update_view(sf::View& window_view, sf::RenderWindow& renderWindow, sf::Vector2f& position_in_map)
{
	unsigned int x = sf::Mouse::getPosition().x;
	unsigned int y = sf::Mouse::getPosition().y;

	//temporary
	int map_size[2] = { 0 };
	state.get_map().get_dimensions(map_size[0], map_size[1]);

	if (x >= renderWindow.getPosition().x + renderWindow.getSize().x)
		position_in_map.x += 10;

	if (x <= renderWindow.getPosition().x)
		position_in_map.x -= 10;

	if (y >= renderWindow.getPosition().y + renderWindow.getSize().y)
		position_in_map.y += 10;

	if (y <= renderWindow.getPosition().y)
		position_in_map.y -= 10;

	if (position_in_map.x >= map_size[0])
		position_in_map.x = map_size[0];
	else if (position_in_map.x <= 0)
		position_in_map.x = 0;

	if (position_in_map.y >= map_size[1])
		position_in_map.y = map_size[1];
	else if (position_in_map.y <= 0)
		position_in_map.y = 0;

	renderWindow.setView(window_view);
	window_view.setCenter(position_in_map);
	cout << x << " " << y << endl;
	cout << position_in_map.x << " " << position_in_map.y << endl << endl;
}