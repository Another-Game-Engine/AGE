#include "Attribute.hh"

Attribute::Attribute(GLenum type, uint8_t sizeType, uint8_t nbrComponent)
: _type(type),
_sizeType(sizeType),
_nbrComponent(nbrComponent)
{

}

Attribute::~Attribute()
{

}

Attribute::Attribute(Attribute const &copy)
: _type(copy._type),
_sizeType(copy._sizeType),
_nbrComponent(copy._nbrComponent)
{

}

Attribute &Attribute::operator=(Attribute const &attribute)
{
	_type = attribute._type;
	_sizeType = attribute._sizeType;
	_nbrComponent = attribute._nbrComponent;
	return (*this);
}

GLenum Attribute::getType() const
{
	return (_type);
}

uint8_t Attribute::getSizeType() const
{
	return (_sizeType);
}

uint8_t Attribute::getNbrComponent() const
{
	return (_nbrComponent);
}