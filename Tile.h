#ifndef TILE_INCL
#define TILE_INCL

class Tile
{
public:
	Tile(int x, int y, int height, int width, int type, bool active);
	int x;
	int y;
	int height;
	int width;
	int type;
	bool active;
};

Tile::Tile(int x, int y, int height, int width, int type, bool active)
{
	this->x = x;
	this->y = y;
	this->height = height;
	this->width = width;
	this->type = type;
	this->active = active;
}

#endif