#include <iostream>
#include <string>	//string

#include "define.hpp"
#include "global_mutex.hpp"

//#include "state.h"
//#include "engine.h"
#include <thread>
#include <SFML/Network.hpp>

sf::Http::Response send(int type, const std::string& uri, Json::Value& request_body);
bool send_command(Json::Value& request_body);
void connect_client();
void test_command(void);
void wait_game_to_start();
void add_player_and_get_id();
void add_character();
void show_playerDB();

sf::Http http("http://localhost", 8080);
//sf::Http http("10.10.26.128", 8080);

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
	sf::Http::Response response = http.sendRequest(request);

	return response;
}

bool send_command(Json::Value& request_body)
{
	sf::Http::Response response(send(POST, "/GameStartedService/add_command/", request_body));
	return (response.getStatus() != sf::Http::Response::Ok) ? false:true;
}

void connect_client()
{
	add_player_and_get_id();
	add_character();
	add_character();
	show_playerDB();
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

	response = send(GET, "TeamFormationService/start", request_body);

	Json::Value id_temp;
	Json::Reader jsonReader;

	if (!jsonReader.parse(response.getBody(), id_temp))
	{
		return;
	}

	int start_ok = id_temp["start"].asBool();
	std::cout << start_ok << "\n";

	if (!start_ok)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		std::cout << "not enough client to start game\n";
		wait_game_to_start();
	}

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

	request_body["name"] = global::player_name; request_body["characters"] = global::character_id;
	response = send(GET, "/TeamFormationService", request_body);
}
