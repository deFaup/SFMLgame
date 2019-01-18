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
#include <SFML/Network.hpp>

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
void play_json(Json::Value* json_commandes, engine::GameEngine* gameEngine);
sf::Http::Response send(sf::Http& client, sf::Http::Request::Method type, const std::string& uri, Json::Value& request_body);
void connect_client();
void test_command(void);

// Global variables
std::string player_id; // we set this once we pushed a player in the PlayerDB
std::string player_name("Joueur 1");
int character_id (100);

void enginet(int player_1_type, int player_2_type)
{
	sf::RenderWindow renderWindow(sf::VideoMode(1102,869), "menu test");

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

	GameState etat;
	GameEngine engine(&etat); std::thread thread_engine;
	Controller controller(renderWindow, engine, etat);
	shared_ptr<Scene> scene = make_shared<Scene>(renderWindow, etat);

	/* Linking the observer to each observable */
	//in Characters::stats & position + Player + Map + GameState
	etat.registerObserver(scene);
	etat.map.registerObserver(scene);
	cout << "main: observers listed\n" << endl;
	
	/* Init game */
	init_game(&etat, player_1_type, player_2_type);
	//engine.init_game(player_2_type);
	
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

	while (renderWindow.isOpen())
	{
		renderWindow.display();

		// Process events
		sf::Event event;
		while (renderWindow.pollEvent(event))
			controller.handle_sfEvents(event);

		renderWindow.clear();
		while (engine.updating) {}
		scene->draw(); // wonder if we can get a segfault if engine.updating changes to true in between		
	}
	
	thread_engine.join();
	cout << "engine thread closed\n";

	if (ai_1) thread_ai_1.join();
	if (ai_2) thread_ai_2.join();
	cout << "ai thread closed\n";
}

void play()
{
	sf::RenderWindow renderWindow;

	GameState etat;
	GameEngine engine(&etat); std::thread thread_engine;
	Controller controller(renderWindow, engine, etat);
	shared_ptr<Scene> scene = make_shared<Scene>(renderWindow, etat);

	/* Linking the observer to each observable */
	//in Characters::stats & position + Player + Map + GameState
	etat.registerObserver(scene);
	etat.map.registerObserver(scene);
	cout << "main: observers listed\n" << endl;

	/* Init game */
	int real = REAL;
	init_game(&etat, real, real);
	scene->background.new_background_layer();
	scene->characters.new_character_layer();

	//laucnh engine thread
	//thread_engine = thread(&engine::GameEngine::workLoop, &engine);

	// get commands from JSON
	std::ifstream json_in_file;
	json_in_file.open(JSON_FILENAME);
	json_in_file >> global::json_commandes;
	json_in_file.close();

	// launch play json thread
	thread thread_json(play_json, &global::json_commandes, &engine);

	while (renderWindow.isOpen())
	{
		renderWindow.display();

		// Process events
		sf::Event event;
		while (renderWindow.pollEvent(event))
			controller.handle_sfEvents(event);

		renderWindow.clear();
		while (engine.updating) {}
		scene->draw(); // wonder if we can get a segfault if engine.updating changes to true in between		
	}

	thread_json.join();
	//thread_engine.join();
	cout << "engine thread closed\n";
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
		else if (strcmp(argv[1], "network") == 0)
		{
			thread th(connect_client);						
			th.join();
			//test_command();
		}
	}
	else if (argc == 4)
	{
		if (strcmp(argv[1], "network") == 0)
		{
			player_name = argv[2];
			int temp(atoi(argv[3]));
			if (temp != 100 || temp != 101 || temp != 200)
				character_id = 100;
			thread th(connect_client);
			th.join();
			//test_command();
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

void play_json(Json::Value* json_commandes, engine::GameEngine* gameEngine)
{
	// Iterate over the sequence elements.
	//json_commandes->size()
	Json::Value& json_cmd = *json_commandes;

	for (unsigned int index = 0; index < json_cmd["commandes"].size(); ++index)
	{
		Json::Value cmd = json_cmd["commandes"][index];
		
		state::sfEvents ev;
		ev.ID = static_cast<sfEventsID>(cmd["sfEventsID"].asInt());
		ev.mouse_position.increaseX( cmd["x"].asInt() );
		ev.mouse_position.increaseY( cmd["y"].asInt() );

		gameEngine->add_command(ev);
		gameEngine->check_stateID();
	}
}

sf::Http::Response send(sf::Http& client, sf::Http::Request::Method type, const std::string& uri, Json::Value& request_body)
{
	sf::Http::Request request;
	request.setMethod(type);
	request.setUri(uri);
	request.setHttpVersion(1, 1);
	if (type == sf::Http::Request::Post)
	{
		std::string temp = request_body.toStyledString();
		request.setBody(temp);
	}
	else
	{
		//request.setField("Content-Type", "application/x-www-form-urlencoded");
	}
	// Send the request
	sf::Http::Response response = client.sendRequest(request);

	return response;
}

void connect_client()
{
	// Create a new HTTP client
	sf::Http http("http://localhost", 8080);

	// requests & response
	Json::Value request_body;
	sf::Http::Response response;

	request_body["name"] = player_name; request_body["character"] = character_id;

	// record the player in PlayerDB
	response = send(http, sf::Http::Request::Post, "/TeamFormationService/player", request_body);
	std::cout << "wtf !!!" << endl;

	if (response.getStatus() == sf::Http::Response::Status::Ok)
	{
		{
			Json::Value id_temp;
			Json::Reader jsonReader;
			if (!jsonReader.parse(response.getBody(), id_temp))
				return;
			player_id = id_temp["id"].asString();
			std::cout << player_id << endl;
		}
		response = send(http, sf::Http::Request::Post, "/TeamFormationService/character", request_body);
		response = send(http, sf::Http::Request::Post, "/TeamFormationService/character", request_body);

		// all went well: show player DB
		response = send(http, sf::Http::Request::Get, "/TeamFormationService", request_body);
/*		//Pour livrable 4.2 mais useless
		// delete player name from PlayerDB
		cout << "Pressez <entrée> pour continuer (supprimer votre joueur du serveur)\n" << endl;
		cout << "Press Enter to Continue";
		//cin.ignore();
		send(http, sf::Http::Request::Post, "/TeamFormationService/delete_player", request_body);
		
		//show PlayerDB
		send(http, sf::Http::Request::Get, "/TeamFormationService", request_body);
*/
	}
}

void test_command(void)
{
	Json::Value JsonCmd;
	JsonCmd["sfEventsID"] = arrow_up;
	JsonCmd["x"] = 187;
	JsonCmd["y"] = 210;
	sf::Http http("http://localhost", 8080);
	send(http, sf::Http::Request::Post, "/GameStartedService/add_command", JsonCmd);
	JsonCmd["sfEventsID"] = arrow_down;
	JsonCmd["x"] = 145;
	JsonCmd["y"] = 368;
	send(http, sf::Http::Request::Post, "/GameStartedService/add_command", JsonCmd);
	//
	send(http, sf::Http::Request::Get, "/GameStartedService/get_command", JsonCmd);
}

/* le thread render/main aura une méthode class whatever pour afficher un menu dans renderwindow.
Dans le menu on choisit le nombre de joueurs et de personnages + autres paramètres si besoin.
Ensuite tous ces paramètres sont transmis au gameEngine qui va ensuite modifier le state.
Seul le moteur peut modifier l'état pour que cela marche en mode réseau.
La méthode menu sera appelé par l'engine au premier call de check state ID
*/

// test command to GameStartedService
//request_body["name"] = "Domingo"; request_body["character"] = 200;
//send(http, sf::Http::Request::Post, "/TeamFormationService/player", request_body);
//send(http, sf::Http::Request::Post, "/TeamFormationService/character", request_body);
//send(http, sf::Http::Request::Post, "/TeamFormationService/character", request_body);
//send(http, sf::Http::Request::Get, "/TeamFormationService", request_body);
//
//Json::Value sfjson;
//sfjson["sfEventsID"] = 101; 		sfjson["x"] = 20;		sfjson["y"] = 30;
//
//send(http, sf::Http::Request::Post, "/GameStartedService/add_command/player0", sfjson);
//send(http, sf::Http::Request::Get, "/GameStartedService/get_command/player0", request_body);
