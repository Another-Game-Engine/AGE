#include "System.h"
#include "Core/Engine.hh"

bool defaultEntityComparaison(Handle e1, Handle e2)
{
	return e1 < e2;
}

System::System(Engine &engine, bool(*comparacomparaisonFunction)(Handle, Handle))
: PubSub(engine.getInstance<PubSub::Manager>()),
_collection(comparacomparaisonFunction)
, _engine(engine)
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
