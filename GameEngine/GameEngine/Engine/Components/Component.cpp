#include <Components/Component.hh>
#include <Entities/EntityData.hh>

using namespace Component;

Base::Base(AScene *scene, Entity &entity)
: _scene(scene)
, _entity(entity)
{
}

Base::~Base()
{
}

Base &Base::operator=(const Base &other)
{
	return *this;
}

void			Base::setEntity(Entity &entity)
{
	_entity = entity;
}

Entity		&Base::getEntity()
{
	return _entity;
}