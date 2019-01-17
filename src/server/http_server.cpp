
#include <iostream> // cerr
#include <sstream>
#include <microhttpd.h>
#include <string>	//string
#include <cstring>	//strcmp
#include "server.h"

using namespace std;
using namespace server;

class Request {
public:
	struct MHD_PostProcessor *pp = nullptr;
	string data;
	~Request() {
		if (pp) MHD_destroy_post_processor(pp);
	}
};

// Fonction pour g�rer les donn�es imporantes en upload (non implant� ici)
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

// D�truit les donn�es d'une requ�te
static void
request_completed(void *cls, struct MHD_Connection *connection,
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
main_handler(void *cls,
	struct MHD_Connection *connection,
	const char *url, // 
	const char *method,
	const char *version,
	const char *upload_data, size_t *upload_data_size, void **ptr)
{
	// Donn�es pour une requ�te (en plusieurs appels � cette fonction)
	Request *request = (Request*)*ptr;

	// Premier appel pour cette requ�te
	if (!request) {
		request = new Request();
		if (!request) {
			return MHD_NO;
		}
		*ptr = request;
		if (strcmp(method, MHD_HTTP_METHOD_POST) == 0
			|| strcmp(method, MHD_HTTP_METHOD_PUT) == 0) {
			request->pp = MHD_create_post_processor(connection, 1024, &post_iterator, request);
			if (!request->pp) {
				std::cerr << "Failed to setup post processor for " << url << endl;
				return MHD_NO;
			}
		}
		return MHD_YES;
	}

	// Cas o� il faut r�cup�rer les donn�es envoy�s par l'utilisateur
	if (strcmp(method, MHD_HTTP_METHOD_POST) == 0
		|| strcmp(method, MHD_HTTP_METHOD_PUT) == 0) {
		MHD_post_process(request->pp, upload_data, *upload_data_size);
		if (*upload_data_size != 0) {
			request->data = upload_data;
			*upload_data_size = 0;
			return MHD_YES;
		}
	}

	HttpStatus status;
	string response;
	try {

		ServicesManager *manager = (ServicesManager*)cls;
		status = manager->queryService(response, request->data, url, method);
	}
	catch (ServiceException& e) {
		status = e.status();
		response = e.what();
		response += "\n";
	}
	catch (exception& e) {
		status = HttpStatus::SERVER_ERROR;
		response = e.what();
		response += "\n";
	}
	catch (...) {
		status = HttpStatus::SERVER_ERROR;
		response = "Unknown exception\n";
	}

	struct MHD_Response *mhd_response;
	mhd_response = MHD_create_response_from_buffer(response.size(), (void *)response.c_str(), MHD_RESPMEM_MUST_COPY);
	if (strcmp(method, MHD_HTTP_METHOD_GET) == 0) {
		MHD_add_response_header(mhd_response, "Content-Type", "application/json; charset=utf-8");
	}
	int ret = MHD_queue_response(connection, status, mhd_response);
	MHD_destroy_response(mhd_response);
	return ret;
}

int server_listen(std::shared_ptr<server::GameServer> gameServer, uint16_t port)
{
	try {
		ServicesManager servicesManager;
		{
			std::shared_ptr<server::AbstractService> game_service = std::make_shared<GameStartedService>();
			game_service->gameServer = gameServer;
			servicesManager.registerService(game_service);

			auto team = std::make_shared<TeamFormationService>();
			team->gameServer = gameServer;
			team->service_gameStarted = (std::static_pointer_cast<GameStartedService>) (game_service);

			std::shared_ptr<server::AbstractService> team_service = team;
			servicesManager.registerService(team_service);
		}
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
		cout << "Pressez <entr�e> pour arr�ter le serveur" << endl;
		(void)getc(stdin);
		MHD_stop_daemon(d);
	}
	catch (exception& e) {
		cerr << "Exception: " << e.what() << endl;
	}

	return 0;
}
