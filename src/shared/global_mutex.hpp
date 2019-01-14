#ifndef GLOBAL_MUTEX_H
#define GLOBAL_MUTEX_H

#include <mutex>
#include <condition_variable>

// pour l'export sous format json
#include <fstream>
#include <json/json.h>

namespace global
{
	extern std::mutex next_player;
	extern std::condition_variable next_player_cv;

	extern std::mutex get_engine;

	extern Json::Value json_commandes;
}

#endif