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
	return *this;
}