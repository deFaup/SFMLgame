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

Character::Character(const state::GameState& state) : state(state), characters()//state.get_characters()) 
{}

void Character::new_character_layer()
{
	characters = state.get_characters();

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
	cout << "Character created\n" << endl;
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
		surface[i]->setSpriteLocation(characters[i]->position);
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

void Character::update_one_character(Characters* char_to_update)
{
	/* find the position of the given character among the list of characters for the current GameState */
	int i = 0;
	for (auto charac : characters)
	{
		if (charac.get() == char_to_update)
			break;
		i++;
	}

	/* create a tile to find the sprite we want in our tileset */ // to be deprecated
	Tile tile; // , tileLeft, tileRight, tileUp, tileDown;

	CharactersID id = characters[i]->get_id();
	switch (id)
	{
	case goku:
		tile.setTile(0, 0, 42, 98);
		//tileLeft.setTile(634, 0, 94, 96);
		//tileRight.setTile(634, 0, 94, 96);
		//tileUp.setTile(224, 0, 89, 93);
		//tileDown.setTile(508, 288, 78, 77);
		break;

	case vegeta:
		tile.setTile(0, 0, 50, 80);
		break;
	case miyo:
		tile.setTile(437, 2029, 68, 90);
		break;

	default:
		break;
	}
	surface[i]->setSpriteTexture(tile);
	surface[i]->setSpriteLocation(characters[i]->position);
}

void Character::delete_character(Characters* character_to_delete)
{
	if (!characters.empty()) //if there is more than one character
	{
		/* find the shared ptr of the dead character in the GameState characters vector */
		int index = 0;
		for (auto charac : characters)
		{
			if (charac.get() == character_to_delete)
			{
				break;
			}
			index++;
		}

		tileset.erase(tileset.begin() + index);
		surface.erase(surface.begin() + index);
		characters = state.get_characters();
		cout << index << " erased in render character\n";
	}

	else
		throw std::runtime_error("render::Character.cpp in delete_character, deleting a character but there are no characters");
	
}