#include "tileData.h"

//TODO: microsoft doesn't like fopen?
#pragma warning(disable:4996)

bool loadTilesFromFile()
{
	FILE* fp = fopen("data/tiles.json", "r");
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
		std::string _name = "Undefined tile name";
		TCODColor _color = TCODColor::lightestGrey;
		TCODColor _backColor = TCODColor::black;
		char _character = '?';
		bool _walkable = true;
		bool _transparent = false;

		if(jsondata[i].HasMember("tile"))
		{
			_name = jsondata[i]["tile"].GetString();
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

			_color = TCODColor(red, green, blue);
		}
		if (jsondata[i].HasMember("background") && jsondata[i]["background"].IsArray() && jsondata[i]["background"].Size() == 3)
		{
			//Default color is magenta
			int red, green, blue;
			red = blue = 255; green = 0;

			red = jsondata[i]["background"][0].GetInt();
			green = jsondata[i]["background"][1].GetInt();
			blue = jsondata[i]["background"][2].GetInt();

			_backColor = TCODColor(red, green, blue);
		}
		if (jsondata[i].HasMember("character"))
		{
			_character = jsondata[i]["character"].GetString()[0];
		}
		if (jsondata[i].HasMember("walkable"))
		{
			_walkable = jsondata[i]["walkable"].GetBool();
		}
		if (jsondata[i].HasMember("transparent"))
		{
			_transparent = jsondata[i]["transparent"].GetBool();
		}

		dungeonTileType *cur = new dungeonTileType(_name, _color, _backColor, _character, _walkable, _transparent);
		defineTile(cur);
	}

	free(buffer);
	return true;
}