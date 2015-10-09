#include "dataLoading.h"

void loadDataFromFiles()
{
	loadTilesFromFile();
	loadCreaturesFromFile();
}

void deleteDataLoadedFromFiles()
{
	deleteTheTiles();
	deleteCreatureTypes();
}
