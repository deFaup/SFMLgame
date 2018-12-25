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

// copy constructor : observers are not copied
GameState::GameState(GameState& gameState) : 
	ID(gameState.ID.load(std::memory_order_relaxed)), map(gameState.map)
{
	cout << "begin copy constructor\n";
	// unregister observers pour toutes les observables
	//empty as by inheritance from Observable the vec is empty
	current_player = 0;

	for (unsigned int i = 0; i < gameState.get_number_of_player(); i++)
	{
		std::shared_ptr<Player> original_player = gameState.get_player(i);
		new_player(original_player->name);
		std::shared_ptr<Player> player_copy = get_player(i);
		cout << "original player ptr " << original_player.get() << "\n";
		cout << "copy player ptr " << player_copy.get() << "\n";

		current_player = (gameState.current_player == original_player) ? player_copy:current_player;

		for (unsigned int j = 0; j < gameState.get_player(i)->get_number_of_characters(); j++)
		{
			std::shared_ptr<Characters> original_character = original_player->get_character(j);
			new_character(i, original_character->get_id());
			std::shared_ptr<Characters> character_copy = player_copy->get_character(j);

			player_copy->current_character = (original_player->current_character == original_character) ?
					character_copy:player_copy->current_character;

			/* set ID */
			character_copy->id = original_character->id;
				
				// set Position
			character_copy->position.setPosition(original_character->position.getPositionX(), original_character->position.getPositionY());
				
			// set Statistics
			character_copy->stats.set_statistics(original_character->stats);

			// set Attack
			for (unsigned int k = 0; k < original_character->get_number_of_attacks(); k++)
				character_copy->addAttack(original_character->get_attack(k));
		}

		// set current character
		//player_copy->current_character = original_player->current_character;
	} 
	
	// set current player
	//current_player = gameState.current_player;
	// modifier les current player et character car on écrit le shared ptr du state et non du copié!
	cout << "end copy constructor\n";
}

GameState::~GameState()
{
	cout << "GameState deleted" << endl;
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
		for(auto obs: this->observers)
			ptr->registerObserver(obs);

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
				// cout << "character " << index << " erased in gamestate\n";
				break;
			}
			index++;
		}
	}

	else
		throw std::runtime_error("GameState.cpp in delete_character, deleting a character but there are no characters");

}

//delete a player and set the GameState ID to end when there is only one player left.
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
				cout << player->name << ". Begin\n";
				players.erase(players.begin() + index);

				if (current_player == player) // player_to_delete was the current_player and died
					current_player = players[(index == players.size()) ? 0 : index];
				// if index == players.size we our of vector boundaries so we set 0

				break;
			}
			index++;
		}

		if (players.size() == 1)
			ID = StateID::end;
	}

	else if (players.size() == 1) {
		cout << "can't delete last player for now. sorry :(\n";
		//throw std::runtime_error("GameState.cpp in delete_player, can not delete the last player");
	}

	else
		throw std::runtime_error("GameState.cpp in delete_player, no player to delete");

}