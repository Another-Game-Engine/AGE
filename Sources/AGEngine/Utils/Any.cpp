#include "Any.hpp"

Any::~Any()
{
}

Any::Any(const Any &o)
{
	_data = o._data;
}