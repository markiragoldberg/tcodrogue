#pragma once

#include "libtcod.hpp"
#include "dungeonMap.h"
#include "actor.h"
#include "creatureAction.h"
#include "direction.h"

//Where to start when getting an NPC to take their turn
creatureAction chooseNPCAction(creature* acting, TCODRandom * gRNG);

//If an NPC wants to randomwalk, this function chooses a specific destination
//The NPC may decide to wait if it chooses an unwalkable or occupied tile
creatureAction chooseRandomwalkDestination(creature* acting, TCODRandom * gRNG);

creatureAction chooseMeleeCharge(creature* acting, TCODRandom * gRNG, int targetX, int targetY);



