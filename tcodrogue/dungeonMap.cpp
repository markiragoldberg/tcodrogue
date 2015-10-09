#include "dungeonMap.h"

dungeonMap testMap = dungeonMap();

TCODColor dungeonMap::color(int x, int y) const
{
	if (xyInDungeon(x,y))
	{
		return _tiles[x][y].color();
	}
	return TCODColor::black;
}

TCODColor dungeonMap::backColor(int x, int y) const
{
	if (xyInDungeon(x, y))
	{
		return _tiles[x][y].backColor();
	}
	return TCODColor::black;
}

char dungeonMap::character(int x, int y) const
{
	if (xyInDungeon(x, y))
	{
		return _tiles[x][y].character();
	}
	return '?';
}

bool dungeonMap::walkable(int x, int y) const
{
	if (xyInDungeon(x, y))
	{
		return _tiles[x][y].walkable();
	}
	return false;
}

bool dungeonMap::transparent(int x, int y) const
{
	if (xyInDungeon(x, y))
	{
		return _tiles[x][y].transparent();
	}
	return false;
}

std::string dungeonMap::type(int x, int y) const
{
	if (xyInDungeon(x, y))
	{
		return _tiles[x][y].name();
	}
	return false;
}

//~~~~~~~~~~~~~~~~~~~~
// Last seen versions
//~~~~~~~~~~~~~~~~~~~~

TCODColor dungeonMap::lastSeenColor(int x, int y) const
{
	if (xyInDungeon(x, y))
	{
		//Strip out-of-view tiles of color, and darken them
		TCODColor drawnColor = _lastSeen[x][y].color();
		drawnColor.scaleHSV(0.0, 0.5);
		return drawnColor;
	}
	return TCODColor::black;
}

TCODColor dungeonMap::lastSeenBackColor(int x, int y) const
{
	if (xyInDungeon(x, y))
	{
		//Again, strip color and darken
		TCODColor drawnColor = _lastSeen[x][y].backColor();
		drawnColor.scaleHSV(0.0, 0.5);
		return drawnColor;
	}
	return TCODColor::black;
}

char dungeonMap::lastSeenCharacter(int x, int y) const
{
	if (xyInDungeon(x, y))
	{
		return _lastSeen[x][y].character();
	}
	return '?';
}

//~~~~~~~~~~~~~~~~~~~~

void dungeonMap::fill(std::string fillerTile)
{
	for (int i = 0; i < DUNGEONWIDTH; i++)
	{
		for (int j = 0; j < DUNGEONHEIGHT; j++)
		{
			_tiles[i][j].setType(fillerTile);
		}
	}
}

void dungeonMap::fillArea(unsigned x, unsigned y, unsigned width, unsigned height,
	std::string newTile)
{
	//setTile handles bounds checking
	for (unsigned i = x; i < x + width; i++)
	{
		for (unsigned j = y; j < y + width; j++)
		{
			setTile(i, j, newTile);
		}
	}
}

void dungeonMap::setTile(unsigned x, unsigned y, std::string newTile)
{
	if (x < DUNGEONWIDTH && y < DUNGEONHEIGHT)
	{
		_tiles[x][y].setType(newTile);
	}
}

bool xyInDungeon(int x, int y)
{
	if (x > -1 && x < DUNGEONWIDTH && y > -1 && y < DUNGEONHEIGHT)
	{
		return true;
	}
	return false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// CREATURE MANAGEMENT
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void dungeonMap::addCreature(creature* newCreature)
{
	newCreature->setXY(-1, -1);
	_creatures.push_back(newCreature);
}

bool dungeonMap::moveCreature(creature* moving, int x2, int y2)
{
	if (walkable(x2,y2) && creatureAt(x2, y2) == NULL)
	{
		moving->setXY(x2, y2);
		return true;
	}
	return false;
}

bool dungeonMap::displaceCreatures(creature* moving, creature* displaced)
{
	int x = moving->x();
	int y = moving->y();
	moving->setXY(displaced->x(),displaced->y());
	displaced->setXY(x, y);

	return true;
}

//Returns the creature at a given coordinate, or NULL if none exists there
creature* dungeonMap::creatureAt(int x, int y)
{
	for (creature* i : _creatures)
	{
		if (i->x() == x && i->y() == y)
		{
			return i;
		}
	}
	return NULL;
}

//Returns the creature at a given index in the set of creatures, or NULL if none exists
creature* dungeonMap::creatureIndex(unsigned i)
{
	if (i < _creatures.size())
	{
		return _creatures[i];
	}
	return NULL;
}

void dungeonMap::deleteDeadCreatures()
{
	//Start at 1 because PC is at index 0 and must never be removed
	for (unsigned c = 1; c < _creatures.size(); c++)
	{
		if (_creatures[c]->hp() < 1)
		{
			delete _creatures[c];
			_creatures.erase(_creatures.begin() + c);
			c--;
		}
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Visibility
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void dungeonMap::resetFoVto(const creature* looking)
{
	if (looking != NULL)
	{
		//If the looking actor isn't dead, check fov
		if (looking->x() != -1 && looking->y() != -1)
		{
			//At some point, view range should depend on PC's state
			_visWalkMap->computeFov(looking->x(), looking->y(),
				DEFAULT_VIEWRANGE, true, FOV_PERMISSIVE_8);
		}
		//Dead are blind
		else
		{
			for (unsigned x = 0; x < DUNGEONWIDTH; x++)
				for (unsigned y = 0; y < DUNGEONHEIGHT; y++)
					_visWalkMap->setInFov(x, y, false);
		}
	}
}

bool dungeonMap::isInFoV(unsigned x, unsigned y)
{
	if (x < DUNGEONWIDTH && y < DUNGEONHEIGHT && _visWalkMap->isInFov(x,y))
	{
		return true;
	}
	return false;
}

void dungeonMap::updateSeen(unsigned x, unsigned y)
{
	if (x < DUNGEONWIDTH && y < DUNGEONHEIGHT)
	{
		_lastSeen[x][y].setType(_tiles[x][y].name());
	}
}

void dungeonMap::resetVisWalkMap()
{
	for (unsigned x = 0; x < DUNGEONWIDTH; x++)
	{
		for (int y = 0; y < DUNGEONHEIGHT; y++)
		{
			_visWalkMap->setProperties(x, y, _tiles[x][y].transparent(), _tiles[x][y].walkable());
		}
	}
}

TCODPath* dungeonMap::getPath()
{
	return new TCODPath(_visWalkMap);
}

void dungeonMap::resetLastSeen()
{
	for (unsigned x = 0; x < DUNGEONWIDTH; x++)
	{
		for (unsigned y = 0; y < DUNGEONHEIGHT; y++)
		{
			_lastSeen[x][y].setType("unknown");
		}
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~

dungeonMap::dungeonMap()
{
	fill("wall");
	resetLastSeen();
	_visWalkMap = new TCODMap(DUNGEONWIDTH,DUNGEONHEIGHT);
	_visWalkMap->clear();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~

dungeonMap::~dungeonMap()
{
	for (creature* i : _creatures)
	{
		delete i;
	}
	delete _visWalkMap;
}
