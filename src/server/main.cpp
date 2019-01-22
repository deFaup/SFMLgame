#include <iostream>
#include <string>	//string
#include <cstring>	//strcmp

#include "define.hpp"
#include "global_mutex.hpp"

#include "state.h"
#include "server.h"
#include "engine.h"
#include "ai.h"
#include <thread>

//using namespace state;
//using namespace engine;
//using namespace ai;
//using namespace server;

extern int server_listen(std::shared_ptr<server::GameServer> gameServer, uint16_t port = 8080);
extern void record(int player_1_type, int player_2_type);
int begin(uint16_t port = 8080);

int main(int argc, char* argv[])
{
	if (argc == 2)
	{
		if (strcmp(argv[1], "record") == 0)
			record(HEURISTIC_AI, HEURISTIC_AI);

		else if (strcmp(argv[1], "listen") == 0)
			return (begin());
	}
	else if (argc == 3 && (strcmp(argv[1], "listen") == 0))
		return (begin(atoi(argv[2])));

    return 0;
}



int begin(uint16_t port)
{
	state::GameState gameState;
	engine::GameEngine gameEngine(&gameState);

	// create a GameServer
	std::shared_ptr<server::GameServer> gameServer = std::make_shared<server::GameServer>(&gameState, &gameEngine);

	/* Linking the observer to each observable */
	//in Characters::stats & position + Player + GameState
	gameState.registerObserver(gameServer);

	std::thread thread_engine(&engine::GameEngine::workLoop, &gameEngine);

	// launch the http server in a thread
	// create services and set them with a pointer to gameServer
	std::thread thread_server_listen(server_listen, gameServer, port);

	thread_engine.join();
	std::cout << "server engine thread closed\n";

	thread_server_listen.join();
	return 0;
}