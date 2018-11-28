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

Character::~Character(){ cout << "Character deleted" << endl; }

Character::Character(const state::GameState& state) : state(state), characters(state.get_characters())
{
	for (unsigned int i = 0; i < characters.size(); i++)
	{// for each character in game

		/* instantiation of a unique surface */
		surface.push_back(make_unique<Surface>());

		/* instantiation of a Tileset */
		tileset.push_back(make_shared<TileSet>());
	}
	
	load_tilset();

	/*scale*/
	float scale = 3.5f;
	
	for (unsigned int i = 0; i < characters.size(); i++)
	{
		/* Linking its Surface texure to its Tileset */
		surface[i]->loadTexture(tileset[i]->getImage());

		/* Linking its Surface sprite to its Surface texture */
		surface[i]->setSpriteTexture();

		sf::Sprite& sprite( surface[i]->getSprite() );
		sprite.setScale(scale, scale);

	}

	update();
	cout << "Character created" << endl;
}

void Character::load_tilset()
{
	for (unsigned int i = 0; i < characters.size(); i++)
	{
		CharactersID id = characters[i]->get_id();
		switch (id)
		{
			case goku://stand: 136x153 pos 140x4585
				tileset[i]->setImageFile("res/DBZ_gokusheet2.gif");
			break;

			case vegeta:
				tileset[i]->setImageFile("res/DBZ_vegeta.gif");
			break;	

			case miyo:
				tileset[i]->setImageFile("res/Computer - Eternal Fighter Zero - Mio Kouzuki.png");
				break;
		}
	}
}

void Character::update()
{
	for (unsigned int i = 0; i < characters.size(); i++)
	{// go through all characters

		/* create a tile to find the sprite we want in our tileset */
		Tile tile;

		CharactersID id = characters[i]->get_id();
		switch (id)
		{
		case goku:
			tile.setTile(0, 0, 42, 98);
			break;

		case vegeta:
			tile.setTile(0, 0, 50, 80);
			break;

		case miyo:
			tile.setTile(437, 2029, 68, 90);
		}
		surface[i]->setSpriteTexture(tile);
		surface[i]->setSpriteLocation(characters[i]->get_position().getPosition());
	}
	//cout << "leaving character update function\n";
}

void Character::setSurface(sf::RenderWindow& window)
{
	for (unsigned int i = 0; i < characters.size(); i++)
	{
		surface[i]->draw(window);
	}
	
}

//void Character::updateSelectedCharacter(unsigned int i)
//{
//	if (i < state.get_number_of_player())
//	{
//		auto state_temp = const_cast<GameState&> (state);
//		auto selected_character = state_temp.get_player(i)->get_current_character();
//
//		/* find the selected character among the character list */
//		int i = 0;
//		for (auto charac : characters)
//		{
//			if (charac == selected_character)
//				break;
//			i++;
//		}
//		/* create a tile to find the sprite we want in our tileset */
//		Tile tile, tileLeft, tileRight, tileUp, tileDown;
//
//		CharactersID id = selected_character->get_id();
//		switch (id)
//		{
//		case goku:
//			tile.setTile(0, 0, 42, 98);
//			tileLeft.setTile(634, 0, 94, 96);
//			tileRight.setTile(634, 0, 94, 96);
//			tileUp.setTile(224, 0, 89, 93);
//			tileDown.setTile(508, 288, 78, 77);
//			break;
//
//		case vegeta:
//			tile.setTile(0, 0, 50, 80);
//			break;
//		}
//		surface[i]->setSpriteLocation(selected_character->get_position().getPosition());
//	}
//}
