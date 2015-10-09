#pragma once

#include <string>
#include <unordered_map>
#include "libtcod.hpp"

//The arbitrary basic time cost of many common actions, like moving one tile
//Also the number of arbitrary time units most creatures receive in one turn
#define TIME_SYSTEM_BASE 12
//TALKING ABOUT TIME UNITS
//It is 12 (and not 10) because 12 is a multiple of 2, 3, and 4
//I predict that I will want to implement creatures that move either:
//same speed as you
//very slightly slower/faster than you (both 1/12th or 1/10th would be fine)
//it doesn't matter if you're 1/10th faster or 1/12th faster than a zombie,
//you're not going to kite it very well.
//tremendously faster than you (e.g. 50%, 100%, 200% faster than you)
//or significantly faster/slower than you in a way that is easy to predict
//For this last purpose, 1 extra turn for every 2/3/4 is more useful than
//1 extra turn every 2 or 5.


//TODO - Tiles/Faux-ASCII rendering data reform
//Creature rendering data should not be touched by the actorType;
//instead, the rendering code should load rendering data for actors.
//The data files should have both stats and rendering data in the same place for the users,
//but the data should be separated internally.
//Doing this is useful if/when multiple rendering options (tiles, character graphics) are supported,
//because at that point the actor code really won't know how actors are actually represented.


//MAPOBJECT
//Anything that exists in a location on the map and either can be moved or does something independently
//Includes creatures
//Includes timed items, like live explosives, that do stuff after some time, or periodically
//Could later include movable furniture that doesn't overwrite the terrain
class mapObject
{
public:
	int x() const;
	int y() const;

	//Consider making these protected and only giving friend status to the map
	//so that only the map can move / remove the actor  in / from the map
	void setXY(int x, int y);

	//mapObjects start at -1,-1, because only the map knows where they CAN go
	mapObject();
private:
	//Location in map
	int _x;
	int _y;
	//mapObjectType _type;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class creatureType
{
public:
	//The common title for all creatures of one type
	//e.g. "vault guard", "mean-looking mercenary"
	std::string title() const;

	//Graphics data that ultimately doesn't belong here
	char tile() const;
	TCODColor color() const;
	//Hypothetically, aura color could indicate statuses
	//TCODColor backColor() const;

	int speed() const;
	bool hostile() const;
	int maxhp() const;
	int damage() const;

	creatureType(std::string title, char tile, TCODColor color, int speed, bool hostile, int maxhp, int damage);
private:
	std::string _title;
	char _tile;
	TCODColor _color;
	int _speed;
	bool _hostile;
	int _maxhp;
	int _damage;
};

//Defines a creature type during data file loading
void defineCreatureType(creatureType* newType);

//Deletes creature type data during end of program
void deleteCreatureTypes();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class creature : public mapObject
{
public:
	//How many actions the creature gets each game-turn
	int speed() const;
	
	//Gives the creature more moves
	//Returns true if creature now has >0 moves, and can act
	//Currently also restores 1 HP because more sophisticated healing DNE
	bool addMoves();
	//Deduct some moves (negative values do nothing)
	void spendMoves(int moves);
	//Deduct moves to get to 0 or a multiple of TIME_SYSTEM_BASE,
	//whichever is higher
	void wait();
	//Returns true if the creature now has >0 moves, and can act
	bool canMove();

	//The common title for all creatures of one type
	//e.g. "vault guard", "mean-looking mercenary"
	std::string title() const;
	//Graphics data that ultimately doesn't belong here
	char tile() const;
	TCODColor color() const;

	bool hostile() const;
	void becomeHostile();

	int hp() const;
	int maxhp() const;
	int damage() const;

	//Returns true if HP reduced to < 1
	//Negative/0 damage has no effect
	bool takeDamage(int amount);

	creature(std::string _type);
private:
	//SEMI-PERMANENT STATISTICS
	std::string _type;

	bool _hostile;

	// TEMPORARY STATISTICS
	//How long the creature needs to wait before taking more actions,
	//or how many actions it can take right now
	int _moves;
	//You need to hold onto one of these to not die
	int _hp;
};

/*
class timedItem : mapObject
{
public:
private:
};
*/
