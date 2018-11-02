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
	/* instantiation of a unique work surface */
	surface = std::make_unique<Surface>();

	/* get all existing characters */
	fill_characters_list();

	for (int i = 0; i < characters.size(); i++)
	{
		/* create a tile to find the sprite we want in our tileset */
		Tile tile;

		const CharactersID id = characters[i]->get_id();
		switch (id)
		{
			case default_value:
				//life_point = 100;
			break;

			case goku:
				tileset.push_back(std::make_shared<TileSet>("res/DBZ_gokusheet2.gif"));
				tile.setTile(134, 192, 80, 80);
			break;

			case vegeta:
				//life_point = 100;
			break;

			case miyo:
				//life_point = 100;
			break;
		}
		sf::Image image = tileset[0]->getImage();
		image.createMaskFromColor(image.getPixel(1, 1));

		surface->loadTexture(image);
		surface->setSpriteTexture(tile);
		surface->setSpriteLocation(characters[i]->get_position());
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