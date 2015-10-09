#include "main.h"

int main()
{
	initRendering();
	loadDataFromFiles();

	cityGenerator();
	testMap.resetVisWalkMap();


	//TCODConsole::setKeyboardRepeat(300, 100);

	gameLoop();

	deleteDataLoadedFromFiles();

	return 0;
}