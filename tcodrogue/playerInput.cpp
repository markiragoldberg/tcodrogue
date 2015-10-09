#include "playerInput.h"

creatureAction handlePlayerTurn()
{
	creature * pc = testMap.creatureIndex(0);
	creatureAction playerAction;

	/*if (pc == NULL)
	{
		//DEBUG WARNING there needs to be safety check here???
	}*/

	renderGame(pc->x(), pc->y());

	//Ultimately used for destination of player's chosen action
	int targetX;
	int targetY;
	
	TCOD_key_t input;
	do
	{
		//Ultimately this will be needed once the game renders stuff
		//after the player does nonactions, or if the window loses focus
		renderFlush();

		//Set target to player's location
		targetX = pc->x();
		targetY = pc->y();

		input = TCODConsole::root->checkForKeypress(TCOD_KEY_PRESSED);
		//If the player pressed a move key, this also moves target coords
		//to those of tile player moved at
		if (keyToXY(input, targetX, targetY))
		{
			if (testMap.creatureAt(targetX, targetY))
			{
				creature* target = testMap.creatureAt(targetX, targetY);

				if (target->hostile())
				{
					playerAction = creatureAction(ACTION_MELEE, targetX, targetY);
				}
				else if (testMap.walkable(targetX, targetY))
				{
					playerAction = creatureAction(ACTION_DISPLACE, targetX, targetY);
				}
			}
			//No one's there but you can walk there
			else if (testMap.walkable(targetX, targetY))
			{
				playerAction = creatureAction(ACTION_MOVE, targetX, targetY);
			}
			//else if there's a door there
				//open the door
			//else maybe say something about that being unwalkable
		}
		//wait if pressing numpad 5
		else if (input.vk == TCODK_KP5)
		{
			playerAction = creatureAction(ACTION_WAIT, targetX, targetY);
		}
	}
	while (playerAction.type() == ACTION_NONE && 
		!TCODConsole::root->isWindowClosed() && input.vk != TCODK_ESCAPE);

	return playerAction;
}
