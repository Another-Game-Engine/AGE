#include <Components/Component.hh>
#include <Entities/EntityData.hh>

using namespace Component;

Base::Base(Entity &entity, std::size_t serId)
: _entity(entity)
, serializedID(serId)
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