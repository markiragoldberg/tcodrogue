#include "mapgen.h"


// Constants related to city generation

//Chance a large building (with no currently-defined purpose) will generate in lots that would allow it
static const int BIGBUILDING_CHANCE = 50;
//Neither dimension can be larger/smaller than these
static const int MAX_BIGBUILDING_SIZE = 30;
static const int MIN_BIGBUILDING_SIZE = 0;
//Chance a set of rowhouses will generate in lots that would allow it
static const int ROWHOUSE_CHANCE = 50;
//Chance an individual rowhouse will be closer or further from the road than its neighbor
static const int ROWHOUSE_UNEVEN_CHANCE = 100;
//Currently detached homes can be quite small, but I'd like that to be rarer later...
static const int MIN_DETACHED_HOUSE_WIDTH = 5;
//Rowhouses are small, but vary in width quite a bit (3-6 interior space, sometimes even more)
static const unsigned MIN_ROWHOUSE_WIDTH = 5;
static const unsigned MAX_ROWHOUSE_WIDTH = 8;
//Sometimes results in duplexes
static const int MIN_ROWHOUSE_ROW_LENGTH = (MIN_ROWHOUSE_WIDTH - 1) * 3 + 1;
static const int MAX_ROWHOUSE_ROW_LENGTH = std::min(DUNGEONWIDTH - 1, DUNGEONHEIGHT - 1);
//Long rows of rowhouses need breaks in them for navigation
static const unsigned MAX_ROWHOUSE_LENGTH_WITHOUT_ALLEY = MAX_ROWHOUSE_ROW_LENGTH/2;
//I think perhaps later, street width should be determined more by how small the BSP partition is
//Also, how safe/isolated the district is
static const int STREET_WIDTH = 1;

bool spaceshipGenerator()
{
	//deleted at end of function
	TCODRandom* mRNG = new TCODRandom();

	testMap.fill("floor");

	lineOfRooms(10, 10, 15, DUNGEONHEIGHT - 10, 7, 9, mRNG);
	lineOfRooms(DUNGEONWIDTH - 10, 10, DUNGEONWIDTH - 25, DUNGEONHEIGHT - 10, 14, 7, mRNG);
	
	lineOfRooms(30, DUNGEONHEIGHT - 10, DUNGEONWIDTH - 35, DUNGEONHEIGHT - 15, 7, 7, mRNG);
	

	//Put holes in exterior wall of cave
	//Then remove entire interior of cave
	//Then add a few pillars/walls to the interior again
	/*randomFill(DUNGEONWIDTH / 4 - 1, DUNGEONHEIGHT / 4 - 1, DUNGEONWIDTH / 2 + 2, DUNGEONHEIGHT / 2 + 2,
		"floor", 50, mRNG);
	testMap.fillArea(DUNGEONWIDTH / 4, DUNGEONHEIGHT / 4, DUNGEONWIDTH / 2, DUNGEONHEIGHT / 4, "floor");
	randomFill(DUNGEONWIDTH / 4, DUNGEONHEIGHT / 4, DUNGEONWIDTH / 2, DUNGEONHEIGHT / 2,
		"wall", 5, mRNG);*/

	delete mRNG;
	return true;
}

bool cityGenerator()
{
	//deleted at end of function
	TCODRandom* mRNG = new TCODRandom();

	//DEBUG Possible error detector
	testMap.fill("debug");
	
	//Parcels of land meant to become buildings, streets, etc.
	//Streets/alleys included so they can be blocked off intelligently without killing connectivity
	std::vector<mapRectangle> lots;
	cityBSP(mRNG, lots, mapRectangle(0, 0, DUNGEONWIDTH-1, DUNGEONHEIGHT-1));

	for (unsigned i = 0; i < lots.size(); i++)
	{
		switch (lots[i].type())
		{
		case BUILDING_HROWHOUSES:
		case BUILDING_VROWHOUSES:
			rowhousesBuilder(mRNG, lots[i]);
			break;
		case BUILDING_BIGBUILDING:
			borderedRectangle(lots[i].x(), lots[i].y(), lots[i].width(), lots[i].height(), "wall", "floor");
			//DEBUG sinfully shitty doors
			testMap.setTile(lots[i].x(), lots[i].y() + lots[i].height() / 2, "door");
			testMap.setTile(lots[i].x() + lots[i].width() - 1, lots[i].y() + lots[i].height() / 2, "door");
			break;
		default:
			borderedRectangle(lots[i].x(), lots[i].y(), lots[i].width(), lots[i].height(), "wall", "floor");
			//DEBUG sinfully shitty doors
			testMap.setTile(lots[i].x() + lots[i].width() - 1, lots[i].y() + lots[i].height() / 2, "door");
		}
		//place debug markers at lot corners
		/*
		testMap.setTile(lots[i].x(), lots[i].y(), "debug");
		testMap.setTile(lots[i].x(), lots[i].y2(), "debug");
		testMap.setTile(lots[i].x2(), lots[i].y(), "debug");
		testMap.setTile(lots[i].x2(), lots[i].y2(), "debug");
		*/
	}

	//Wall off any streets that hit the edge of the map but aren't meant to be district exits
	borderBuilder(0, 0, DUNGEONWIDTH, DUNGEONHEIGHT, "wall");

	//Place district exits (which don't yet exist)

	delete mRNG;
	return true;
}

void cityBSP(TCODRandom* mRNG, std::vector<mapRectangle> &lots, mapRectangle current)
{
	int vert = -1;

	//Don't divide a building that's very small
	//Do divide buildings that have only one small side, but have a chance of making rowhouses instead
	if (current.width() < MIN_DETACHED_HOUSE_WIDTH * 2 + STREET_WIDTH)
	{
		//Chance of making rowhouses
		if (current.height() <= MAX_ROWHOUSE_ROW_LENGTH &&
			current.height() >= MIN_ROWHOUSE_ROW_LENGTH && 
			mRNG->getInt(0, 99) < ROWHOUSE_CHANCE)
		{
			current.setType(BUILDING_VROWHOUSES);
			lots.push_back(current);
			return;
		}
		else
		{
			vert = 0;
		}
	}
	if (current.height() < MIN_DETACHED_HOUSE_WIDTH * 2 + STREET_WIDTH)
	{
		if (current.width() < MAX_ROWHOUSE_ROW_LENGTH && 
			current.width() > MIN_ROWHOUSE_ROW_LENGTH &&
			mRNG->getInt(0, 99) < ROWHOUSE_CHANCE)
		{
			current.setType(BUILDING_HROWHOUSES);
			lots.push_back(current);
			return;
		}
		else
		{
			vert += 2;
		}
	}

	//Can divide in either direction
	if (vert == -1)
	{
		//May make a big building instead
		if (mRNG->getInt(0, 99) < BIGBUILDING_CHANCE &&
			current.width() >= MIN_BIGBUILDING_SIZE && current.width() <= MAX_BIGBUILDING_SIZE &&
			current.height() >= MIN_BIGBUILDING_SIZE && current.height() <= MAX_BIGBUILDING_SIZE)
		{
			current.setType(BUILDING_BIGBUILDING);
			lots.push_back(current);
			return;
		}
		vert = mRNG->getInt(0, 1);
	}
	//This lot cannot be subdivided in either direction
	else if (vert == 2)
	{
		lots.push_back(current);
		return;
	}

	//Divide with a street
	if (vert == 1)
	{
		// If street is > 1 tile, need to account for it
		int streetx = mRNG->getInt(current.x() + MIN_DETACHED_HOUSE_WIDTH,
			current.x2() - MIN_DETACHED_HOUSE_WIDTH - (STREET_WIDTH-1));

		cityBSP(mRNG,lots,mapRectangle(current.x(), current.y(), streetx-1, current.y2()));
		cityBSP(mRNG, lots, mapRectangle(streetx + STREET_WIDTH, current.y(), current.x2(), current.y2()));

		borderedRectangle(streetx, current.y(), STREET_WIDTH, current.height(), "pavers", "pavers");
		//DEBUG building street immediately instead of lotting and potentially strewing junk
	}
	else
	{
		// -1 for street's own size
		int streety = mRNG->getInt(current.y() + MIN_DETACHED_HOUSE_WIDTH, 
			current.y2() - MIN_DETACHED_HOUSE_WIDTH - (STREET_WIDTH - 1));

		cityBSP(mRNG, lots, mapRectangle(current.x(), current.y(), current.x2(), streety-1));
		cityBSP(mRNG, lots, mapRectangle(current.x(), streety + STREET_WIDTH, current.x2(), current.y2()));

		//DEBUG TODO build the street immediately so we can see the way it works out
		borderedRectangle(current.x(), streety, current.width(), STREET_WIDTH, "pavers", "pavers");
	}
}

bool cityTester()
{
	//deleted at end of function
	TCODRandom* mRNG = new TCODRandom();

	testMap.fill("grass");

	borderedRectangle(5, 5, DUNGEONWIDTH - 10, DUNGEONHEIGHT - 10, "wall", "grass");

	delete mRNG;
	return true;
}

void rowhousesBuilder(TCODRandom* mRNG, mapRectangle rowhouse)
{
	//"vert" here  refers to the direction the walls dividing the houses run
	//For horizontal rowhouses, the dividing walls are vertical; else, horizontal
	bool vert = rowhouse.type() == BUILDING_HROWHOUSES;

	//Fill the rowhouse area with pavers and dirt; some tiles won't be used for buildings
	borderedRectangle(rowhouse.x(), rowhouse.y(), rowhouse.width(), rowhouse.height(),"pavers","pavers");
	randomFill(rowhouse.x(), rowhouse.y(), rowhouse.width(), rowhouse.height(), "dirt", 20, mRNG);

	//Assign each new rowhouse a random size until you run out of space
	//Then allocate remaining space randomly
	//The -1 is because the first rowhouse takes up one extra space for the first exterior wall
	unsigned totalSpace = (vert ? rowhouse.width() : rowhouse.height()) - 1;
	std::vector<unsigned> rowhouseSizes;

	//Large rowhouses need an alley breaking them up
	bool alley = totalSpace > MAX_ROWHOUSE_LENGTH_WITHOUT_ALLEY;

	//If this is false, the program would crash!
	assert(totalSpace >= MIN_ROWHOUSE_WIDTH);

	unsigned remainingSpace = totalSpace;
	while (remainingSpace >= MIN_ROWHOUSE_WIDTH)
	{
		//Alleys go in ~halfway, else allocate a rowhouse and deduct its space
		if (alley && remainingSpace <= totalSpace / 2)
		{
			//While alleys take up one tile, they necessitate an extra exterior wall
			rowhouseSizes.push_back(1);
			remainingSpace -= 2;
			alley = false;
		}
		else
		{
			unsigned curWidth = mRNG->getInt(MIN_ROWHOUSE_WIDTH, std::min(MAX_ROWHOUSE_WIDTH, remainingSpace));

			rowhouseSizes.push_back(curWidth);
			remainingSpace -= (curWidth - 1);
		}
	}
	//Allocate any remainder randomly to existing rowhouses
	while (remainingSpace > 0)
	{
		unsigned house = mRNG->getInt(0, rowhouseSizes.size() - 1);
		//Do not enlarge the alley
		if (rowhouseSizes[house] >= MIN_ROWHOUSE_WIDTH)
		{
			rowhouseSizes[house]++;
			remainingSpace--;
		}
	}
	//Build the rowhouses
	//TODO: Currently, rowhouses are just empty boxes with no doors!

	//Slightly randomize and track the y-coords of rowhouses, but not for the first or last ones
	int x = rowhouse.x();
	int y = rowhouse.y();
	//Length of wall perpendicular to the line of rowhouses
	int depth = vert ? rowhouse.height() : rowhouse.width();
	//Houses further from the edges of the row can be increasingly small
	int minDepth = depth;

	for (unsigned i = 0; i < rowhouseSizes.size(); i++)
	{
		//For each tile further in, a house can be 1 tile smaller than before
		if (i < rowhouseSizes.size() / 2)
			minDepth = depth - i;
		else
			minDepth = depth - (rowhouseSizes.size() - i - 1);
		//Global minimum still enforced!
		minDepth = std::max((int) MIN_ROWHOUSE_WIDTH, minDepth);

		//Curious test
		//This seems to look more run-down - for dangerous areas?
		minDepth = MIN_ROWHOUSE_WIDTH;

		if (rowhouseSizes[i] < MIN_ROWHOUSE_WIDTH)
		{
			//This is the alley
			//Not necessary to build, just use whatever floor we start with
			/*if (vert)
				borderedRectangle(x + 1, rowhouse.y(), 1, depth, "dirt", "dirt");
			else
				borderedRectangle(x, y + 1, rowdepth, 1, "dirt", "dirt");*/

			(vert ? x : y) += rowhouseSizes[i] + 1; // two unshared walls
		}
		else
		{
			//Depth can, but does not have, to be small in the middle of the row
			unsigned curDepth = mRNG->getInt(minDepth, vert ? rowhouse.height() : rowhouse.width());
			//Pick a valid starting coordinate based on the chosen depth
			if (vert)
			{
				y = mRNG->getInt(rowhouse.y(), rowhouse.y() + (rowhouse.height() - curDepth));
				borderedRectangle(x, y, rowhouseSizes[i], curDepth, "wall", "floor");

				//DEBUG quick & dirty door placement
				//mins/maxes are so doors don't face onto walls/edge of map
				if (y > 1)
					testMap.setTile(x + rowhouseSizes[i] / 2, y, "door");
				if (y + curDepth - 1 < DUNGEONHEIGHT - 2)
					testMap.setTile(x + rowhouseSizes[i] / 2, y + curDepth - 1, "door");
			}
			else
			{
				x = mRNG->getInt(rowhouse.x(), rowhouse.x() + (rowhouse.width() - curDepth));
				borderedRectangle(x, y, curDepth, rowhouseSizes[i], "wall", "floor");
				//DEBUG quick & dirty door placement
				if (x > 1)
					testMap.setTile(x, y + rowhouseSizes[i] / 2, "door");
				if (x + curDepth - 1 < DUNGEONWIDTH - 2)
					testMap.setTile(x + curDepth - 1, y + rowhouseSizes[i] / 2, "door");
			}

			(vert ? x : y) += rowhouseSizes[i] - 1;	// again, -1 for the shared wall
		}
	}

	//Debug lot corners
	/*
	testMap.setTile(rowhouse.x(), rowhouse.y(), "debug");
	testMap.setTile(rowhouse.x(), rowhouse.y2(), "debug");
	testMap.setTile(rowhouse.x2(), rowhouse.y(), "debug");
	testMap.setTile(rowhouse.x2(), rowhouse.y2(), "debug");
	*/
}


void randomFill(unsigned x, unsigned y, unsigned width, unsigned height,
	std::string newTile, int percent, TCODRandom* mRNG)
{
	for (unsigned i = x; i < x + width; i++)
	{
		for (unsigned j = y; j < y + height; j++)
		{
			if (mRNG->getInt(0, 99) < percent)
			{
				testMap.setTile(i, j, newTile);
			}
		}
	}
}
void randomFill(unsigned x, unsigned y, unsigned width, unsigned height,
	std::string newTile, std::string targetTiles, int percent, TCODRandom* mRNG)
{
	for (unsigned i = x; i < x + width; i++)
	{
		for (unsigned j = y; j < y + height; j++)
		{
			if (testMap.type(i,j) == targetTiles && mRNG->getInt(0, 99) < percent)
			{
				testMap.setTile(i, j, newTile);
			}
		}
	}
}

void borderedRectangle(unsigned x, unsigned y, unsigned width, unsigned height,
	std::string border, std::string interior)
{
	//Border
	for (unsigned i = x; i < x + width; i++)
	{
		testMap.setTile(i, y, border);
		testMap.setTile(i, y + height - 1, border);
	}
	for (unsigned j = y + 1; j < y + height - 1; j++)
	{
		testMap.setTile(x, j, border);
		testMap.setTile(x + width - 1, j, border);
	}

	//Interior
	for (unsigned i = x + 1; i < x + width - 1; i++)
	{
		for (unsigned j = y + 1; j < y + height - 1; j++)
		{
			testMap.setTile(i, j, interior);
		}
	}
}

void borderBuilder(unsigned x, unsigned y, unsigned width, unsigned height,
	std::string border)
{
	for (unsigned i = x; i < x + width; i++)
	{
		testMap.setTile(i, y, border);
		testMap.setTile(i, y + height - 1, border);
	}
	for (unsigned j = y + 1; j < y + height - 1; j++)
	{
		testMap.setTile(x, j, border);
		testMap.setTile(x + width - 1, j, border);
	}
}

void oneDoorRoom(unsigned x, unsigned y, unsigned width, unsigned height,
	std::string border, std::string interior, std::string door, TCODRandom* mRNG,
	direction doorWall)
{
	borderedRectangle(x, y, width, height, border, interior);
	if (doorWall == NO_DIRECTION)
	{
		doorWall = (direction)mRNG->getInt(1, 4);
	}
	if (doorWall == N)
	{
		testMap.setTile(mRNG->getInt(x + 1, x + width - 2), y, door);
	}
	else if (doorWall == S)
	{
		testMap.setTile(mRNG->getInt(x + 1, x + width - 2), y + width - 1, door);
	}
	else if (doorWall == E)
	{
		testMap.setTile(x + width - 1, mRNG->getInt(y + 1, y + width - 2), door);
	}
	else if (doorWall == W)
	{
		testMap.setTile(x, mRNG->getInt(y + 1, y + width - 2), door);
	}
}

void lineOfRoomsv2(int x1, int y1, int x2, int y2,
	unsigned thickness, unsigned roomSize, TCODRandom * mRNG, direction doorWall)
{

}

void lineOfRooms(int x1, int y1, int x2, int y2,
	unsigned thickness, unsigned roomSize, TCODRandom * mRNG, direction doorWall)
{
	//X must go right to left; reverse coordinates if necessary
	if (x1 > x2)
	{
		int temp = x1;
		x1 = x2;
		x2 = temp;
		temp = y1;
		y1 = y2;
		y2 = temp;
	}

	//Determine which dimension corresponds to thickness and roomSize
	//If change in x > change in y, line is horizontalish so thickness is vertical (y)
	//If change in y > change in x, line is verticalish and thickness is horizontal (x)
	//roomSize is opposite of thickness
	unsigned roomWidth;
	unsigned roomHeight;
	float deltaX;
	float deltaY;
	bool vert;
	// Fitting the line closely to the desired length along the primary axis can be a problem
	// This value is used to adjust the starting position so the line winds up closer to the desired length
	int startOffset;

	if (abs(x2 - x1) > abs(y2 - y1)) // horizontal
	{
		vert = false;
		roomHeight = thickness;
		roomWidth = roomSize;
		//delta in direction of line is less one because each new room shares a wall with the last
		deltaX = (float) roomWidth - 1;
		deltaY = (y2 - y1) / (abs(x2 - x1) / ((float)roomWidth));
		//The first room adds 1 more length to the line because it doesn't share its first wall
		//with a room that preceded it
		startOffset = ((x2 - x1) % (roomWidth-1)) / 2;
	}
	else //vertical
	{
		vert = true;
		roomHeight = roomSize;
		roomWidth = thickness;
		deltaX = (x2 - x1) / (abs(y2 - y1) / ((float)roomHeight));
		//Because x is always increasing, y is sometimes decreasing, so must check to invert deltaY
		deltaY = (int)y2 - y1 > 0 ? (float)roomHeight - 1 : -(float)roomHeight + 1;
		startOffset = (abs(y2 - y1) % (roomHeight-1)) / 2;
	}

	//Calculate change in upper-left corner of each room along the line
	//Change in y per room = change in y / number of rooms along x-axis
	//floats because delta is likely fractional along the less-important axis
	float x = (float) x1;
	float y = (float) y1;

	//Apply starting offset
	if (!vert)
	{
		x += startOffset;
	}
	else
	{
		deltaY >= 0 ? y += startOffset : y -= startOffset;
	}

	//Only check bounds on the primary dimension of the line (mainly x if horiz, y if vert)
	while (!vert && x + roomWidth <= x2 || vert && (deltaY >= 0 && y+roomHeight <= y2 || deltaY < 0 && y >= y2))
	{
		//Randomize wall of door for each room
		direction iterDoor;
		if (doorWall != NO_DIRECTION)
		{
			iterDoor = doorWall;
		}
		else
		{
			if (vert)
			{
				if (mRNG->getInt(0, 1))
				{
					iterDoor = W;
				}
				else
				{
					iterDoor = E;
				}
			}
			else
			{
				if (mRNG->getInt(0, 1))
				{
					iterDoor = N;
				}
				else
				{
					iterDoor = S;
				}
			}
		}
		oneDoorRoom((unsigned) x, (unsigned) y, roomWidth, roomHeight,
			"wall", "floor", "door", mRNG, iterDoor);
		x += deltaX;
		y += deltaY;
	}

	//Debug line placement by showing locations of line endpoints

	testMap.setTile(x1, y1, "window");
	testMap.setTile(x2, y2, "window");
}