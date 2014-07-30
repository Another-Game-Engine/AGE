#include <Render/Material.hh>
#include <memory>

namespace gl
{
	Material::Material()
	{
		_data = new uint8_t[Last::size];
		memset(_data, 0, Last::size);
	}

	Material::~Material()
	{
		delete[] _data;
	}

	Material::Material(Material const &copy)
	{
		_data = new uint8_t[Last::size];
		memcpy(_data, copy._data, Last::size);
	}

	Material &Material::operator=(Material const &m)
	{
		memcpy(_data, m._data, Last::size);
		return (*this);
	}

	uint8_t const *Material::getData(size_t offset) const
	{
		return (&_data[offset]);
	}
}