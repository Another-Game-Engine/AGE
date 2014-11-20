#include <Render/Data.hh>

Data::operator size_t() const
{
	return (size_t(_type));
}

size_t Data::size() const
{
	return (_data.size());
}

void const *Data::data() const
{
	return (_data.data());
}

glm::vec2 const &Data::range() const
{
	return (_range);
}

Data &Data::range(glm::vec2 const &r)
{
	_range = r;
	return (*this);
}

