#include <iostream>
#include <string>	//string
#include <thread>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "define.hpp"
#include "global_mutex.hpp"
#include "state.h"
#include "render.h"
#include "engine.h"
#include "ai.h"

static void RenderWindow_isOpen(sf::RenderWindow& renderWindow, engine::GameEngine& engine,
	render::Controller& controller, std::shared_ptr<render::Scene>& scene);
extern void menu();
static void registerObservers(state::GameState& etat, std::shared_ptr<render::Scene>& scene); 

sf::Http::Response send(int type, const std::string& uri, Json::Value& request_body);
bool send_command(Json::Value& request_body);
void connect_client();
void wait_game_to_start();
void game_network();

void test_command(void);
void add_player_and_get_id();
void add_character();
void show_playerDB();
void get_seed();
static void set_map_players_characters(state::GameState* gameState, const Json::Value& players);

/********** Global variables *************/
sf::Http http("http://localhost", 8080);
Json::Value team_info;

sf::Http::Response send(int type, const std::string& uri, Json::Value& request_body)
{
	sf::Http::Request request;
	request.setUri(uri);
	request.setHttpVersion(1, 1);
	
	std::string temp;

	switch (type)// == sf::Http::Request::Post)
	{
	case POST:
		request.setMethod(sf::Http::Request::Post);
		temp = request_body.toStyledString();
		request.setBody(temp);
		break;

	//request.setField("Content-Type", "application/x-www-form-urlencoded"); for get ? not useful sf do it automaticly
	case GET:
		request.setMethod(sf::Http::Request::Get);
		break;

	case PUT:
		request.setMethod(sf::Http::Request::Put);
		break;
	
	default:
		request.setMethod(sf::Http::Request::Delete);
		break;
	}

	// Send the request
	return http.sendRequest(request);
}
bool send_command(Json::Value& request_body)
{
	sf::Http::Response response(send(POST, "/GameStartedService/add_command/", request_body));
	return (response.getStatus() != sf::Http::Response::Ok) ? false:true;
}
Json::Value get_commands()
{
	Json::Value request_body;
	sf::Http::Response response(send(GET, "/GameStartedService/get_command/"+global::player_id, request_body));
	Json::Value JSONfile;
	Json::Reader jsonReader;

	if (!jsonReader.parse(response.getBody(), JSONfile))
	{
		std::cout << "can't parse in client.cpp get_commands()\n";
		return JSONfile;
	}
	return JSONfile;
}

void connect_client()
{
	add_player_and_get_id();
	add_character();
	add_character();
	show_playerDB();
	get_seed();
	std::cout << "client connected\n";

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
void wait_game_to_start()
{
	Json::Value request_body;
	sf::Http::Response response;
	Json::Value id_temp;
	Json::Reader jsonReader;
	int start_ok(0);
	// loop until we have reached the good number of players and characters (defined in TeamFormationService)
	while (!start_ok)
	{
		std::cout << "not enough client to start game\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		response = send(GET, "TeamFormationService/start", request_body);

		if (!jsonReader.parse(response.getBody(), id_temp))
			return;

		start_ok = id_temp["start"].asBool();
	}
	
	show_playerDB();	
}
void game_network()
{
	connect_client();
	wait_game_to_start();
	menu();

	sf::RenderWindow renderWindow;
	state::GameState etat;
	engine::GameEngine engine(&etat); engine.network_active = true; std::thread thread_engine;
	render::Controller controller(renderWindow, engine, etat);
	std::shared_ptr<render::Scene> scene = std::make_shared<render::Scene>(renderWindow, etat);

	registerObservers(etat, scene);
	set_map_players_characters(&etat, team_info);

	scene->background.new_background_layer();
	scene->characters.new_character_layer();

	thread_engine = std::thread(&engine::GameEngine::workLoop, &engine);
	RenderWindow_isOpen(renderWindow, engine, controller, scene);

	thread_engine.join();
	std::cout << "engine thread closed\n";
}

void test_command(void)
{
	global::player_name = "Domingo"; global::character_id = 200;
	add_player_and_get_id();
	add_character();
	add_character();
	show_playerDB();

	Json::Value request_body;
	sf::Http::Response response;
	request_body["name"] = global::player_name; request_body["characters"] = global::character_id;

	Json::Value sfjson;
	sfjson["sfEventsID"] = 101; 		sfjson["x"] = 20;		sfjson["y"] = 30;
	sfjson["id"] = global::player_id;

	send(POST, "/GameStartedService/add_command/", sfjson);
	send(GET, "/GameStartedService/get_command/" + global::player_id, request_body);
}

void add_player_and_get_id()
{
	sf::Http::Response response;
	Json::Value request_body;
	request_body["name"] = global::player_name; request_body["characters"] = global::character_id;

	response = send(POST, "/TeamFormationService/player", request_body);

	Json::Value id_temp;
	Json::Reader jsonReader;
	if (!jsonReader.parse(response.getBody(), id_temp))
		return;
	global::player_id = id_temp["id"].asString();
	std::cout << "player_id = " << global::player_id << "\n";
}
void add_character()
{
	sf::Http::Response response;
	Json::Value request_body;

	request_body["name"] = global::player_name; request_body["characters"] = global::character_id;
	response = send(POST, "/TeamFormationService/character", request_body);
}
void show_playerDB()
{
	sf::Http::Response response;
	Json::Value request_body;
	Json::Reader jsonReader;

	response = send(GET, "/TeamFormationService", request_body);
	if (!jsonReader.parse(response.getBody(), team_info))
		return;
	std::cout << team_info << "\n";
}
void get_seed()
{
	sf::Http::Response response;
	Json::Value request_body;

	request_body["name"] = global::player_name; request_body["characters"] = global::character_id;
	response = send(GET, "/seed", request_body);

	Json::Reader jsonReader;
	if (!jsonReader.parse(response.getBody(), request_body))
		return;

	global::server_seed = request_body["seed"].asUInt();
	global::rng.seed(global::server_seed);
	std::cout << "server_seed = " << global::server_seed << "\n";
}

// same as in GameStartedService
void set_map_players_characters(state::GameState* gameState, const Json::Value& players)
{
	/* Create players, characters and a map. Will be rewritten when menu is implemented */
	if (gameState->ID == state::StateID::not_started)
	{
		gameState->new_map(3000, 2000);

		int player_no(0);
		for (auto& elem : players["team"])
		{
			gameState->new_player(elem["name"].asString());
			for (auto& characters : elem["characters"])
			{
				gameState->new_character(player_no,
					static_cast<state::CharactersID>(characters.asInt()));
			}
			player_no++;
		}
		gameState->ID = state::StateID::team_selected;
	}
}

// same as in main.cpp
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
