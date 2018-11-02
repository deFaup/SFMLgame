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
	int width(0), height(0);
	state.get_map().get_dimensions(width, height);
	sf::RenderWindow renderWindow(sf::VideoMode(width, height), "test affichage", sf::Style::Titlebar| sf::Style::Close);


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
		background.setSurface(renderWindow); // first the background
		characters.setSurface(renderWindow); // on top of the background the characters
		renderWindow.display();
	}

}
