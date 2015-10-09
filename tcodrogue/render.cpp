#include "render.h"

void initRendering()
{
	TCODConsole::initRoot(WINDOWWIDTH, WINDOWHEIGHT, "Map generation testing");
}

//Updates libtcod's internal idea of what should be shown on the screen/map when
//it refreshes (but does not actually refresh the screen).
void renderGame(int x, int y)
{
	TCODConsole::root->clear();

	creature* drawnCreature;

	//AWFUL DEATH HANDLING WITH SHAMEFUL GOTO RETURN
	if (testMap.creatureIndex(0)->hp() < 1)
	{
		TCODConsole::root->print(WINDOWWIDTH / 2 - 6, WINDOWHEIGHT / 2, 
			"YOU ARE DEAD.");
		return;
	}

	//The calculations here look, and are, complicated because
	//they have to convert positions from one frame of reference to another.

	//The internal map stores locations relative to a point 0,0 at the upper
	//left / northwest corner of the internal map, but they need to be drawn
	//relative to the PC's ever-changing position, and with the PC in
	//the center of the map window as an additional factor.

	//Here, 'i' and 'j' are coordinates for the internal map, relative to 0,0.
	//Converting those to window coordinates requires subtracting away
	//their initial value (so their value before any iteration is 0).
	for (int i = x-WINDOWWIDTH/2; i <= x+WINDOWWIDTH/2; i++)
	{
		for (int j = y-WINDOWHEIGHT/2; j <= y+WINDOWHEIGHT/2; j++)
		{
#ifndef CANSEEEVERYTHING
			if (testMap.isInFoV(i,j))
#endif
			{
				drawnCreature = testMap.creatureAt(i, j);
				//Draw creature if present, else terrain
				if (drawnCreature != NULL)
				{
					TCODConsole::root->putCharEx(
						i - x + WINDOWWIDTH / 2, j - y + WINDOWHEIGHT / 2,
						drawnCreature->tile(), drawnCreature->color(), TCODColor::black);
				}
				else
				{
					TCODConsole::root->putCharEx(
						i - x + WINDOWWIDTH / 2, j - y + WINDOWHEIGHT / 2,
						testMap.character(i, j), testMap.color(i, j), testMap.backColor(i, j));
				}
				//Whenever a tile is rendered, update the lastSeen version of it
				testMap.updateSeen(i, j);
			}
#ifndef CANSEEEVERYTHING
			else
			{
				//draw remembered features, if any exist
				TCODConsole::root->putCharEx(
					i - x + WINDOWWIDTH / 2, j - y + WINDOWHEIGHT / 2,
					testMap.lastSeenCharacter(i, j), testMap.lastSeenColor(i, j), 
					testMap.lastSeenBackColor(i, j));
			}
#endif
		}
	}
	//Shitty health display
	std::stringstream healthdisplay;
	healthdisplay << "Health: " << testMap.creatureIndex(0)->hp();
	TCODConsole::root->print(0, 0, healthdisplay.str().c_str());
}

void renderFlush()
{
	TCODConsole::root->flush();
}

