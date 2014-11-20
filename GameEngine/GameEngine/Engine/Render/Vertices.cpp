#include <Render/Vertices.hh>
#include <Render/Data.hh>
#include <algorithm>

Vertices::Vertices(std::vector<Data> const &data)
{
	_data.resize(data.size());
	size_t index = 0;
	std::for_each(data.begin(), data.end(), [&](Data const &data){
		_data[index++] = std::make_shared<Data>(data);
	});
}

Vertices::Vertices(std::vector<Data> &&data)
{
	_data.resize(std::move(data).size());
	size_t index = 0;
	std::for_each(data.begin(), data.end(), [&](Data &data){
		_data[index++] = std::make_shared<Data>(std::move(data));
	});
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
