#include "AIDecision.h"

creatureAction chooseNPCAction(creature* acting, TCODRandom * gRNG)
{
	//Neutral NPCs currently always randomwalk
	if (!acting->hostile())
	{
		return chooseRandomwalkDestination(acting, gRNG);
	}
	//Hostile NPCs charge the player if they are visible, else randomwalk
	else
	{
		//Check FoV
		testMap.resetFoVto(acting);

		//Find the player
		int targetX = testMap.creatureIndex(0)->x();
		int targetY = testMap.creatureIndex(0)->y();

		//if can't see player, randomwalk
		if (!testMap.isInFoV(targetX, targetY))
		{
			return chooseRandomwalkDestination(acting, gRNG);
		}
		//else, charge player recklessly
		else
		{
			return chooseMeleeCharge(acting, gRNG, targetX, targetY);
		}
	}
}

creatureAction chooseRandomwalkDestination(creature* acting, TCODRandom * gRNG)
{
	creatureAction NPCAction;

	int x = acting->x();
	int y = acting->y();

	direction direct = (direction)gRNG->getInt(N, NW);

	directionToXY(direct, x, y);

	if (testMap.walkable(x, y))
	{
		//NPCs don't displace you or others
		if (testMap.creatureAt(x, y) == NULL)
		{
			NPCAction = creatureAction(ACTION_MOVE, x, y);
		}
	}
	else
	{
		NPCAction = creatureAction(ACTION_WAIT, acting->x(), acting->y());
	}
	return NPCAction;
}

creatureAction chooseMeleeCharge(creature* acting, TCODRandom * gRNG, int targetX, int targetY)
{
	creatureAction NPCAction;

	TCODPath * path = testMap.getPath();

	int x = acting->x();
	int y = acting->y();

	//Compute a path to the target
	//If it fails, there is no path, so just randomwalk
	if (!path->compute(x, y, targetX, targetY))
	{
		NPCAction = chooseRandomwalkDestination(acting, gRNG);
	}
	//If adjacent, attack
	else if (path->size() <= 1)
	{
		//TODO this is where you'd melee attack
		NPCAction = creatureAction(ACTION_MELEE, targetX, targetY);
	}
	//If not adjacent and have path, move
	else
	{
		path->walk(&targetX, &targetY, false);  //sets targetX/Y to next tile along path
		NPCAction = creatureAction(ACTION_MOVE, targetX, targetY);
	}
	delete path;
	return NPCAction;
}
