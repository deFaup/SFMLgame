#include "define.hpp"
#include "global_mutex.hpp"

#include "AI.h"
#include "engine.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

// threads, thread::sleep_for
#include <thread>
#include <chrono>

using namespace ai;

AI::AI(engine::GameEngine& moteur) : moteur(moteur) {}
AI::~AI() {}

void AI::place_character(engine::GameEngine& moteur)
{
	std::shared_ptr<state::Player> ia_player = moteur.etat->current_player;

	int width(0), height(0);
	moteur.etat->map.get_dimensions(width, height);
	srand(time(NULL));

	static bool aligned = false;
	if (!aligned)
	{
		for (unsigned i = 0; i < ia_player->get_number_of_characters(); i++) 
		// set characters randomly accross the top of the map
		{
			int nb_aleatoire = rand() % (width-10);
			for (int j = 0; j < nb_aleatoire; j++)
				moteur.add_command(state::sfEventsID::arrow_right);
		} 
		aligned = true;
	}

	// In this part we are going to move down each character one by one
	static state::Position previous_position; static unsigned int i = 0;
	previous_position = ia_player->get_current_character()->position;

	// Move down the current character of the AI
	//for (int i = 0; i != 5; i++)
	{
		state::sfEventsID arrow = state::sfEventsID::arrow_down;
		engine::Move move_commande(arrow);
		move_commande.execute(*(moteur.etat));
	}

	if (previous_position.getPositionY() == ia_player->get_current_character()->position.getPositionY())
	{
		state::sfEvents change_character(state::sfEventsID::arrow_up);
		moteur.add_command(change_character);
		i++;
	}

	if (i == ia_player->get_number_of_characters())
	{
		std::cout << "ia placement finished begin\n";
		next_player(moteur);
		i = 0;
		std::cout << "ia placement finished end\n";
	}
}

// when the engine is executed in another thread !!
void AI::next_player(engine::GameEngine& gameEngine) 
{
	state::sfEvents next_player(state::sfEventsID::enter);
	gameEngine.add_command(next_player);
	gameEngine.executeCommandes();

	// possible dead lock but not with the way above
	//std::unique_lock<std::mutex> unique_next_player(global::next_player);
	//global::next_player_cv.wait(unique_next_player);
}

void AI::workloop()
{
	while (!((moteur.etat->ID == state::StateID::end) || moteur.game_ended))
	{
		if (moteur.etat->current_player->name == this->name)
		{
			std::unique_lock<std::mutex> unique_get_engine(global::get_engine);
			play();
		}
		
		else//pause
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}
