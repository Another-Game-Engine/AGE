#include <Render/Data.hh>

Data::operator size_t() const
{
	return (size_t(_type));
}

size_t Data::getSize() const
{
	return (_data.size());
}

void const *Data::getData() const
{
	return (_data.data());
}

void *Data::getData()
{
	return (_data.data());
}