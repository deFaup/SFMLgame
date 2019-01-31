// Include
#include "global_mutex.hpp"

namespace global
{
	std::mutex next_player;
	std::condition_variable next_player_cv;

	std::mutex get_engine;

	/* json */
	Json::Value json_commandes;

	//std::mt19937 rng;
	//std::uniform_int_distribution<std::mt19937::result_type> dist6(20, 350);
	unsigned int server_seed;

	/* Network variables */
	std::string player_id; // we set this once we pushed a player in the PlayerDB
	std::string player_name("Joueur 1");
	int character_id(100);

}