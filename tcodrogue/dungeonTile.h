#pragma once
#include "libtcod.hpp"
#include <unordered_map>

#include <cassert>

// TODO - Tiles stored as strings
//I think that storing tile names as a string in all places
//could be slowing the game down a lot. Before release,
//it may be a good idea to change this so that only the
//tile type knows the tile's name, and individual tiles use
//the index of the tile type to identify themselves internally.

//I can probably test this assertion by shortening all the
//tile type names to one character and seeing if it makes the
//game faster.

class dungeonTile
{
public:
	std::string name() const;
	TCODColor color() const;
	TCODColor backColor() const;
	char character() const;
	bool walkable() const;
	bool transparent() const;

	void setType(std::string newType);

	dungeonTile(std::string type);
	dungeonTile();
private:
	std::string _type;
};

class dungeonTileType
{
public:
	std::string name() const;
	TCODColor color() const;
	TCODColor backColor() const;
	char character() const;
	bool walkable() const;
	bool transparent() const;

	// effect blockity, durability, etc as needed

	dungeonTileType(std::string name, TCODColor color, TCODColor backcolor, 
		char character, bool walkable, bool transparent);
	dungeonTileType();
private:
	std::string _name;
	TCODColor _color;
	TCODColor _backColor;
	char _character;
	bool _walkable;
	bool _transparent;
};

//Get a dungeonTileType to look at
//May return NULL
const dungeonTileType* getTileType(std::string name);

//Define a dungeonTileType, such as during data file loading
void defineTile(dungeonTileType* newTile);

//Deletes tile data before program exit
void deleteTheTiles();
