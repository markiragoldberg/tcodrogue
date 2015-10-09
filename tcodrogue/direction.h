#pragma once
#include "libtcod.hpp"

enum direction
{
	NO_DIRECTION,
	N,
	E,
	S,
	W,
	NE,
	SE,
	SW,
	NW
};

// TODO low priority: These last two functions _maybe_ shouldn't be here
// because some code that needs directions doesn't need to handle user input

//translate keyboard input into a direction
direction keyToDirection(const TCOD_key_t &key);

//adjust x, y coordinates based on keyboard input
//Returns true if a change was made, else false
bool keyToXY(const TCOD_key_t & key, int &x, int &y);

//Converts a direction to a one-tile shift in coordinates given
//Returns true if a change occured, else fals
bool directionToXY(direction d, int &x, int &y);
