#pragma once
#include "libtcod.hpp"
#include "dungeonMap.h"
#include "direction.h"
#include "mapRectangle.h"
#include <algorithm>
#include <vector>

/*
	TODO

	Implement "room" class for use in map generation, probably also later during gameplay
		e.g. for use with lineOfRooms function; return set of generated rooms
		so that you can fill out contents of rooms easily instead of leaving them empty
		or wasting lots of effort relocating them or writing overly specific versions of the function

		also use for quick bounds checking of room generation;
			lineOfRooms currently has no bounds checking for example

	improve fit of lineOfRooms rooms to actual underlying line;
		figure out number of rooms that fit to line and then center roughly on line,
		instead of centering first room on x1, y1 and just hoping last room winds up near x2,y2
		fit is particularly atrocious for large rooms

	outsource tile defs to json? or too soon? not that hard, but harder to do right.
		needed for pretty generation of stuff like streets

	begin working towards implementing stuff you need to make a city;
		fitted versions of lineOfRooms full of apartments,
		apartment builders,
		intersection makers,
		shops
		alleys in lineOfrooms
		etc.

	regardless of all this, lineOfRooms looks like it has big potential for city roguelike. highfive!
*/

bool spaceshipGenerator();

//Meant to generate a random city/district layout
bool cityGenerator();

//Recursive function to subdivide city interiors into buildings and streets
void cityBSP(TCODRandom* mRNG, std::vector<mapRectangle> &lots, mapRectangle current);

//Used to test possible city layout features
bool cityTester();

//Builds rowhouses in the specified lot
void rowhousesBuilder(TCODRandom* mRNG, mapRectangle rowhouse);

//Randomly replaces individual tiles in designated area with newTile at specified chance of each replacement
void randomFill(unsigned x, unsigned y, unsigned width, unsigned height,
	std::string newTile, int percent, TCODRandom* mRNG);

//Similar, but never affects tiles that are not of the same type as targetTiles
void randomFill(unsigned x, unsigned y, unsigned width, unsigned height,
	std::string newTile, std::string targetTiles, int percent, TCODRandom* mRNG);


//Creates a rectangle in designated area with different tiles on border and interior
void borderedRectangle(unsigned x, unsigned y, unsigned width, unsigned height,
	std::string border, std::string interior);

//Creates a rectangle without touching the insides of the rectangle
void borderBuilder(unsigned x, unsigned y, unsigned width, unsigned height,
	std::string border);

//Creates a room with a door at a random point on the wall that isn't a corner
//If doorWall is specified, places door on that wall of the room
void oneDoorRoom(unsigned x, unsigned y, unsigned width, unsigned height,
	std::string border, std::string interior, std::string door, TCODRandom* mRNG,
	direction doorWall = NO_DIRECTION);


//These functions are a WIP, and abandoned :/
//Meant to allow streets to run along diagonals for less structured-seeming city layouts

//Creates a series of adjacent oneDoorRooms centered along a line
//Thickness and roomSize are the dimensions of the room
//Thickness is the dimension closer to perpendicular to the line (so increasing it makes the line thicker)
//Roomsize is closer to parallel to the line, and so has less or no effect on the line's thickness
//If doorwall is set, all rooms will have their one door on the specified wall
//Otherwise, all rooms have a door on one of the two walls parallel to the line (or closer to parallel)
void lineOfRoomsv2(int x1, int y1, int x2, int y2,
	unsigned thickness, unsigned roomSize, TCODRandom * mRNG, direction doorWall = NO_DIRECTION);

void lineOfRooms(int x1, int y1, int x2, int y2,
	unsigned thickness, unsigned roomSize, TCODRandom * mRNG, direction doorWall = NO_DIRECTION);
