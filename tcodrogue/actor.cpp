#include "actor.h"

static std::unordered_map<std::string,creatureType*> _creatureTypes;

int mapObject::x() const
{
	return _x;
}

int mapObject::y() const
{
	return _y;
}
void mapObject::setXY(int x, int y)
{
	_x = x;
	_y = y;
}

mapObject::mapObject()
{
	_x = -1;
	_y = -1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Creature Types
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//e.g. "vault guard", "mean-looking mercenary"
std::string creatureType::title() const
{
	return _title;
}

char creatureType::tile() const
{
	return _tile;
}

TCODColor creatureType::color() const
{
	return _color;
}

int creatureType::speed() const
{
	return _speed;
}

bool creatureType::hostile() const
{
	return _hostile;
};

int creatureType::maxhp() const
{
	return _maxhp;
};

int creatureType::damage() const
{
	return _damage;
};

creatureType::creatureType(std::string title, char tile, TCODColor color, int speed, bool hostile, int maxhp, int damage)
{
	_title = title;
	_tile = tile;
	_color = color;
	_speed = speed;
	_hostile = hostile;
	_maxhp = maxhp;
	_damage = damage;
}


//Defines a creature type during data file loading
void defineCreatureType(creatureType* newType)
{
	if (_creatureTypes.count(newType->title()) == 0)
	{
		_creatureTypes[newType->title()] = newType;
	}
	else
	{
		// TODO : scream about identical type names. also, type name shouldn't need to be unique.
		delete newType;
	}
}

//Deletes creature type data during end of program
void deleteCreatureTypes()
{
	std::unordered_map<std::string, creatureType*>::iterator i;
	for (i = _creatureTypes.begin(); i != _creatureTypes.end(); i++)
	{
		delete i->second;
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Creature
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int creature::speed() const
{
	//Currently all creatures get 1 action per turn
	return TIME_SYSTEM_BASE;
}

bool creature::addMoves()
{
	_moves += speed();

	if (_hp < maxhp())
		_hp++;

	return _moves > 0;
}

void creature::spendMoves(int moves)
{
	if (moves > 0)
	{
		_moves -= moves;
	}
}

void creature::wait()
{
	//if you have no moves, do nothing
	if (_moves == 0)
		return;
	//make your last move a fraction if it's not
	if (_moves % TIME_SYSTEM_BASE == 0)
	{
		_moves -= 1;
	}
	//integer round off said fraction
	_moves = (_moves / TIME_SYSTEM_BASE) * TIME_SYSTEM_BASE;
}

bool creature::canMove()
{
	return _moves > 0;
}

std::string creature::title() const
{
	return _creatureTypes[_type]->title();
}

char creature::tile() const
{
	return _creatureTypes[_type]->tile();
}

TCODColor creature::color() const
{
	return _creatureTypes[_type]->color();
}

bool creature::hostile() const
{
	return _hostile;
}

void creature::becomeHostile()
{
	_hostile = true;
}

int creature::hp() const
{
	return _hp;
}

int creature::maxhp() const
{
	return _creatureTypes[_type]->maxhp();
}

int creature::damage() const
{
	return _creatureTypes[_type]->damage();
}


bool creature::takeDamage(int damage)
{
	if (damage > 0)
	{
		_hp -= damage;
	}
	return _hp < 1;
}

//Constructor

creature::creature(std::string type)
{
	_moves = 0;
	_hp = _creatureTypes[type]->maxhp();
	_type = type;
	_hostile = _creatureTypes[type]->hostile();
}
