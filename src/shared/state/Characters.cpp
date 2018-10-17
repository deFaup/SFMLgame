#include "state/Characters.h"

namespace state {

state::Characters::Characters(){
	return;
}

state::Characters::Characters(Statistics stats){
	this->stats = stats;
	return;
}

state::Characters::Characters(std::string name){
	this->name = name;
	if(name == "Vegeta"){
		this->stats.Set_statistiques(101,101,101);
	}
	return;
}

};
