#include "AResource.hh"

namespace	Resources
{

AResource::AResource(void) :
	_isLoaded(false)
{
}

AResource::~AResource(void)
{
}

bool			AResource::isLoaded() const
{
	return (_isLoaded);
}

void			AResource::setLoaded(bool loaded)
{
	_isLoaded = loaded;
}

AResource::ResourceType	AResource::getType() const
{
	return (_type);
}

}