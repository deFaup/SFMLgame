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

#include <sstream>
#include <microhttpd.h>

using namespace std;
using namespace state;
using namespace engine;
using namespace ai;
using namespace server;

class Request {
public:
    struct MHD_PostProcessor *pp = nullptr;
    string data;
    ~Request() {
        if (pp) MHD_destroy_post_processor (pp);
    }
};

// Fonction pour gérer les données imporantes en upload (non implanté ici)
static int
post_iterator(void *cls,
        enum MHD_ValueKind kind,
        const char *key,
        const char *filename,
        const char *content_type,
        const char *transfer_encoding,
        const char *data, uint64_t off, size_t size) 
{
    return MHD_NO;
}

// Détruit les données d'une requête
static void
request_completed (void *cls, struct MHD_Connection *connection,
                   void **con_cls, enum MHD_RequestTerminationCode toe)
{
  Request *request = (Request*)*con_cls;
  if (request) {
      delete request;
      *con_cls = nullptr;
  }
}

// Gestionnaire principal
static int
main_handler (void *cls,      
          struct MHD_Connection *connection,
          const char *url, // 
          const char *method,
          const char *version,
          const char *upload_data, size_t *upload_data_size, void **ptr) 
{
    // Données pour une requête (en plusieurs appels à cette fonction)
    Request *request = (Request*)*ptr;

    // Premier appel pour cette requête
    if (!request) { 
        request = new Request();
        if (!request) {
            return MHD_NO;
        }
        *ptr = request;
        if (strcmp(method, MHD_HTTP_METHOD_POST) == 0
         || strcmp(method, MHD_HTTP_METHOD_PUT) == 0) {
            request->pp = MHD_create_post_processor(connection,1024,&post_iterator,request);
            if (!request->pp) {
                cerr << "Failed to setup post processor for " << url << endl;
                return MHD_NO;
            }
        }
        return MHD_YES;
    }    
    
    // Cas où il faut récupérer les données envoyés par l'utilisateur
    if (strcmp(method, MHD_HTTP_METHOD_POST) == 0
     || strcmp(method, MHD_HTTP_METHOD_PUT) == 0) {
        MHD_post_process(request->pp,upload_data,*upload_data_size);
        if (*upload_data_size != 0) {
            request->data = upload_data;
            *upload_data_size = 0;
            return MHD_YES;
        }    
    }

    HttpStatus status;
    string response;
    try {

        ServicesManager *manager = (ServicesManager*) cls;
        status = manager->queryService(response,request->data,url,method);
    }
    catch(ServiceException& e) {
        status = e.status();
        response = e.what();
        response += "\n";
    }
    catch(exception& e) {
        status = HttpStatus::SERVER_ERROR;
        response = e.what();
        response += "\n";
    }
    catch(...) {
        status = HttpStatus::SERVER_ERROR;
        response = "Unknown exception\n";
    }

    struct MHD_Response *mhd_response;
    mhd_response = MHD_create_response_from_buffer(response.size(),(void *)response.c_str(),MHD_RESPMEM_MUST_COPY);
    if (strcmp(method,MHD_HTTP_METHOD_GET) == 0) {
        MHD_add_response_header(mhd_response,"Content-Type","application/json; charset=utf-8");
    }
    int ret = MHD_queue_response(connection, status, mhd_response);
    MHD_destroy_response(mhd_response);
    return ret;
}


int server_listen(uint16_t port = 8080);
void init_game(state::GameState* etat, int& player_1_type, int& player_2_type);
void enginet(int player_1_type, int player_2_type);

int main(int argc, char* argv[])
{

	if (argc == 2)
	{
		if (strcmp(argv[1], "record") == 0)
			enginet(HEURISTIC_AI, HEURISTIC_AI);

		else if (strcmp(argv[1], "listen") == 0)
			return (server_listen());
	}
	else if (argc == 3 && (strcmp(argv[1], "listen") == 0))
		return (server_listen(atoi(argv[2])));

    return 0;
}

int server_listen(uint16_t port)
{
	try {
		ServicesManager servicesManager;

		std::unique_ptr<server::AbstractService> team_service = std::make_unique<TeamFormationService>();
		servicesManager.registerService(std::move(team_service));

		std::unique_ptr<server::AbstractService> game_service = std::make_unique<GameStartedService>();
		servicesManager.registerService(std::move(game_service));

		struct MHD_Daemon *d;

		d = MHD_start_daemon(// MHD_USE_SELECT_INTERNALLY | MHD_USE_DEBUG | MHD_USE_POLL,
			MHD_USE_SELECT_INTERNALLY | MHD_USE_DEBUG,
			// MHD_USE_THREAD_PER_CONNECTION | MHD_USE_DEBUG | MHD_USE_POLL,
			// MHD_USE_THREAD_PER_CONNECTION | MHD_USE_DEBUG,
			port,
			NULL, NULL,
			&main_handler, (void*)&servicesManager,
			MHD_OPTION_NOTIFY_COMPLETED, request_completed, NULL,
			MHD_OPTION_END);
		if (d == NULL)
			return 1;
		cout << "Pressez <entrée> pour arrêter le serveur" << endl;
		(void)getc(stdin);
		MHD_stop_daemon(d);
	}
	catch (exception& e) {
		cerr << "Exception: " << e.what() << endl;
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

void enginet(int player_1_type, int player_2_type)
{
	GameState etat;
	GameEngine engine(&etat); std::thread thread_engine;
	shared_ptr<server::GameServer> m_server = make_shared<server::GameServer>(&etat, &engine);

	/* Linking the observer to each observable */
	//in Characters::stats & position + Player + GameState
	etat.registerObserver(m_server);
	cout << "main: observers listed\n" << endl;

	/* Init game */
	init_game(&etat, player_1_type, player_2_type);
	std::ofstream json_out_file;
	json_out_file.open(JSON_FILENAME);
	engine.JSONActive = true;

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

	thread_engine = thread(&engine::GameEngine::workLoop, &engine);
	if (ai_1)
		thread_ai_1 = thread(&ai::DeepAI::workloop, ai_1);
	if (ai_2)
		thread_ai_2 = thread(&ai::DeepAI::workloop, ai_2);

	thread_engine.join();
	cout << "engine thread closed\n";

	if (ai_1) thread_ai_1.join();
	if (ai_2) thread_ai_2.join();
	cout << "ai thread closed\n";

	/* Game is over */
	engine.JSONActive = false;
	json_out_file << global::json_commandes;
	json_out_file.close();
}
