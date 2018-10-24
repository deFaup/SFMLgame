#include "define.hpp"
#include "state.h"
#include "Layer.h"
#include <SFML/Graphics.hpp>

using namespace render;
using namespace state;

sf::RenderWindow fenetre(sf::VideoMode(500, 500), "test affichage");

sf::RenderWindow& Layer::fenetre = fenetre;

Layer::Layer(){

}
