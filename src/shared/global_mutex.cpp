// Include
#include "global_mutex.hpp"

namespace global
{
	std::mutex next_player;
	std::condition_variable next_player_cv;
}