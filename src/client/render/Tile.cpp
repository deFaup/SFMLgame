#include "Tile.h"
using namespace render;

Tile::Tile(): x(0), y(0), width(0), height(0) {}
Tile::Tile(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}

void Tile::setTile(int x, int y, int width, int height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}