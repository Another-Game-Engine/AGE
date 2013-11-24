#include "System.h"

bool defaultEntityComparaison(Entity *e1, Entity *e2)
{
	return e1 < e2;
}

System::System(bool(*comparacomparaisonFunction)(Entity*, Entity*))
: _collection(comparacomparaisonFunction)
{}

System::~System()
{
	_collection.clear();
}


void System::update(double time)
{
	updateBegin(time);
	mainUpdate(time);
	updateEnd(time);
}

void System::init()
{
	initialize();
}

const Barcode &System::getCode() const
{
	return _code;
}
