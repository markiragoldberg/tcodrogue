#include "dungeonTile.h"

static std::unordered_map<std::string, dungeonTileType*> _tileData;

std::string dungeonTile::name() const
{
	return _type;
}

TCODColor dungeonTile::color() const
{
	return _tileData[_type]->color();
}

TCODColor dungeonTile::backColor() const
{
	return _tileData[_type]->backColor();
}

char dungeonTile::character() const
{
	return _tileData[_type]->character();
}

bool dungeonTile::walkable() const
{
	return _tileData[_type]->walkable();
}

bool dungeonTile::transparent() const
{
	return _tileData[_type]->transparent();
}

void dungeonTile::setType(std::string newType)
{
	_type = newType;
}


dungeonTile::dungeonTile(std::string type)
{
	_type = type;
}

dungeonTile::dungeonTile()
{
	_type = "floor";
}

std::string dungeonTileType::name() const
{
	return _name;
}


TCODColor dungeonTileType::color() const
{
	return _color;
}

TCODColor dungeonTileType::backColor() const
{
	return _backColor;
}

char dungeonTileType::character() const
{
	return _character;
}

bool dungeonTileType::walkable() const
{
	return _walkable;
}

bool dungeonTileType::transparent() const
{
	return _transparent;
}

dungeonTileType::dungeonTileType(std::string name, TCODColor color, TCODColor backcolor,
	char character, bool walkable, bool transparent)
{
	_name = name;
	_color = color;
	_backColor = backcolor;
	_character = character;
	_walkable = walkable;
	_transparent = transparent;
}

dungeonTileType::dungeonTileType()
{
	_name = "undefined tile";
	_color = TCODColor::magenta;
	_backColor = TCODColor::darkerMagenta;
	_character = '?';
	_walkable = true;
	_transparent = true;
}



const dungeonTileType* getTileType(std::string name)
{
	if (_tileData.count(name) == 1)
	{
		return _tileData[name];
	}
	else
	{
		return NULL;
	}
}

void defineTile(dungeonTileType* newTile)
{
	if (_tileData.count(newTile->name()) == 0)
	{
		_tileData[newTile->name()] = newTile;
	}
	else
	{
		// TODO : scream about identical tile names. also, tile name probably shouldn't need to be unique.
		delete newTile;
	}
}

void deleteTheTiles()
{
	std::unordered_map<std::string, dungeonTileType*>::iterator i;
	for (i = _tileData.begin(); i != _tileData.end(); i++)
	{
		delete i->second;
	}
}
