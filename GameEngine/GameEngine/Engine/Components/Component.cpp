#include <Components/Component.hh>
#include <Entities/Entity.hh>

using namespace Component;

Base::Base(AScene *scene, Handle &entity, const std::string &name)
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

void			Base::setEntity(Handle &entity)
{
	_entity = entity;
}

Handle		&Base::getEntity()
{
	return _entity;
}

std::string const &Base::getName() const
{
	return _name;
}