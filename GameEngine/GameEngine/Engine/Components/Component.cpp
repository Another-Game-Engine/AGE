#include <Components/Component.hh>

using namespace Component;

Base::Base(std::size_t serId)
: serializedID(serId)
{
}

Base::~Base()
{
}

Base &Base::operator=(const Base &other)
{
	entityId = other.entityId;
	return *this;
}

Base::Base(const Base &other)
{
	entityId = other.entityId;
}

Base::Base(Base &&other)
{
	entityId = std::move(other.entityId);
}