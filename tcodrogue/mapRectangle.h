#pragma once

enum buildingType
{
	BUILDING_ANY,
	BUILDING_VROWHOUSES,
	BUILDING_HROWHOUSES,
	BUILDING_BIGBUILDING
};

class mapRectangle
{
public:
	void setX(int x);
	void setY(int y);
	void setX2(int x2);
	void setY2(int y2);
	void setCoords(int x, int y, int x2, int y2);
	void setType(unsigned type);
	
	int x() const;
	int y() const;
	int x2() const;
	int y2() const;
	unsigned type() const;
	int width() const;
	int height() const;

	mapRectangle(int x, int y, int x2, int y2, unsigned type = BUILDING_ANY);
private:
	int _x, _y;
	int _x2, _y2;

	unsigned _type;
};