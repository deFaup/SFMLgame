/** 
 * @file ServicesManager.cpp
 * @author Philippe-Henri Gosselin
 * @date 9 décembre 2015
 * @copyright CNRS
 */

#include "ServicesManager.hpp"

using namespace std;
using namespace server;

void ServicesManager::registerService(unique_ptr<AbstractService> service) {
	services.push_back(std::move(service));
}

AbstractService* ServicesManager::findService(const string& url) const 
{
	for (auto& service : services) 
	{
		const string& pattern(service->getPattern());

		if (url.find(pattern) == 0) //if we find the pattern at position 1 (as url is '/version')
		{
			if ((url.size() > pattern.size()) && url[pattern.size()] != '/') // but what if they have the same beginning
				continue;
			return service.get();
		}
		else
			continue;
	}
	return nullptr;
}

HttpStatus ServicesManager::queryService (string& out, const string& in, const string& url, const string& method) 
{
	// find the corresponding service
	AbstractService* service = findService(url);
	if (!service)
		throw ServiceException(HttpStatus::NOT_FOUND, "no service for url: "+ url);

	/* error cases */
	// ?

	HttpStatus status;
	if (method == "GET")
	{
		Json::Value version;
		status = service->get(url, version);
		out = version.toStyledString();
	}
	else if (method == "POST")
	{
		Json::Value json_in;
		status = service->post(url, json_in);
	}
	else if (method == "PUT")
	{
		Json::Value json_in;
		Json::Value json_out;

		status = service->put(json_out, json_in);
	}
	// autres méthodes?
	else
	{
		throw ServiceException(HttpStatus::NOT_IMPLEMENTED, "Non implanté mais t'est quand même arrivé là");
		return(BAD_REQUEST);
	}

	return status;
	//if(url == "/version" && method == "GET")
	//{
	//	Json::Value version;
	//	version["major"] = 1;
	//	version["minor"] = 0;
	//	string fin = version.toStyledString();
	//	out = fin;
	//}

}


