#include "gameLoop.h"

void gameLoop()
{
	TCODRandom* gRNG = new TCODRandom();
	//TCODConsole::setKeyboardRepeat(300, 100);

	//Spawn the player and some creatures
	for (int i = 0; i < 10; i++)
	{
		creature* spawn;
		if (i == 0)
			spawn = new creature("player");
		else if (i == 9)
			spawn = new creature("thief");
		else
			spawn = new creature("human");

		testMap.addCreature(spawn);

		int x; int y;
		do
		{
			x = gRNG->getInt(1, DUNGEONWIDTH - 1);
			y = gRNG->getInt(1, DUNGEONHEIGHT - 1);
		} while (!testMap.moveCreature(spawn, x, y));
	}

	//Coordinates of viewpoint
	int x = testMap.creatureIndex(0)->x(); int y = testMap.creatureIndex(0)->y();
	testMap.resetFoVto(testMap.creatureIndex(0));
	renderGame(x, y);
	do // turn loop / while player hasn't quit
	{
		renderFlush();

		unsigned index = 0;
		creature * actor = testMap.creatureIndex(index);
		while (actor != NULL)
		{
			//If adding moves gives the actor enough moves to act,
			if (actor->addMoves())
			{
				creatureAction action;

				do // loop in case of fast actors with 2+ turns
				{
					testMap.resetFoVto(actor);
					//TODO if actor is the PC
					if (index == 0)
					{
						action = handlePlayerTurn();
						//until player has chosen an action that costs moves
							//get player input
							//action = parse player action();
							//respond to player input if it doesn't result in an action
					}
					else
					{
						action = chooseNPCAction(actor, gRNG);
						//resetFOV to NPC if necessary
						//THE NPC CANNOT EVER NEVER CHOOSE AN ACTION THAT
						//DOES NOT COST AT LEAST 1 MOVE
						//NO. FUCKING. EXCUSES.
						//creatureAction = npcTakeYourTurn();
					}
					//DEDUCT SOME MOVES IN FOLLOWING FUNCTION
					//COME HELL OR HIGH WATER
					takeCreatureAction(actor, action);
				}
				while (actor->canMove());
			}
			index++;
			actor = testMap.creatureIndex(index);
		}
		//Between turns is a safe time to delete dead creatures without messing up iteration
		testMap.deleteDeadCreatures();
	} while (!TCODConsole::root->isWindowClosed());

	delete gRNG;
}