#include <Render/Vertices.hh>
#include <Render/Data.hh>

Vertices::Vertices(std::vector<Data> const &data)
{
	_data.resize(data.size());
	for (size_t index = 0; index < data.size(); ++index)
	{
		_data[index] = std::make_shared<Data>(data[index]);
	}
}

Vertices::Vertices(std::vector<Data> &&data)
{
	_data.resize(std::move(data).size());
	for (size_t index = 0; index < data.size(); ++index)
	{
		_data[index] = std::make_shared<Data>(std::move(data[index]));
	}
}

Vertices::iterator Vertices::begin()
{
	return (_data.begin());
}

Vertices::const_iterator Vertices::begin() const
{
	return  (_data.begin());
}

Vertices::iterator Vertices::end()
{
	return (_data.end());
}

Vertices::const_iterator Vertices::end() const
{
	return  (_data.end());
}
