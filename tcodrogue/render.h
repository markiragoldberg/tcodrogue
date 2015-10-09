#pragma once
#include <sstream>
#include "libtcod.hpp"
#include "dungeonMap.h"

//The area dedicated to the map should be odd for best results
//(Currently, the entire window is dedicated to the map)
//TODO DEBUG actual gameplay should use much lower values for compatibility with 32px tiles and low rezzes
//Ex: 9 viewrange for 19 tiles worth of map (608 pixels) and then some space at the bottom for messages
const int WINDOWWIDTH = 81;
const int WINDOWHEIGHT = 81;

//Sets up libtcod
void initRendering();

//Recenters the game map on x,y
//Does not flush
void renderGame(int x, int y);

//Flushes (shows) the game map
void renderFlush();
