/* Includes */
#include <iostream>
#include "define.hpp"
#include "Scene.h"

using namespace std;
using namespace render;

Scene::Scene(const state::GameState& state, const state::Map& map) : state(state), background(state, map)
{
	cout << "Scene created" << endl;
}
