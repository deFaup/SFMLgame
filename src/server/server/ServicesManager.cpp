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
		Json::Reader jsonReader;
		Json::Value json_in;
		if (!jsonReader.parse(in, json_in))
			throw ServiceException(HttpStatus::BAD_REQUEST, "Données invalides: " + jsonReader.getFormattedErrorMessages());

		status = service->post(url, json_in);
	}
	else if (method == "PUT")
	{
		Json::Value json_in;
		Json::Value json_out;

		status = service->put(json_out, json_in);
	}
	else
	{
		throw ServiceException(HttpStatus::NOT_IMPLEMENTED, "HTML request unknown to Service Manager\n");
		return(BAD_REQUEST);
	}

	return status;
}


