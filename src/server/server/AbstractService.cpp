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

HttpStatus AbstractService::get (Json::Value& out) const {
    throw ServiceException(HttpStatus::NOT_IMPLEMENTED,"Non implanté");
}

HttpStatus AbstractService::post (const Json::Value& in) {
    throw ServiceException(HttpStatus::NOT_IMPLEMENTED,"Non implanté");
}

HttpStatus AbstractService::put (Json::Value& out, const Json::Value& in) {
    throw ServiceException(HttpStatus::NOT_IMPLEMENTED,"Non implanté");
}

