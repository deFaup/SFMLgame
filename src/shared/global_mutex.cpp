// Include
#include "global_mutex.hpp"

namespace global
{
	std::mutex next_player;
	std::condition_variable next_player_cv;

	std::mutex get_engine;
	//std::condition_variable get_engine_cv;

	Json::Value json_commandes;

	std::mt19937 rng;
	std::uniform_int_distribution<std::mt19937::result_type> dist6(20, 350);
}