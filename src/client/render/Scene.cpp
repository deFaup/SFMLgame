/* Includes */
#include <iostream>
#include "define.hpp"
#include "Scene.h"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace render;

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
		background.update();
		background.setSurface(renderWindow);
		characters.setSurface(renderWindow);
		renderWindow.display();
	}

}
