#include "dungeonTile.h"
#include <string>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

//Load terrain data from .json file
bool loadTilesFromFile();

//Translate user-friendly strings to corresponding TCODColor
// Ex: "lightest grey" to TCODColor::lightestGrey

//alternatively just use RGB values
TCODColor stringToTCODColor(std::string colorstring);

