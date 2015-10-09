#include "creatureData.h"

//TODO : Windows doesn't like fopen()
#pragma warning(disable:4996)


bool loadCreaturesFromFile()
{
	FILE* fp = fopen("data/creatures.json", "r");
	fseek(fp, 0, SEEK_END);
	size_t filesize = (size_t)ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buffer = (char*)malloc(filesize + 1);
	size_t readLength = fread(buffer, 1, filesize, fp);
	buffer[readLength] = '\0';
	fclose(fp);
	// In situ parsing the buffer into d, buffer will also be modified
	// Query/manipulate the DOM here...
	rapidjson::Document jsondata;
	jsondata.Parse(buffer);

	for (unsigned i = 0; i < jsondata.Size(); i++)
	{
		std::string title = "Undefined creature name";
		char tile = '@';
		TCODColor color = TCODColor::yellow;
		int speed = TIME_SYSTEM_BASE;
		bool hostile = false;
		int maxhp = 1;
		int damage = 0;

		if (jsondata[i].HasMember("name"))
		{
			title = jsondata[i]["name"].GetString();
		}
		// TODO: allow use of strings (e.g. "light grey") for user friendlier .json
		if (jsondata[i].HasMember("color") && jsondata[i]["color"].IsArray() && jsondata[i]["color"].Size() == 3)
		{
			//Default color is magenta
			int red, green, blue;
			red = blue = 255; green = 0;

			red = jsondata[i]["color"][0].GetInt();
			green = jsondata[i]["color"][1].GetInt();
			blue = jsondata[i]["color"][2].GetInt();

			color = TCODColor(red, green, blue);
		}
		if (jsondata[i].HasMember("tile"))
		{
			tile = jsondata[i]["tile"].GetString()[0];
		}
		if (jsondata[i].HasMember("speed"))
		{
			speed = jsondata[i]["speed"].GetInt();
		}
		if (jsondata[i].HasMember("speed"))
		{
			speed = jsondata[i]["speed"].GetInt();
		}
		if (jsondata[i].HasMember("hostile"))
		{
			hostile = jsondata[i]["hostile"].GetBool();
		}
		if (jsondata[i].HasMember("max hp"))
		{
			maxhp = jsondata[i]["max hp"].GetInt();
		}
		if (jsondata[i].HasMember("damage"))
		{
			damage = jsondata[i]["damage"].GetInt();
		}
		creatureType* cur = new creatureType(title, tile, color, speed, hostile, maxhp, damage);
		defineCreatureType(cur);
	}

	free(buffer);
	return true;
}
