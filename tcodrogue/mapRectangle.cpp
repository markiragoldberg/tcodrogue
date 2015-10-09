#include "mapRectangle.h"


void mapRectangle::setX(int x)
{
	_x = x;
}
void mapRectangle::setY(int y)
{
	_y = y;
}
void mapRectangle::setX2(int x2)
{
	_x2 = x2;
}
void mapRectangle::setY2(int y2)
{
	_y2 = y2;
}

void mapRectangle::setCoords(int x, int y, int x2, int y2)
{
	_x = x;
	_y = y;
	_x2 = x2;
	_y2 = y2;
}

void mapRectangle::setType(unsigned type)
{
	_type = type;
}

int mapRectangle::x() const
{
	return _x;
}
int mapRectangle::y() const
{
	return _y;
}
int mapRectangle::x2() const
{
	return _x2;
}
int mapRectangle::y2() const
{
	return _y2;
}

unsigned mapRectangle::type() const
{
	return _type;
}

int mapRectangle::width() const
{
	return _x2 - _x + 1;
}


int mapRectangle::height() const
{
	return _y2 - _y + 1;
}


mapRectangle::mapRectangle(int x, int y, int x2, int y2, unsigned type)
{
	_x = x;
	_y = y;
	_x2 = x2;
	_y2 = y2;
	_type = type;
}