#include <Systems/System.h>
#include <Core/AScene.hh>

bool defaultEntityComparaison(Entity e1, Entity e2)
{
	return e1 < e2;
}

System::System(AScene *scene, bool(*comparacomparaisonFunction)(Entity, Entity))
: PubSub(scene->getInstance<PubSub::Manager>()),
_collection(comparacomparaisonFunction)
, _scene(scene)
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

void System::_componentAdded(Entity &e, unsigned int typeId)
{
	if (_code.match(e->getCode()))
		_collection.insert(e);
}

void System::_componentRemoved(Entity &e, unsigned int typeId)
{
	if (!_code.match(e->getCode()))
		_collection.erase(e);
}