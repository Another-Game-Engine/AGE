#include <Components/Component.hh>
#include <Entities/EntityData.hh>

using namespace Component;

Base::Base(AScene *scene, Entity &entity, const std::string &name)
: _scene(scene)
, _name(name)
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

std::string const &Base::getName() const
{
	return _name;
}