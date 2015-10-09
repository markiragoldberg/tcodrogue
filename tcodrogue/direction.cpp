#include "direction.h"

direction keyToDirection(const TCOD_key_t &key)
{
	//numpad
	switch (key.vk)
	{
	case TCODK_KP1:
		return SW;
	case TCODK_KP2:
		return S;
	case TCODK_KP3:
		return SE;
	case TCODK_KP4:
		return W;
	case TCODK_KP6:
		return E;
	case TCODK_KP7:
		return NW;
	case TCODK_KP8:
		return N;
	case TCODK_KP9:
		return NE;
	}
	// hjklyubn
	switch (key.c)
	{
	case 'h':
	case 'H':
		return W;
	case 'j':
	case 'J':
		return S;
	case 'k':
	case 'K':
		return N;
	case 'l':
	case 'L':
		return E;
	case 'y':
	case 'Y':
		return NW;
	case 'u':
	case 'U':
		return NE;
	case 'b':
	case 'B':
		return SW;
	case 'n':
	case 'N':
		return SE;
	//neither numpad nor hjklyubn have hit a match at this point
	default:
		return NO_DIRECTION;
	}
}

bool keyToXY(const TCOD_key_t & key, int &x, int &y)
{
	direction dir = keyToDirection(key);

	return directionToXY(dir, x, y);
}

bool directionToXY(direction dir, int &x, int &y)
{
	switch (dir)
	{
	case SW:
		x--; y++;
		return true;
	case S:
		y++;
		return true;
	case SE:
		x++; y++;
		return true;
	case W:
		x--;
		return true;
	case E:
		x++;
		return true;
	case NW:
		x--; y--;
		return true;
	case N:
		y--;
		return true;
	case NE:
		x++; y--;
		return true;
	default:
		return false;
	}
}
