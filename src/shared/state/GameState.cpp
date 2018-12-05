#include <iostream>
#include <stdexcept>

#include "define.hpp"
#include "GameState.h"

using namespace state;
using namespace std;

GameState::GameState()
{
	cout << "GameState created" << endl;
	ID = not_started;
}
//	
GameState::~GameState()
{
	cout << "GameState deleteted" << endl;
}

/* Create a mask in the map object */
void GameState::new_map(unsigned const int width, unsigned const int height)
{
	if (width == 0 && height == 0) // default parameters of the function
		map.set_screen_dimensions();
    
	else
		map.set_dimensions(width, height);

    map.create_mask ();
	// notify map's observer EventMap that the mask has changed
	//notifyObservers(EventMap(Map_maskChanged));
}

/* create a new instance of Player */
void GameState::new_player(const std::string name)
{
	if(get_number_of_player() < MAX_NB_PLAYER)
	{
		shared_ptr<Player> ptr = make_shared<Player>(name);
		players.push_back(ptr);
		current_player = *(players.begin());
	}
}

/* create a new character for player i */
void GameState::new_character(const unsigned int player_id, const CharactersID character_id)
{
	if (player_id > get_number_of_player())
		throw std::invalid_argument("in GameState::new_character player_id is not valid");
	
	shared_ptr<Characters> ptr(0);
	ptr = get_player(player_id)->new_character(character_id);
	characters.push_back(ptr);
}

/* return the number of Player's instances */
unsigned int GameState::get_number_of_player() const{ return(players.size()); }

/* return a reference (not const) to a player */
shared_ptr<Player>& GameState::get_player (unsigned int i)
{
	if (i >= get_number_of_player()) {
		throw std::invalid_argument("you can't access to a non existent player");
	}
	else
		return players[i];
}

/* return a const reference to a player */
const shared_ptr<Player>& GameState::get_player(unsigned int i) const
{
	if (i >= get_number_of_player())
		throw std::invalid_argument("you can't access to a non existent player");
	else
		return players[i];
}

/* return a reference to the characters list */
const vector<shared_ptr<Characters>> GameState::get_characters() const {  return characters;  }

/* Limit the maximum number of characters the players can choose
	The range is also limited by MAX_NB_PLAYER in define.hpp */
void GameState::set_characters_range(unsigned int range) { Player::set_characters_range(range); }

void GameState::delete_character(Characters* character_to_delete)
{
	if ( !characters.empty() ) // if the vector is not empty we can delete something
	{
		/* find the shared ptr of the dead character in the GameState characters vector */
		int index = 0; 
		for (auto charac : characters)
		{
			if (charac.get() == character_to_delete)
			{
				characters.erase(characters.begin() + index);
				cout << "character " << index << " erased in gamestate\n";
				break;
			}
			index++;
		}
	}

	else
		throw std::runtime_error("GameState.cpp in delete_character, deleting a character but there are no characters");

}

void GameState::delete_player(Player* player_to_delete)
{
	if (players.size() > 1) // at least two players
	{
		/* find the shared ptr of the dead player in the GameState players vector */
		unsigned int index(0);
		for (auto player : players)
		{
			if (player.get() == player_to_delete)
			{
				players.erase(players.begin() + index);

				if (current_player == player) // player_to_delete was the current_player an died
					current_player = players[(index == players.size()) ? 0 : index];

				cout << "player " << index << " erased in gamestate\n";
				break;
			}
			index++;
		}

		if (players.size() == 1)
			ID = StateID::end;
	}

	else if (players.size() == 1)
		throw std::runtime_error("GameState.cpp in delete_player, can not delete the last player");

	else
		throw std::runtime_error("GameState.cpp in delete_player, no player to delete");

}