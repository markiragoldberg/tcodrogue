#pragma once

#include "dungeonMap.h"
#include "actor.h"
#include "direction.h"

enum actionType
{
	//if NPCs choose this action, and they shouldn't, waste 1 move
	ACTION_NONE,
	//waste enough moves to get to the highest possible multiple of TIME_SYSTEM_BASE,
	//or 0
	ACTION_WAIT,
	//into empty tiles only
	ACTION_MOVE,
	//doors, lootbaskets, etc.
	ACTION_OPEN,
	ACTION_CLOSE,
	//swap positions with nonhostile creature at target position
	ACTION_DISPLACE,
	//take a swing at creature at target position
	ACTION_MELEE
};

class creatureAction
{
public:
	actionType type() const;
	int x() const;
	int y() const;
	//time left

	creatureAction();
	creatureAction(actionType type, int x, int y);
private:
	actionType _type;
	int _x;
	int _y;
	//time left
};

//Execute a creatureAction previously decided upon by player input or AI
void takeCreatureAction(creature* acting, creatureAction action);
