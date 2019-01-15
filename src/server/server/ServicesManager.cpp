/** 
 * @file ServicesManager.cpp
 * @author Philippe-Henri Gosselin
 * @date 9 décembre 2015
 * @copyright CNRS
 */

#include "ServicesManager.hpp"

using namespace std;
using namespace server;

void ServicesManager::registerService (unique_ptr<AbstractService> service) {
	throw ServiceException(HttpStatus::NOT_IMPLEMENTED,"Non implanté");
}

AbstractService* ServicesManager::findService (const string& url) const {
	throw ServiceException(HttpStatus::NOT_IMPLEMENTED,"Non implanté");
}

HttpStatus ServicesManager::queryService (string& out, const string& in, const string& url, const string& method) {
	if(url == "/version" && method == "GET")
	{
		Json::Value version;
		version["major"] = 1;
		version["minor"] = 0;
		string fin = version.toStyledString();
		out = fin;
		return(OK);
	}
	else
	{
		throw ServiceException(HttpStatus::NOT_IMPLEMENTED,"Non implanté mais t'est quand même arrivé là");
		return(BAD_REQUEST);
	}
}


