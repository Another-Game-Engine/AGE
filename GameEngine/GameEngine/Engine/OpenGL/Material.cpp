#include <OpenGL/Material.hh>
#include <memory>

namespace gl
{
	Material::Material()
	{
		_data = new uint8_t[LAST::size];
		memset(_data, 0, LAST::size);
	}

	Material::~Material()
	{
		delete[] _data;
	}

	Material::Material(Material const &copy)
	{
		_data = new uint8_t[LAST::size];
		memcpy(_data, copy._data, LAST::size);
	}

	Material &Material::operator=(Material const &m)
	{
		memcpy(_data, m._data, LAST::size);
		return (*this);
	}
}