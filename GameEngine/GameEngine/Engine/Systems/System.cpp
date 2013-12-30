#include <Systems/System.h>
#include <Core/AScene.hh>

bool defaultEntityComparaison(Handle e1, Handle e2)
{
	return e1 < e2;
}

System::System(AScene *scene, bool(*comparacomparaisonFunction)(Handle, Handle))
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

void System::_componentAdded(Handle &e, unsigned int typeId)
{
	if (_code.match(e->getCode()))
		_collection.insert(e);
}

void System::_componentRemoved(Handle &e, unsigned int typeId)
{
	if (!_code.match(e->getCode()))
		_collection.erase(e);
}