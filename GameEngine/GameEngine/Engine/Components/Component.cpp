#include <Components/Component.hh>
#include <Entities/Entity.hh>

using namespace Component;

Base::Base(Engine &engine, Handle &entity, const std::string &name)
: _engine(engine)
, _name(name)
, _entity(entity)
{
}

Base::~Base(){}

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