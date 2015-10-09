#include "creatureAction.h"

//~~~~~~~~~~~~~~~~~~
// Creature action
//~~~~~~~~~~~~~~~~~~
actionType creatureAction::type() const
{
	return _type;
}
int creatureAction::x() const
{
	return _x;
}
int creatureAction::y() const
{
	return _y;
}

creatureAction::creatureAction()
{
	_type = ACTION_NONE;
	_x = _y -1;
}

creatureAction::creatureAction(actionType type, int x, int y)
{
	_type = type;
	_x = x;
	_y = y;
}


//~~~~~~~~~~~~~~~~~~

void takeCreatureAction(creature* acting, creatureAction action)
{
	switch (action.type())
	{
		//waste enough moves to get to the highest possible multiple of TIME_SYSTEM_BASE,
		//or 0
	case ACTION_WAIT:
		acting->wait();
		break;
		//into empty tiles only
	case ACTION_MOVE:
		if (testMap.moveCreature(acting, action.x(), action.y()))
		{
			//should use movecost of terrain
			acting->spendMoves(TIME_SYSTEM_BASE);
		}
		else
		{
			//TODO cry about it in debug log
			acting->spendMoves(1);
		}
		break;
		//doors, lootbaskets, etc.
	case ACTION_OPEN:
		//burn one move so that NPCs can't loop infinitely if this gets picked somehow
		acting->spendMoves(1);
		break;
	case ACTION_CLOSE:
		//burn one move so that NPCs can't loop infinitely if this gets picked somehow
		acting->spendMoves(1);
		break;
		//swap positions with nonhostile creature at target position
	case ACTION_DISPLACE:
		if (testMap.creatureAt(action.x(), action.y()))
		{
			//Only the PC can displace, and they cannot displace hostile creatures
			if (!testMap.creatureAt(action.x(), action.y())->hostile())
			{
				testMap.displaceCreatures(acting, testMap.creatureAt(action.x(), action.y()));
			}
		}
		//should use movecost of terrain
		//regardless, always spend moves in case of AI choosing a nonsense action
		//or game mechanic allowing displacement to fail
		acting->spendMoves(TIME_SYSTEM_BASE);
		break;
		//take a swing at creature at target position
	case ACTION_MELEE:
		if (testMap.creatureAt(action.x(), action.y()))
		{
			creature* target = testMap.creatureAt(action.x(), action.y());
			//Current melee attack resolution:
			//Always hit for max damage
			int damage = acting->damage();
			//Deal damage
			if (target->takeDamage(damage))
			{
				//If target has < 1 hp, IT IS MARKED FOR DEATH
				target->setXY(-1, -1);
			}
			acting->spendMoves(TIME_SYSTEM_BASE);
		}
		else
		{
			//TODO this action shouldn't be chosen; complain about it?
			acting->spendMoves(1);
		}
		break;
		//in case of emergency, waste a move
	case ACTION_NONE:
	default:
		acting->spendMoves(1);
	}
}