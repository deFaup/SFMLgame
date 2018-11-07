/* Includes */
#include "define.hpp"
#include "Character.h"
#include "TileSet.h"
#include <memory>
#include <vector>
#include <iostream>

using namespace render;
using namespace state;
using namespace std;

Character::Character(const state::GameState& state) : state(state)
{
	/* get all existing characters */
	fill_characters_list();

	for (int i = 0; i < characters.size(); i++)
	{// for each character in game

		/* instantiation of a unique surface */
		surface.push_back(make_unique<Surface>());

		/* instantiation of a Tileset */
		tileset.push_back(make_shared<TileSet>());
	}
	
	load_tilset();

	/*scale*/
	float scale = 2.f;
	
	for (int i = 0; i < characters.size(); i++)
	{
		/* Linking its Surface texure to its Tileset */
		surface[i]->loadTexture(tileset[i]->getImage());

		/* Linking its Surface sprite to its Surface texture */
		surface[i]->setSpriteTexture();

		sf::Sprite& sprite( surface[i]->getSprite() );
		sprite.setScale(scale, scale);

	}

	update();
}

void Character::load_tilset()
{
	for (int i = 0; i < characters.size(); i++)
	{
		CharactersID id = characters[i]->get_id();
		switch (id)
		{
			case default_value:
				//life_point = 100;
			break;

			case goku:
				tileset[i]->setImageFile("res/DBZ_gokusheet2.gif");
			break;

			case vegeta:
				tileset[i]->setImageFile("res/DBZ_vegeta.gif");
			break;
/*
			case miyo: 
				tileset[i]->setImageFile("res/Computer - Eternal Fighter Zero - Mio Kouzuki.png");
				sf::Image& image = tileset[i]->getImage();
				image.createMaskFromColor(image.getPixel(0, 0));
			break;//58 14 57 82
*/			
		}
	}
}

void Character::fill_characters_list() {
	for (unsigned int i = 0; i < state.get_number_of_player(); i++)
	{
		for (unsigned int j = 0; j < state.get_player(i)->get_number_of_characters(); j++)
		{
			characters.push_back(state.players[i]->get_character(i));
		}
	}
}

void Character::update()
{
	// TODO re-init character list

	for (int i = 0; i < characters.size(); i++)
	{// go through all characters

		/* create a tile to find the sprite we want in our tileset */
		Tile tile;
		
		CharactersID id = characters[i]->get_id();
		switch (id)
		{
			/* Not really defined:
			can load the tile corresponding to the character's state
			can check the position and print the characters at their position
			*/
			case default_value:
				//life_point = 100;
			break;

			case goku:
				cout << " upadte\n";
				tile.setTile(0, 0, 42, 98);
			break;

			case vegeta:
				tile.setTile(0, 0, 50, 80);
			break;
		}

		surface[i]->setSpriteTexture(tile);
		surface[i]->setSpriteLocation(characters[i]->get_position().getPosition());
	}
}

void Character::setSurface(sf::RenderWindow& window)
{
	update();
	for (int i = 0; i < characters.size(); i++)
	{
		surface[i]->draw(window);
	}
	
}