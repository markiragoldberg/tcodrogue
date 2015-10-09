#pragma once
#include "tileData.h"
#include "creatureData.h"

//Calls every startup function that reads data from files
void loadDataFromFiles();

//Deletes all data loaded from files
void deleteDataLoadedFromFiles();
