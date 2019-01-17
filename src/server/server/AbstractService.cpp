/** 
 * @file AbstractService.cpp
 * @author Philippe-Henri Gosselin
 * @date 9 décembre 2015
 * @copyright CNRS
 */

#include "AbstractService.hpp"

using namespace std;
using namespace server;

AbstractService::AbstractService () {
}

AbstractService::AbstractService (const string& pattern) {
    setPattern(pattern);
}

AbstractService::~AbstractService() {
    
}

const string& AbstractService::getPattern () const {
    return pattern;
}

void AbstractService::setPattern (const string& pattern) {
    this->pattern = pattern;
}

HttpStatus AbstractService::get (const string& url, Json::Value& out) const {
    throw ServiceException(HttpStatus::NOT_IMPLEMENTED,"Non implanté");
}

HttpStatus AbstractService::post (const string& url, const Json::Value& in, Json::Value& out) {
    throw ServiceException(HttpStatus::NOT_IMPLEMENTED,"abstract service post");
}

HttpStatus AbstractService::put (Json::Value& out, const Json::Value& in) {
    throw ServiceException(HttpStatus::NOT_IMPLEMENTED,"Non implanté");
}

