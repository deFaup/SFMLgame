#include <iostream>
#include <string>	//string
#include <cstring>	//strcmp

#include "define.hpp"
#include "global_mutex.hpp"

#include "state.h"
#include "render.h"
#include "engine.h"
#include "ai.h"
#include <thread>
#include "../test/unit_test.hpp"

using namespace std;
using namespace state;
using namespace render;
using namespace engine;
using namespace ai;

// Les lignes suivantes ne servent qu'à vérifier que la compilation avec SFML fonctionne
#include <SFML/Graphics.hpp>

void testSFML() {
	sf::Texture texture;
	sf::Image image;
}
// Fin test SFML

/*************************************************************/
/*************************************************************/

void init_game(state::GameState* etat, int& player_1_type, int& player_2_type);
void RenderWindow_isOpen(
	sf::RenderWindow& renderWindow, GameEngine& engine, Controller& controller, shared_ptr<Scene>& scene);
void menu();
void registerObservers(state::GameState& etat, std::shared_ptr<render::Scene>& scene);

// mode: network (client.cpp)
extern void game_network();
extern void test_command(void);

// mode: play
void play();
void play_json(Json::Value* json_commandes, engine::GameEngine* gameEngine);

// mode: local
void enginet(int player_1_type, int player_2_type)
{
	// in local mode we set the seed here
	global::server_seed = std::random_device()();
	global::rng.seed(global::server_seed);
	std::cout << "server_seed = " << global::server_seed << "\n";
	int nb_aleatoire = global::dist6(global::rng);
	cout << nb_aleatoire << endl;
	nb_aleatoire = global::dist6(global::rng);
	cout << nb_aleatoire << endl;
	nb_aleatoire = global::dist6(global::rng);
	cout << nb_aleatoire << endl;
	nb_aleatoire = global::dist6(global::rng);
	cout << nb_aleatoire << endl;
	std::cout << "random = " << global::server_seed << "\n";

	menu();

	sf::RenderWindow renderWindow;
	GameState etat;
	GameEngine engine(&etat); std::thread thread_engine;
	Controller controller(renderWindow, engine, etat);
	shared_ptr<Scene> scene = make_shared<Scene>(renderWindow, etat);

	registerObservers(etat, scene);
	init_game(&etat, player_1_type, player_2_type);
	
	/* config AI */
	shared_ptr<AI> ai_1(0); std::thread thread_ai_1;
	shared_ptr<AI> ai_2(0); std::thread thread_ai_2;
	switch (player_1_type)
	{
	case REAL://this player is not an AI
		break;
	case RANDOM_AI:
		ai_1 = make_shared<RandomAI>(engine);
		break;
	case HEURISTIC_AI:
		ai_1 = make_shared<HyAI>(engine);
		break;
	case DEEP_AI:
		ai_1 = make_shared<DeepAI>(engine);
		break;
	default:
		break;
	}
	switch (player_2_type)
	{
	case REAL://this player is not an AI
		break;
	case RANDOM_AI:
		ai_2 = make_shared<RandomAI>(engine);
		break;
	case HEURISTIC_AI:
		ai_2 = make_shared<HyAI>(engine);
		break;
	case DEEP_AI:
		ai_2 = make_shared<DeepAI>(engine);
		break;
	default:
		break;
	}
	if (ai_1) ai_1->name = AI_1;
	if (ai_2) ai_2->name = AI_2;
	cout << "main: IA created" << endl;

	scene->background.new_background_layer();
	scene->characters.new_character_layer();

	thread_engine = thread(&engine::GameEngine::workLoop, &engine);
	if (ai_1)
		thread_ai_1 = thread(&ai::DeepAI::workloop, ai_1);	
	if (ai_2)
		thread_ai_2 = thread(&ai::DeepAI::workloop, ai_2);

	RenderWindow_isOpen(renderWindow, engine, controller, scene);

	thread_engine.join();
	cout << "engine thread closed\n";

	if (ai_1) thread_ai_1.join();
	if (ai_2) thread_ai_2.join();
	cout << "ai thread closed\n";
}


int main(int argc, char* argv[])
{
	if (argc == 2)
	{
		if (strcmp(argv[1], "hello") == 0)
			cout << "Bienvenue chez antoine et gregoire !" << endl;

		else if (strcmp(argv[1], "state") == 0)
		{
			//testStatistics();
			//testCharacters();
			//testPlayer();
			//testGameState();
		}
		else if (strcmp(argv[1], "render") == 0)
		{
			testSFML();
			//rendering();
			//render_state();
		}
		else if (strcmp(argv[1], "engine") == 0)	enginet(REAL, REAL);
		else if (strcmp(argv[1], "random_ai") == 0)	enginet(REAL, RANDOM_AI);
		else if (strcmp(argv[1], "heuristic_ai") == 0)	enginet(REAL, HEURISTIC_AI);
		else if (strcmp(argv[1], "deep_ai") == 0)	enginet(REAL, DEEP_AI);
		else if (strcmp(argv[1], "rollback") == 0)	enginet(HEURISTIC_AI, HEURISTIC_AI);
		else if (strcmp(argv[1], "test") == 0)
		{
			result_statistics();
			result_position();
		}
		else if (strcmp(argv[1], "thread") == 0) enginet(HEURISTIC_AI, HEURISTIC_AI);
		else if (strcmp(argv[1], "play") == 0) play();
		else if (strcmp(argv[1], "network") == 0) game_network();
	}

	else if (argc == 3)
	{
		if (strcmp(argv[1], "network") == 0 && strcmp(argv[2], "test") == 0)
		{
			test_command();
		}
	}
	else if (argc == 4)
	{
		if (strcmp(argv[1], "network") == 0)
		{ //ex: "bin/client my_name_in_the_game 100"
			global::player_name = argv[2];
			
			int temp(atoi(argv[3]));
			std::cout << "character_id = " << temp << endl;
			if (temp != 100 && temp != 101 && temp != 200)
			{
				global::character_id = 100;
			}
			game_network();
		}
	}
	return 0;
}

/* Init the game with two players: player can be AI or real */
void init_game(state::GameState* etat, int& player_1_type, int& player_2_type)
{
	global::rng.seed(std::random_device()());

	/* Create players, characters and a map. Will be rewritten when menu is implemented */
	if (etat->ID == not_started)
	{
		etat->new_map(3000, 2000);

		if (player_1_type == REAL) etat->new_player("Joueur 1");
		else etat->new_player(AI_1);
		
		if (player_2_type == REAL) etat->new_player("Joueur 2");
		else etat->new_player(AI_2);

		etat->new_character(0, vegeta);
		etat->new_character(0, vegeta);
		etat->new_character(0, vegeta);

		etat->new_character(1, goku);
		etat->new_character(1, goku);
		etat->new_character(1, goku);
		
		etat->ID = team_selected;
	}
}

void menu()
{
	sf::RenderWindow renderWindow(sf::VideoMode(1102, 869), "menu test");

	/* Menu begin */
	sf::Texture pic;
	pic.loadFromFile("res/menu.png");
	sf::Sprite menu_sprite(pic);

	bool Return_key_pressed(false);
	while (!Return_key_pressed)
	{
		sf::Event event;
		while (renderWindow.pollEvent(event))
		{
			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Return)
					Return_key_pressed = true;
			}
		}
		renderWindow.draw(menu_sprite);
		renderWindow.display();
		renderWindow.clear();
	}
	/* Menu end */
}

inline void RenderWindow_isOpen(sf::RenderWindow& renderWindow, engine::GameEngine& engine,
	render::Controller& controller, std::shared_ptr<render::Scene>& scene)
{
	while (renderWindow.isOpen())
	{
		renderWindow.display();
		sf::Event event;
		while (renderWindow.pollEvent(event))
			controller.handle_sfEvents(event);

		renderWindow.clear();
		while (engine.updating) {}
		scene->draw(); // wonder if we can get a segfault if engine.updating changes to true in between		
	}
}
inline void registerObservers(state::GameState& etat, std::shared_ptr<render::Scene>& scene)
{
	/* Linking the observer to each observable */
	//in Characters::stats & position + Player + Map + GameState
	etat.registerObserver(scene);
	etat.map.registerObserver(scene);
	std::cout << "main: observers listed\n\n";
}

/************** PLAY functions (done after server RECORD) **************/
void play()
{
	sf::RenderWindow renderWindow;

	state::GameState etat;
	engine::GameEngine engine(&etat); std::thread thread_engine;
	render::Controller controller(renderWindow, engine, etat);
	std::shared_ptr<render::Scene> scene = std::make_shared<render::Scene>(renderWindow, etat);

	registerObservers(etat, scene);

	/* Init game */
	int real = REAL;
	init_game(&etat, real, real);
	scene->background.new_background_layer();
	scene->characters.new_character_layer();

	// get commands from JSON
	std::ifstream json_in_file;
	json_in_file.open(JSON_FILENAME);
	json_in_file >> global::json_commandes;
	json_in_file.close();

	// launch play json thread
	std::thread thread_json(play_json, &global::json_commandes, &engine);

	RenderWindow_isOpen(renderWindow, engine, controller, scene);

	thread_json.join();
	std::cout << "json thread closed\n";
}
void play_json(Json::Value* json_commandes, engine::GameEngine* gameEngine)
{
	// Iterate over the sequence elements.
	//json_commandes->size()
	Json::Value& json_cmd = *json_commandes;

	for (unsigned int index = 0; index < json_cmd["commandes"].size(); ++index)
	{
		Json::Value cmd = json_cmd["commandes"][index];

		state::sfEvents ev;
		ev.ID = static_cast<state::sfEventsID>(cmd["sfEventsID"].asInt());
		ev.mouse_position.increaseX(cmd["x"].asInt());
		ev.mouse_position.increaseY(cmd["y"].asInt());

		gameEngine->add_command(ev);
		gameEngine->check_stateID();
	}
}
