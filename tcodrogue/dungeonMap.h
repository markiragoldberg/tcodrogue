#pragma once
#include <vector>
#include "libtcod.hpp"
#include "dungeonTile.h"
#include "actor.h"

const int DUNGEONWIDTH = 81;
const int DUNGEONHEIGHT = 81;

const int DEFAULT_VIEWRANGE = 9;

//#define CANSEEEVERYTHING

class dungeonMap
{
public:
	//Get the real attributes of a given tile
	TCODColor color(int x, int y) const;
	TCODColor backColor(int x, int y) const;
	char character(int x, int y) const;
	bool walkable(int x, int y) const;
	bool transparent(int x, int y) const;
	std::string type(int x, int y) const;

	//Get the attributes of an out-of-view, possibly remembered tile
	TCODColor lastSeenColor(int x, int y) const;
	TCODColor lastSeenBackColor(int x, int y) const;
	char lastSeenCharacter(int x, int y) const;

	//TERRAIN MANAGEMENT

	void fill(std::string tileType);
	void fillArea(unsigned x, unsigned y, unsigned width, unsigned height, std::string TileType);
	void setTile(unsigned x, unsigned y, std::string tileType);

	//CREATURE MANAGEMENT

	//Adds a creature to play, starting at -1, -1 for safety
	void addCreature(creature* newCreature);

	// Attempts to move a creature to a coordinate pair on the map
	// Returns true if it works, or false if the creature cannot move there
	// (typically because the coordinate is already occupied or impassable)
	bool moveCreature(creature* moving, int x2, int y2);
	//Attempts to swap positions of two creatures
	//Returns false if it can't
	bool displaceCreatures(creature* moving, creature* displaced);
	//Returns the creature at a given coordinate, or NULL if none exists there
	creature* creatureAt(int x, int y);
	//Returns the creature at a given index in the set of creatures, or NULL if none exists
	creature* creatureIndex(unsigned i);
	//Deletes all creatures with negative health
	//Does not, should not, delete a dead PC; doing so would crash the game
	void deleteDeadCreatures();

	//PLAYER CHARACTER FIELD OF VIEW MANAGEMENT

	//Recalculates fov for a specific creature
	//Used temporarily between player turns
	//If creature is the player, remembers tiles visible last turn
	//Also later take into account status effects
	void resetFoVto(const creature* looking);
	//Check if tile is in current FoV
	bool isInFoV(unsigned x, unsigned y);

	//When the player can see a tile, call this to
	//update the player's last seen contents of that tile
	void updateSeen(unsigned x, unsigned y);

	//Initializes or updates TCODMap of visible/walkable tiles
	//Update if terrain changes during play
	//Should also eventually be able to update for creatures
	//that have unusual movement/sight restrictions
	void resetVisWalkMap();

	//Used to get a path for libtcod pathfinding
	TCODPath* getPath();

	dungeonMap();
	~dungeonMap();
private:
	//Initializes / resets the player's map memory
	void resetLastSeen();



	//Actual, present terrain found in each tile
	dungeonTile _tiles[DUNGEONWIDTH][DUNGEONHEIGHT];
	//What the PC last saw in that tile, but may have changed out of sight
	//Ex: when a door opens out of sight, this keeps the player from seeing it
	dungeonTile _lastSeen[DUNGEONWIDTH][DUNGEONHEIGHT];
	//Map used for libtcod field of view/pathfinding features
	TCODMap* _visWalkMap;
	std::vector<creature*> _creatures;
};

extern dungeonMap testMap;

//Checks if x, y are inside the dungeon's bounds
bool xyInDungeon(int x, int y);
