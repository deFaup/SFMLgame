#include "define.hpp"
#include "Command.h"
#include <iostream>

using namespace engine;
using namespace std;

Command::Command(){
}

int Command::isLegit(state::GameState& etat){return(-1);}
void Command::execute(state::GameState& etat){return;}

/*Command::Command(CommandeType command_type) : command_type(command_type){
}
*/

